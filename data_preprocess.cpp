#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main(){
    // string in[4]={"CG","CA","CH","FB"};
    // string out[4]={"CG1","CA1","CH1","FB1"};
    string in[3]={"AM","LJ","amazon"};
    string out[3]={"AM1","LJ1","amazon1"};
    int i=0;
    //for(int i=0; i<4; i++){
        ifstream myfile;
        myfile.open("./dataset/"+in[i]+".txt");
        string str;

        unordered_map<int, int> node_map;
        int nodes_count = 0;
        unordered_set<string> edge_set;

        // Read and write the first two lines
        ofstream outfile("./dataset/"+out[i]+".txt");
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
            string set_check,outfile_str;
            ss >> node1 >> node2;

            if(node_map[node1]>node_map[node2]){
                set_check=to_string(node_map[node2]) + "-" + to_string(node_map[node1]);
                outfile_str=to_string(node_map[node2]) +"\t"+ to_string(node_map[node1]);
            }else{
                set_check=to_string(node_map[node1]) + "-" + to_string(node_map[node2]);
                outfile_str=to_string(node_map[node1]) +"\t"+ to_string(node_map[node2]);
            }

            if(node1==node2 || edge_set.count(set_check))
                continue;

            edge_set.insert(set_check);
            // Output mapped node numbers

            outfile << outfile_str << endl;


        }
        myfile.close();
        outfile.close();
    //}
return 0;
}
