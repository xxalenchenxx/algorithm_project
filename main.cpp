#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <queue>
#include <map>
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
        int tau=0;
        Graph(){};

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
            for(int i=0;i< adj.size();i++){
                cout<<i<<" ";
                for(auto it = adj[i].begin(); it != adj[i].end(); it++)
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
        
        void compute_edge_support(int v, int w){
            vector<int> Q1, Q2;
            int sup=0;
            Q1=tau_hop_neighbor(v);
            Q2=tau_hop_neighbor(w);
            
            for(int i=0; i<Q1.size(); i++){
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j])
                        sup++;     
            }

            cout<<"sup: "<<sup<<endl;
            for(auto it = adj[v].begin(); it != adj[v].end(); it++)
                if(it->vertex==w)
                    it->sup=sup;

            for(auto it = adj[w].begin(); it != adj[w].end(); it++)
                if(it->vertex==v)
                    it->sup=sup;

            return;
        }

        

    private:
        vector<int> tau_hop_neighbor(int v){
            queue<int> q;
            vector<bool> visited(adj.size(), false);
            map<int ,int> distances;
            vector<int> Q1;

            q.push(v);
            distances[v]=0;
            visited[v]=true;
            while(!q.empty()){
                int node = q.front();
                Q1.push_back(node);
                q.pop();
                
                if(distances[node]<tau){
                    for(auto it = adj[node].begin(); it != adj[node].end(); it++){
                        if(!visited[it->vertex]){
                            distances[it->vertex]=distances[node]+1;
                            visited[it->vertex]=true;
                            q.push(it->vertex);
                        }
                            
                    }   
                }

            }
            Q1.erase(Q1.begin());
            return Q1;
        }
        
};

bool check_any_edge(Graph G){
    for(int i=0; i<G.adj.size(); i++)
        if(G.adj[i].size())
            return true; 
    return false;
}

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
    string filename="./dataset/test.txt";//graph
    int tau=2;


    if(!read_file(filename,&node_num,&edge_num,&G_input,tau))
        return EXIT_FAILURE;
    

    cout << "Nodes: " << node_num << endl;
    cout << "Edges: " << edge_num << endl;

    G_input.compute_edge_support(2,1);
    // G_input.printGraph();
    //G_input.python_draw_graph();
    
    //start HOtdecom
    // while(check_any_edge(G_input)){

    // }


    return 0;
}