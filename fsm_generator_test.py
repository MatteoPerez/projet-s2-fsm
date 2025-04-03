from lxml import etree  # etree is used to parse XML files

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

def print_graph(nodes, edges):
    print("Nodes:")
    for node in nodes:
        print(f"  - ID: {node['id']}, Label: {node['label']}")

    print("\nEdges:")
    for edge in edges:
        print(f"  - Source: {edge['source']}, Target: {edge['target']}, Label: {edge['label']}")

# Example
if __name__ == "__main__":
    graphml_file = "exp_vg_enemy.graphml"
    nodes, edges = parse_graphml(graphml_file)
    print_graph(nodes, edges)