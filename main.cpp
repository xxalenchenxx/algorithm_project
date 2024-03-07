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
    int vertex=-1;
    int sup=-1;
    int k=-1;
    //int lowerBound_k=-1;
    //int upperBound_k=-1;

}NODE;

struct EDGE{
    int s;
    int t;
};
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
                    cout << it->vertex <<"("<<it->k<<")"<< " -> ";
                
                cout<<"null"<<endl;
            }
        }
        
        void python_draw_graph(){
            ofstream outfile("output.txt");
            if (outfile.is_open()) {
                for (int i=0;i< adj.size();i++) {
                    for(auto it = this->adj[i].begin(); it != this->adj[i].end(); it++)
                        outfile << i <<" "<< it->vertex<< " "<<it->k<<"\n";
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
            //find MNN nodes
            for(int i=0; i<Q1.size(); i++){
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j])
                        sup++;     
            }
            
            // cout<<"\n v neighbors:";
            // for(int i=0; i<Q1.size(); i++)
            //     cout<<Q1[i]<<" ";
            // cout<<"\n u neighbors:";
            // for(int i=0; i<Q2.size(); i++)
            //     cout<<Q2[i]<<" ";
            
            // cout<<"\n sup = "<<sup<<endl;

            // e(u,v) sup
            for(auto it = adj[v].begin(); it != adj[v].end(); it++)
                if(it->vertex==w)
                    it->sup=sup;
            // e(v,u) sup
            for(auto it = adj[w].begin(); it != adj[w].end(); it++)
                if(it->vertex==v)
                    it->sup=sup;

            return;
        }

        void compute_ALL_support(int *small_sup){
            for (int i=0;i< adj.size();i++) {
                    for(auto it = adj[i].begin(); it != adj[i].end(); it++)
                        if(i< it->vertex){
                            compute_edge_support(i,it->vertex);
                            if(it->sup<*small_sup)
                                *small_sup=it->sup;
                        }
                            
                }
        }


        vector<EDGE> effect_edge(int u,int v){
            vector<int> Q1, Q2;
            vector<EDGE> edge;
            
            Q1=tau_hop_neighbor(u);
            
            Q2=tau_hop_neighbor(v);
            
            // cout<<"\n u neighbors:";
            // for(int i=0; i<Q1.size(); i++)
            //     cout<<Q1[i]<<" ";
            // cout<<"\n v neighbors:";
            // for(int i=0; i<Q2.size(); i++)
            //     cout<<Q2[i]<<" ";
            
            //effect nodes
            vector<int> MNN;
            MNN.push_back(u);
            MNN.push_back(v);
            for(int i=0; i<Q1.size(); i++){
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j])
                        MNN.push_back(Q1[i]);   
            }
            
            //find effect node by edge
            for(int i=0; i<MNN.size(); i++)
                for(int j=0; j<MNN.size(); j++)
                    if((MNN[i]<MNN[j])&&find_edge(MNN[i],MNN[j]))
                        edge.push_back({MNN[i],MNN[j]});

            // for(int i=0; i<edge.size(); i++){
            //     cout<<"edge: "<<edge[i].s<<" , "<<edge[i].t<<endl;
            // }
            
            //delete edge(u,v)
            for(auto it=edge.begin(); it!=edge.end();)
                if((it->s==u&&it->t==v)||(it->s==v&&it->t==u))
                    it=edge.erase(it);
                else
                    it++;
      
            return edge;
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
        
        bool find_edge(int u,int v){
            for(auto it = adj[u].begin(); it != adj[u].end(); it++)
                if(it->vertex==v)
                    return true;
            return false;
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



bool edge_smaller_than_k(vector<list<node>> graph_adj,int k,int *u,int *v){
    for (int i=0;i< graph_adj.size();i++) {
        for(auto it = graph_adj[i].begin(); it != graph_adj[i].end(); it++)
            if(it->sup<=k){
                *u=i;
                *v=it->vertex;
                return true;
            }                    
    }
    return false;
}

int main(){
    Graph G_input;
    int node_num =0, edge_num =0;

    //-------------------input element----------------------------------
    string filename="./dataset/test.txt";//graph
    int tau=2;

    //read file
    if(!read_file(filename,&node_num,&edge_num,&G_input,tau))
        return EXIT_FAILURE;
    

    // cout << "Nodes: " << node_num << endl;
    // cout << "Edges: " << edge_num << endl;

    //compute_ALL_support
    int min_k=INT32_MAX;
    G_input.compute_ALL_support(&min_k);
    G_input.printGraph();
    Graph graph_adj=G_input;


 while(check_any_edge(graph_adj)){
    cout<<"---------------k="<<min_k+2<<"-----------------"<<endl;   
    int u,v;
    while(edge_smaller_than_k(graph_adj.adj,min_k,&u,&v)){
        vector<EDGE> effect_edge;
        cout<<"1"<<endl;
        //put k_truss in edge(u,v)
        for(auto it = G_input.adj[u].begin(); it != G_input.adj[u].end(); it++)
            if(it->vertex==v)
                it->k=min_k+2;

        for(auto it = G_input.adj[v].begin(); it != G_input.adj[v].end(); it++)
            if(it->vertex==u)
                it->k=min_k+2;
        
        //G'<-G\e(u,v)
        //Graph graph_before=graph_adj;
        effect_edge=graph_adj.effect_edge(u,v);
        graph_adj.removeEdge(u,v);
        
        
        //effect edge(u,v) after delete u,v
        //effect_edge=graph_before.effect_edge(u,v);
        
        for(int i=0; i<effect_edge.size(); i++)
            for(auto it = graph_adj.adj[effect_edge[i].s].begin(); it != graph_adj.adj[effect_edge[i].s].end(); it++)
                if(it->vertex==effect_edge[i].t && it->sup>min_k)
                    graph_adj.compute_edge_support(effect_edge[i].s,effect_edge[i].t);
        

        
        // cout<<"\n effect_edge size: "<<effect_edge.size()<<endl;
        // for(int i=0;i<effect_edge.size();i++){
        //     cout<<effect_edge[i].s<<" "<<effect_edge[i].t<<endl;
        // }
    }
    //graph_adj.python_draw_graph();
    min_k++;
 }

    G_input.printGraph();
    
    G_input.python_draw_graph();
    //graph_adj.python_draw_graph();
    
    
    //start HOtdecom
    // while(check_any_edge(G_input)){

    // }


    return 0;
}