#include "class.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

bool read_file(const std::string& filename, int* nodes, int* edges, Graph* G_in, int tau) {
    std::ifstream myfile(filename);
    std::string str;
    int source = 0;
    int target = 0;

    if (!myfile.is_open()) {
        std::cout << "Cannot open input file :(" << std::endl;
        return false;
    }

    // Read Nodes and resize adj vector
    std::getline(myfile, str);
    std::istringstream nodes_line(str);
    nodes_line >> str >> *nodes;
    G_in->adj.resize(*nodes);

    // Read Edges
    std::getline(myfile, str);
    std::istringstream edges_line(str);
    edges_line >> str >> *edges;

    // Read edges and add to graph
    while (std::getline(myfile, str)) {
        std::istringstream ss(str);
        ss >> source >> target;
        G_in->addEdge(source, target);
    }

    G_in->tau = tau;

    return true;
}

int main() {
    Graph G_input;
    int node_num = 0, edge_num = 0;
    std::string in[3] = { "dblp1", "LJ1", "amazon1" };
    int i = 2;

    std::string filename = "../dataset/d/" + in[i] + ".txt";
    int tau = 2;

    if (!read_file(filename, &node_num, &edge_num, &G_input, tau))
        return EXIT_FAILURE;

    std::cout << "opened " << in[i] << "\n";
    std::ofstream outfile("degree.txt");
    if (!outfile.is_open()) {
        std::cerr << "Failed to open output file!" << std::endl;
        return EXIT_FAILURE;
    }

    // Count node degrees
    std::vector<unsigned int> node_degree(node_num, 0);
    for (const auto& adj_list : G_input.adj) {
        node_degree[adj_list.size()]++;
    }

    outfile << in[i] << std::endl;
    for (size_t j = 0; j < node_degree.size(); j++) {
        if (node_degree[j] > 0) {
            outfile << j << "\t" << node_degree[j] << std::endl;
        }
    }

    outfile.close();

    std::string cmd = "python show_degree_plot.py degree.txt";
    system(cmd.c_str());

    return 0;
}
