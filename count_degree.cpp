#include"class.h"
#include <unordered_map>
#include <map>
bool read_file(string filename ,int *nodes,int *edges,Graph *G_in,int tau){
    ifstream myfile;string str;
    Graph G;
    int source = 0;
    int target = 0;
    //open file
    myfile.open(filename);
    if(!myfile.is_open()){
        cout << "Cannot open input file :(" << endl;
        return false;
    }

    while(getline(myfile, str)){
        stringstream ss(str);
        string token;

        ss >> token;
        if(token == "Nodes"){ //Nodes
            ss >> (*nodes);
            G.adj.resize(*nodes);
            //cout << "Nodes: " << *nodes << endl;
        }
        else if(token == "Edges"){ //Edges
            ss >> (*edges);
            //cout << "Edges: " << *edges << endl;
        }
        else {
            source= stoi(token);
            ss >> target;
            //cout << "Source: " << source << " , Target: " << target << endl;
            G.addEdge(source,target);
        }    
    }
    G.tau=tau;
    *G_in=G;
    
    return true;
}






int main(){
    Graph G_input;
    int node_num =0, edge_num =0;

    //-------------------input element----------------------------------
    string filename="./dataset/FB1.txt";//graph
    int tau=2;

    //-------------------read file----------------------------------
    if(!read_file(filename,&node_num,&edge_num,&G_input,tau))
        return EXIT_FAILURE;
    
    ofstream outfile("degree.txt");
    unordered_map<int, int> node_map;
    for(int i=0;i<G_input.adj.size();i++){
        int degree =0;
        for(auto it=G_input.adj[i].begin();it!=G_input.adj[i].end();it++){
           degree++;
        }
        node_map[degree]++;
    }
    
    map<int, int> sorted_map(node_map.begin(), node_map.end());
    //outfile<<to_string(degree)<<"\t"<<to_string(map int)<<endl;
    for (const auto& pair : sorted_map){
        outfile << pair.first << "\t" << pair.second << endl;
    }

    outfile.close();
    
    string cmd = "python show_degree_plot.py degree.txt";
    system(cmd.c_str());

    return 0;
}