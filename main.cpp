#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

//struct node
typedef struct node{
    //node *next;
    int vertex=-1;
    int sup=-1;
    int lowerBound_k=-1;
    int upperBound_k=-1;

}NODE;


//class graph
class Graph{
    public:
        vector<list<node>> adj;
        Graph(){};

        // void resize_list(int node_number){
        //     adj.resize(node_number);
        // }
        void addEdge(int u, int v){
            node* s = new node; 
            s->vertex = v;
            adj[u].push_back(*s); 
            s = new node; 
            s->vertex = u;
            adj[v].push_back(*s); 
        }
        void removeEdge(int u, int v){
            adj[u].remove_if([v](NODE& n) { return n.vertex == v; });
            adj[v].remove_if([u](NODE& n) { return n.vertex == u; });
        }

        void printGraph(){
            for(int i=0;i< this->adj.size();i++){
                cout<<i<<" ";
                for(auto it = this->adj[i].begin(); it != this->adj[i].end(); it++)
                    cout << it->vertex << " -> ";
                
                cout<<"null"<<endl;
            }
        }
        
        void python_draw_graph(){
            ofstream outfile("output.txt");
            if (outfile.is_open()) {
                for (int i=0;i< adj.size();i++) {
                    for(auto it = this->adj[i].begin(); it != this->adj[i].end(); it++)
                        outfile << i<<" "<< it->vertex<< "\n";
                }
                outfile.close();
            } else {
                std::cout << "Unable to open file for writing." << std::endl;
                return;
            }

            // 呼叫 Python 程式
            std::string cmd = "python show_graph.py output.txt";
            system(cmd.c_str());
        }
        
        
        
};


bool read_file(string filename ,int *nodes,int *edges,Graph *G_in){
    ifstream myfile;string str;
    Graph G;
    int source = 0;
    int target = 0;
    //open file
    myfile.open(filename);
    if(!myfile.is_open()){
        cout << "無法開啟檔案" << endl;
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
    
    *G_in=G;
    return true;
}

int main(){
    Graph G_input;
    int node_num =0, edge_num =0;
    string filename="./dataset/test.txt";
    if(!read_file(filename,&node_num,&edge_num,&G_input))
        return EXIT_FAILURE;
    

    cout << "Nodes: " << node_num << endl;
    cout << "Edges: " << edge_num << endl;


    G_input.printGraph();
    G_input.python_draw_graph();

    return 0;
}