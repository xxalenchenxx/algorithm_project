#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

int main(){
    ifstream myfile;
    myfile.open("./dataset/CA.txt");
    string str;

    unordered_map<int, int> node_map;
    int nodes_count = 0;

    // Read and write the first two lines
    ofstream outfile("./dataset/CA_new.txt");
    getline(myfile, str);
    outfile << str << endl;
    getline(myfile, str);
    outfile << str << endl;

    // Construct the node map
    while(getline(myfile, str)){
        stringstream ss(str);
        int node1, node2;
        ss >> node1 >> node2;

        // Map original node number to new node number
        //determine first number mapped
        if(node_map.find(node1) == node_map.end()){
            node_map[node1] = nodes_count++;
        }
        if(node_map.find(node2) == node_map.end()){
            node_map[node2] = nodes_count++;
        }
    }

    // Reopen the input txt
    myfile.clear();
    myfile.seekg(0);

    // Skip the line of nodes and edges
    getline(myfile, str);
    getline(myfile, str);

    // Output to file
    while(getline(myfile, str)){
        stringstream ss(str);
        int node1, node2;
        ss >> node1 >> node2;

        if(node1==node2)
            continue;
        
        // Output mapped node numbers
        if(node_map[node1]>node_map[node2]){
            outfile << node_map[node2] << "\t" << node_map[node1] << endl;
        }else{
            outfile << node_map[node1] << "\t" << node_map[node2] << endl;
        }
 
        
    }

    


    myfile.close();
    outfile.close();

    return 0;
}
