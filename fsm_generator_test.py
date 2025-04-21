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

    fsms = {
        "global": {"nodes": [], "edges": []}
    }

    # Extract nodes
    def extract_node(node):
        node_id = node.get("id")
        label = node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = f"Node_{node_id}"

        description_data = node.xpath("./graphml:data[@key='d5']", namespaces=namespaces)
        description = ""
        for data in description_data:
            if data.text:
                description = data.text.strip()
                if description.startswith("<![CDATA["):
                    description = description[9:-3]
        if not description:
            description = "No description"
        
        shape = node.xpath(".//y:Shape/@type", namespaces=namespaces)
        if shape:
            shape = shape[0]
        else:
            shape = "Ellipse"

        return {"id": node_id, "label": label, "description": description, "shape": shape}
    
    for node in root.xpath("//graphml:node", namespaces=namespaces): # Use XPath to find all <node> elements in the GraphML file
        is_group = node.xpath(".//graphml:graph", namespaces=namespaces)
        if not is_group:
            fsms["global"]["nodes"].append(extract_node(node))

    # Extract edges
    def extract_edge(edge):
        source = edge.get("source")
        target = edge.get("target")
        label = edge.xpath(".//y:EdgeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = None

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

    for edge in root.xpath("//graphml:edge", namespaces=namespaces): # Find all <edge> elements and use 'graphml' prefix
        fsms["global"]["edges"].append(extract_edge(edge))

    for group_node in root.xpath("//graphml:node[graphml:graph]", namespaces=namespaces):
        group_label = group_node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)
        group_label = group_label[0].strip() if group_label else group_node.get("id")
        
        fsms[group_label] = {"nodes": [], "edges": []}
        
        subgraph = group_node.find(".//graphml:graph", namespaces=namespaces)
        if subgraph is not None:
            for subnode in subgraph.xpath(".//graphml:node", namespaces=namespaces):
                fsms[group_label]["nodes"].append(extract_node(subnode))
            # Edges are not under nodes in the .graphml file
            # for subedge in subgraph.xpath(".//graphml:edge", namespaces=namespaces):
            #     print(subedge)
            #     fsms[group_label]["edges"].append(extract_edge(subedge))

    for fsm_name, fsm in fsms.items():
        # Find the node with the shape "octagon" to set as the initial state
        initial_node = next((node for node in fsm["nodes"] if node["shape"] == "octagon"), None)
        if initial_node:
            fsm["initial_state"] = initial_node["label"]
        else:
            fsm["initial_state"] = fsm["nodes"][0]["label"] # First node is default if no octagon is found

    return fsms

def generate_c_code(fsms, multitasking, output_file="generated_fsm.c"):
    env = Environment(loader=FileSystemLoader('.'))
    if(multitasking == "preemptive"):
        template = env.get_template("fsm_template_preemptive.c.j2")    
        output = template.render(fsms=fsms)
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(output)
        print(f"C code with preemptive multitasking generated in : {output_file}")
    elif(multitasking == "cooperative"):
        template = env.get_template("fsm_template_cooperative.c.j2")    
        output = template.render(fsms=fsms)
        with open(output_file, "w", encoding="utf-8") as f:
            f.write(output)
        print(f"C code with cooperative multitasking generated in : {output_file}")
    else:
        print("Error when generating C code")

def print_fsms(fsms):
    for fsm_name, fsm in fsms.items():
        print(f"FSM: {fsm_name}")
        print("Nodes:")
        for node in fsm["nodes"]:
            print(f"    - ID: {node['id']}, Label: {node['label']}, Description: {node['description']}")
        print("Edges:")
        for edge in fsm["edges"]:
            print(f"    - Source: {edge['source']}, Target: {edge['target']}, Label: {edge['label']}, Description: {edge['description']}")
        print("")

def choose_file(multitasking):
    file = filedialog.askopenfilename()
    # print(file)
    if file:
        extension = os.path.splitext(file)[1]
        # print(extension)
        if extension == ".graphml":
            fsms = parse_graphml(file)
            print_fsms(fsms)
            generate_c_code(fsms, multitasking)
        elif extension == ".graphml.gz":
            base_name = os.path.splitext(file)[0]
            output_path = f"{base_name}_decompressed.graphml"
            with gzip.open(file, 'rb') as f_in:
                with open(output_path, 'wb') as f_out:
                    shutil.copyfileobj(f_in, f_out)
            fsms = parse_graphml(file)
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