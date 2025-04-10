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

    # Extract nodes
    nodes = []
    for node in root.xpath("//graphml:node", namespaces=namespaces):  # Use XPath to find all <node> elements in the GraphML file
        node_id = node.get("id")    # Get id
        
        label = node.xpath(".//y:NodeLabel/text()", namespaces=namespaces)  # Get label
        if label:
            label = label[0]
        else:
            f"Node_{node_id}"
        
        description = node.xpath(".//y:Description/text()", namespaces=namespaces)  # Get description
        if description:
            description = description[0]
        else:
            description = None
        
        nodes.append({"id": node_id, "label": label})

    # Extract edges
    edges = []
    for edge in root.xpath("//graphml:edge", namespaces=namespaces):  # Find all <edge> elements and use 'graphml' prefix
        source = edge.get("source")
        target = edge.get("target")
        label = edge.xpath(".//y:EdgeLabel/text()", namespaces=namespaces)
        if label:
            label = label[0]
        else:
            label = None
        edges.append({"source": source, "target": target, "label": label})

    return nodes, edges

def generate_c_code(nodes, edges, output_file="generated_fsm.c"):
    env = Environment(loader=FileSystemLoader('.'))
    template = env.get_template("fsm_template.c.j2")
    
    # Map node id to node data for easier access
    nodes_map = {node["id"]: node for node in nodes}
    
    # Choix du premier état (par défaut le premier dans la liste)
    initial_state = nodes[0]["label"] if nodes else "UNKNOWN"
    
    output = template.render(nodes=nodes, edges=edges, nodes_map=nodes_map, initial_state=initial_state)
    
    with open(output_file, "w") as f:
        f.write(output)
    
    print(f"Code C généré dans : {output_file}")

def print_graph(nodes, edges):
    print("Nodes:")
    for node in nodes:
        print(f"  - ID: {node['id']}, Label: {node['label']}")

    print("\nEdges:")
    for edge in edges:
        print(f"  - Source: {edge['source']}, Target: {edge['target']}, Label: {edge['label']}")

def choose_file():
    file = filedialog.askopenfilename()
    print(file)
    if file:
        extension = os.path.splitext(file)[1]
        print(extension)
        if extension == ".graphml":
            nodes, edges = parse_graphml(file)
            print_graph(nodes, edges)
            generate_c_code(nodes, edges)
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