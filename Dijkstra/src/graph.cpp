#include <iostream>
#include "graph.h"

void graph::loadGraph(std::string fileName) {
    std::ifstream infile(fileName);
    std::string line;
    std::string vertex1, vertex2, cost;
    node *pVertex1, *pVertex2;
    bool inHashTable;
    
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        iss >> vertex1 >> vertex2 >> cost;

        // If this is the first time we're seeing either node, push them o.dest
        // the list and hash table
        if (!this->nodeHash.contains(vertex1)) {
            node newNode = {.name = vertex1,
                            .d = INT_MAX,
                            .p = nullptr,
                            .known = false,
                            .adjacency = {}};
            this->graphNodes.push_back(newNode);
            this->nodeHash.insert(vertex1, &newNode); // FIXME is it &newNode?
        }

        if (!this->nodeHash.contains(vertex2)) {
            node newNode = {.name = vertex2,
                            .d = INT_MAX,
                            .p = nullptr,
                            .known = false,
                            .adjacency = {}};
            this->graphNodes.push_back(newNode);
            this->nodeHash.insert(vertex2, &newNode); // FIXME is it &newNode?
        }

        // Add the edge.dest vertex1's adjacency list
        pVertex1 = static_cast<node *>(this->nodeHash.getPointer(vertex1,
                    &inHashTable));
        pVertex2 = static_cast<node *>(this->nodeHash.getPointer(vertex2,
                    &inHashTable));
        edge newEdge = {.dest = pVertex2,
                        .cost = stoi(cost)};
        pVertex1->adjacency.push_back(newEdge);
    }

    infile.close();
}

void graph::dijkstra(std::string startingVertex) {
    node *pStartingVertex
        = static_cast<node *>(this->nodeHash.getPointer(startingVertex));
    pStartingVertex->d = 0;

    // Build heap of edges
    heap edgeHeap(graphNodes.size());
    for (edge startingEdge : pStartingVertex->adjacency) {
        edgeHeap.insert(startingEdge.dest->name, startingEdge.cost,
                &startingEdge.dest);
    }

    std::string *pName = nullptr; // name of edge's dest
    int *pCost = nullptr;         // cost of edge
    node *pNode = nullptr;        // pointer to edge's dest
    void *ppNode = nullptr;       // pointer to pointer to edge's dest

    while (!edgeHeap.deleteMin(pName, pCost, ppNode)) {
        pNode = static_cast<node *>(ppNode); 
        pNode->known = true;

        for (edge outgoingEdge : pNode->adjacency) {
            if (!outgoingEdge.dest->known &&
                    pNode->d + outgoingEdge.cost < outgoingEdge.dest->d) {
                outgoingEdge.dest->d = pNode->d + outgoingEdge.cost;
                outgoingEdge.dest->p = pNode;
            }
        }
    }
}

void graph::outputDijkstra(std::string startingVertex, std::string fileName) {
    std::ofstream outfile(fileName);
    std::string line = "";
    std::string path = "";
    node tmp = *this->graphNodes.begin();

    for (node graphNode : this->graphNodes) {
        line = graphNode.name;
        line += ": ";

        if (graphNode.p != nullptr) {
            line += std::to_string(graphNode.d);
            line += " [";

            path = "";
            tmp = graphNode;
            while (tmp.name != startingVertex) {
                path = tmp.name + ", " + path;
                tmp = *tmp.p;
            }

            line += path;
            line += "]\n";
        }
        else {
            line += "NO PATH\n";
        }

        outfile << line;
    }

    outfile.close();
}

bool graph::isValidVertex(std::string vertexName) {
    return this->nodeHash.contains(vertexName);
}
