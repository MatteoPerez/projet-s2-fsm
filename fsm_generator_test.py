import os
import tkinter as tk
from tkinter import filedialog
from lxml import etree  # etree is used to parse XML files
from jinja2 import Environment, FileSystemLoader

# Parse the GraphML file
def parse_graphml(file_path):
    tree = etree.parse(file_path)   # Read and parse the GraphML file
    root = tree.getroot()   # Get the root element of the XML tree (i.e., <graphml> element in the file).

    # Define namespaces (GraphML uses namespaces to avoid conflicts between different XML schemas)
    namespaces = {
        'graphml': "http://graphml.graphdrawing.org/xmlns",  # Main namespace for GraphML
        'y': "http://www.yworks.com/xml/graphml"    # Namespace used by yEd 
    }

    fsms = {
        "global": {"nodes": [], "edges": []}
    }

    # Extract nodes
    def extract_node(node):
        node_id = node.get("id")    # Get id
        label = node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)  # Get label
        if label:
            label = label[0]
        else:
            f"Node_{node_id}"

        description_data = node.xpath("./graphml:data", namespaces=namespaces)
        description = ""
        for data in description_data:
            if data.text:
                description = data.text.strip()
                print(description)
                if description.startswith("<![CDATA["):
                    description = description[9:-3]
        if not description:
            description = "No description"
        return {"id": node_id, "label": label, "description": description}
    
    for node in root.xpath("//graphml:node", namespaces=namespaces):    # Use XPath to find all <node> elements in the GraphML file
        is_group = node.xpath(".//graphml:graph", namespaces=namespaces)
        if not is_group:
            fsms["global"]["nodes"].append(extract_node(node))

    # Extract edges
    for edge in root.xpath("//graphml:edge", namespaces=namespaces):  # Find all <edge> elements and use 'graphml' prefix
        source = edge.get("source")
        target = edge.get("target")
        label = edge.xpath(".//y:EdgeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = None
        fsms["global"]["edges"].append({"source": source, "target": target, "label": label})

    for group_node in root.xpath("//graphml:node[graphml:graph]", namespaces=namespaces):
        group_label = group_node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)
        group_label = group_label[0].strip() if group_label else group_node.get("id")
        
        fsms[group_label] = {"nodes": [], "edges": []}
        
        subgraph = group_node.find(".//graphml:graph", namespaces=namespaces)
        if subgraph is not None:
            for subnode in subgraph.xpath(".//graphml:node", namespaces=namespaces):
                fsms[group_label]["nodes"].append(extract_node(subnode))
            for subedge in subgraph.xpath(".//graphml:edge", namespaces=namespaces):
                source = subedge.get("source")
                target = subedge.get("target")
                label = subedge.xpath(".//y:EdgeLabel/text()", namespaces=namespaces)
                label = label[0].strip() if label else ""
                fsms[group_label]["edges"].append({"source": source, "target": target, "label": label})

    return fsms

def generate_c_code(fsms, output_file="generated_fsm.c"):
    env = Environment(loader=FileSystemLoader('.'))
    template = env.get_template("fsm_template.c.j2")    
    output = template.render(fsms=fsms)
    with open(output_file, "w") as f:
        f.write(output)
    print(f"Code C généré dans : {output_file}")

def print_fsms(fsms):
    for fsm_name, fsm in fsms.items():
        print(f"FSM: {fsm_name}")
        print(" Nodes:")
        for node in fsm["nodes"]:
            print(f"   - ID: {node['id']}, Label: {node['label']}, Description: {node['description']}")
        print(" Edges:")
        for edge in fsm["edges"]:
            print(f"   - Source: {edge['source']}, Target: {edge['target']}, Label: {edge['label']}")
        print("")

def choose_file():
    file = filedialog.askopenfilename()
    print(file)
    if file:
        extension = os.path.splitext(file)[1]
        print(extension)
        if extension == ".graphml":
            fsms = parse_graphml(file)
            print_fsms(fsms)
            generate_c_code(fsms)
    else:
        print("Error when selecting file")

window = tk.Tk()
window.title("FSM generator")

choose_button = tk.Button(window, text="Choose file", command=choose_file)
choose_button.pack()

window.mainloop()

# # Example
# if __name__ == "__main__":
#     graphml_file = "exp_vg_enemy.graphml"
#     nodes, edges = parse_graphml(graphml_file)
#     print_graph(nodes, edges)