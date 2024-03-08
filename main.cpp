#include"class.h"

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

void HOTdecom(Graph *G_input){
//start HOtdecom
    //compute_ALL_support
    int min_k=INT32_MAX;
    G_input->compute_ALL_support(&min_k);
    //G_input.printGraph();
    Graph graph_adj=*G_input;


 while(check_any_edge(graph_adj)){
    cout<<"---------------k="<<min_k+2<<"-----------------"<<endl;   
    int u,v;
    while(edge_smaller_than_k(graph_adj.adj,min_k,&u,&v)){
        vector<EDGE> effect_edge;
        cout<<"1"<<endl;
        //put k_truss in edge(u,v)
        

        for(auto it = G_input->adj[u].begin(); it != G_input->adj[u].end(); it++)
            if(it->vertex==v)
                it->k=min_k+2;

        for(auto it = G_input->adj[v].begin(); it != G_input->adj[v].end(); it++)
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
}


int main(){
    Graph G_input;
    int node_num =0, edge_num =0;

    //-------------------input element----------------------------------
    string filename="./dataset/test.txt";//graph
    int tau=2;

    //-------------------read file----------------------------------
    if(!read_file(filename,&node_num,&edge_num,&G_input,tau))
        return EXIT_FAILURE;
    

    // cout << "Nodes: " << node_num << endl;
    // cout << "Edges: " << edge_num << endl;


    HOTdecom(&G_input);

    G_input.printGraph();
    
    G_input.python_draw_graph();
    //graph_adj.python_draw_graph();
    
    
    //start HOtdecom
    // while(check_any_edge(G_input)){

    // }


    return 0;
}