#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

//struct node
typedef struct node{
    node *next;
    int vertex=-1;
    int sup=-1;
    int lowerBound_k=-1;
    int upperBound_k=-1;

}NODE;


//class graph
class Graph{
    public:
        vector<list<node>> adj;
        Graph();

        // void resize_list(int node_number){
        //     adj.resize(node_number);
        // }
        void addEdge(int u, int v){
            node* s= new node;
            s->vertex=v;
            adj[u].push_back(*s);
            s->vertex=u;
            adj[v].push_back(*s);
        }
        void removeEdge(int u, int v){
            adj[u].remove_if([v](NODE& n) { return n.vertex == v; });
            adj[v].remove_if([u](NODE& n) { return n.vertex == u; });
        }
        void printGraph(){
            
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
            ss >> *nodes;
            cout << "Nodes: " << nodes << endl;
        }
        else if(token == "Edges"){ //Edges
            ss >> *edges;
            cout << "Edges: " << edges << endl;
        }
        else {
            ss >> source >> target;
            cout << "Source: " << source << " , Target: " << target << endl;
            G.addEdge(source,target);
        }    
    }
    *G_in=G;
    return true;
}

int main(){
    Graph G_input;
    int node_num =0, edge_num =0;
    string filename="";
    if(!read_file(filename,&node_num,&edge_num,&G_input))
        return EXIT_FAILURE;
   
    for(int i=0;i< G_input.adj.size();i++){
        cout<<i<<" ";
        for(int j=0;j< G_input.adj[i].size();j++){
            //cout<< G_input.adj[i].<<"->";
        }
        cout<<"null"<<endl;
    }


    return 0;
}