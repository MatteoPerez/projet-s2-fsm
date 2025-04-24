import os
import gzip
import shutil
import tkinter as tk
from tkinter import filedialog
from lxml import etree # etree is used to parse XML files
from jinja2 import Environment, FileSystemLoader

# Parse the GraphML file
def parse_graphml(file_path):
    tree = etree.parse(file_path) # Read and parse the GraphML file
    root = tree.getroot() # Get the root element of the XML tree (i.e., <graphml> element in the file).

    # Define namespaces (GraphML uses namespaces to avoid conflicts between different XML schemas)
    namespaces = {
        'graphml': "http://graphml.graphdrawing.org/xmlns", # Main namespace for GraphML
        'y': "http://www.yworks.com/xml/graphml" # Namespace used by yEd 
    }

    fsms = {}

    # Extract nodes
    def extract_node(node):
        node_id = node.get("id")
        label = node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = f"Node_{node_id}"

        # extract description
        description_data = node.xpath("./graphml:data[@key='d5']", namespaces=namespaces)
        description = ""
        for data in description_data:
            if data.text:
                description = data.text.strip()
                if description.startswith("<![CDATA["):
                    description = description[9:-3]
        if not description:
            description = "No description"
        
        # Extract shape
        shape = node.xpath(".//y:Shape/@type", namespaces=namespaces)
        if shape:
            shape = shape[0]
        else:
            shape = "Ellipse"

        return {"id": node_id, "label": label, "description": description, "shape": shape}

    # Extract edges
    def extract_edge(edge):
        source = edge.get("source")
        target = edge.get("target")
        label = edge.xpath(".//y:EdgeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = None

        # Extract description
        description_data = edge.xpath("./graphml:data[@key='d9']", namespaces=namespaces)
        description = ""
        for data in description_data:
            if data.text:
                description = data.text.strip()
                if description.startswith("<![CDATA["):
                    description = description[9:-3]
        if not description:
            description = "No description"

        return {"label": label, "source": source, "target": target, "description": description}

    def process_graph(graph_element, graph_name):
        fsm = {"nodes": [], "edges": []}

        # Extract all variables
        global_data = graph_element.xpath("./graphml:data[@key='d0']", namespaces=namespaces)
        if global_data:
            if global_data[0].text:
                fsm["globals"] = global_data[0].text.strip() 
            else:
                fsm["globals"] = ""

        for node in graph_element.findall("graphml:node", namespaces=namespaces):
            subgraph = node.find("graphml:graph", namespaces=namespaces)
            if subgraph is not None:
                label = node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)
                label = label[0].strip() if label else node.get("id")
                process_graph(subgraph, label)
            else:
                fsm["nodes"].append(extract_node(node))

        for edge in graph_element.findall("graphml:edge", namespaces=namespaces): # Find all <edge> elements and use 'graphml' prefix
            fsm["edges"].append(extract_edge(edge))
        
        initial_node = next((node for node in fsm["nodes"] if node["shape"] == "octagon"), None)
        if initial_node:
            fsm["initial_state"] = initial_node["label"] 
        else:
            fsm["initial_state"] = fsm["nodes"][0]["label"] if fsm["nodes"] else "None" # First node is default if no octagon is found

        fsms[graph_name] = fsm
    
    top_graph = root.find(".//graphml:graph", namespaces=namespaces)
    if top_graph is not None:
        process_graph(top_graph, "global")
    
    return fsms

def generate_c_code(fsms, multitasking, output_file="generated_fsm.c"):
    env = Environment(loader=FileSystemLoader('templates'))
    if(multitasking == "preemptive"):
        template = env.get_template("fsm_template_preemptive.c.j2")
    elif(multitasking == "cooperative"):
        template = env.get_template("fsm_template_cooperative.c.j2")
    elif(multitasking == "rtos"):
        template = env.get_template("fsm_template_rtos.c.j2")
    else:
        print("Error when generating C code")
        return
    
    output = template.render(fsms=fsms)
    with open(output_file, "w", encoding="utf-8") as f:
        f.write(output)
    print(f"C code with {multitasking} in : {output_file}")

def validate_fsms(fsms):
    errors = []
    for fsm_name, fsm in fsms.items():
        for node in fsm["nodes"]:
            if not node["description"] or node["description"] == "No description":
                errors.append(f"FSM '{fsm_name}': Node '{node['label']}' (ID: {node['id']}) has no description.")
        for edge in fsm["edges"]:
            if not edge["label"] or edge["label"] == "No description":
                errors.append(f"Edge from '{edge['source']}' to '{edge['target']}' has no description.")

    if errors:
        print("Validation Errors:")
        for error in errors:
            print(f"    - {error}")
    else:
        print("All FSMs are valid!")

def print_fsms(fsms):
    def print_graph(fsm_name, fsm, indent=""):
        print(f"{indent}FSM: {fsm_name}")
        print(f"{indent}Nodes:")
        for node in fsm["nodes"]:
            print(f"{indent}    - ID: {node['id']}, Label: {node['label']}, Description: {node['description']}, Shape: {node['shape']}")
            if "subgraph" in node:
                print(f"{indent}    Subgraph:")
                print_graph(node["label"], node["subgraph"], indent + "        ")

    for fsm_name, fsm in fsms.items():
        print_graph(fsm_name, fsm)
        print()
    
    for fsm_name, fsm in fsms.items():
        print(f"FSM: {fsm_name}")
        print("Globals:")
        print(f"    {fsm.get('globals', 'No globals defined')}")
        print("Edges:")
        for edge in fsm["edges"]:
            print(f"    - Source: {edge['source']}, Target: {edge['target']}, Label: {edge['label']}, Description: {edge['description']}")

def choose_file(multitasking):
    file = filedialog.askopenfilename()
    # print(file)
    if file:
        extension = os.path.splitext(file)[1]
        # print(extension)
        if extension == ".graphml":
            fsms = parse_graphml(file)
        elif extension == ".graphmlz":
            base_name = os.path.basename(os.path.splitext(file)[0])
            output_path = os.path.join("graphs", f"{base_name}_decompressed.graphml")
            with gzip.open(file, 'rb') as f_in:
                with open(output_path, 'wb') as f_out:
                    shutil.copyfileobj(f_in, f_out)
            fsms = parse_graphml(file)
        else:
            print("Error: extension not supported")
            return
        validate_fsms(fsms)
        print_fsms(fsms)
        generate_c_code(fsms, multitasking)
    else:
        print("Error when selecting file")

window = tk.Tk()
window.title("C code generator for FSM")

label = tk.Label(window, text="Preemptive multitasking")
label.pack()
choose_button_1 = tk.Button(window, text="Choose file", command=lambda: choose_file("preemptive"))
choose_button_1.pack()

label = tk.Label(window, text="Cooperative multitasking")
label.pack()
choose_button_2 = tk.Button(window, text="Choose file", command=lambda: choose_file("cooperative"))
choose_button_2.pack()

window.mainloop()

# # Example
# if __name__ == "__main__":
#     graphml_file = "exp_vg_enemy.graphml"
#     nodes, edges = parse_graphml(graphml_file)
#     print_graph(nodes, edges)