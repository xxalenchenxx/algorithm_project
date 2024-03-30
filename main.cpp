#include"class.h"
#include <chrono>

bool check_any_edge(Graph G){
    for(int i=0; i<G.adj.size(); i++)
        if(G.adj[i].size())
            return true; 
    return false;
}
//-------------------read file function----------------------------------
bool read_file(string filename ,int *nodes,int *edges,Graph *G_in,int tau){
    ifstream myfile;string str;
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
            G_in->adj.resize(*nodes);
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
            G_in->addEdge(source,target,INT32_MAX,INT32_MAX);
        }    
    }
    G_in->tau=tau;
    
    return true;
}

//------------------- HOTdecom & HOTdecom+ function----------------------------------
bool edge_smaller_than_k(vector<list<node>> graph_adj,int sup,int *u,int *v){
    for (int i=0;i< graph_adj.size();i++) {
        for(auto it = graph_adj[i].begin(); it != graph_adj[i].end(); it++)
            if(it->sup<=sup){
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
    int min_sup=INT32_MAX;
    G_input->compute_ALL_support(&min_sup);
    //G_input.printGraph();
    Graph graph_adj=*G_input;


 while(check_any_edge(graph_adj)){
    cout<<"---------------k="<<min_sup+2<<"-----------------"<<endl;   
    int u,v;
    while(edge_smaller_than_k(graph_adj.adj,min_sup,&u,&v)){
        vector<EDGE> effect_edge;
        //cout<<"1"<<endl;
        //put k_truss in edge(u,v)
        

        for(auto it = G_input->adj[u].begin(); it != G_input->adj[u].end(); it++)
            if(it->vertex==v){
                it->k=min_sup+2;
                break;
            }
                

        for(auto it = G_input->adj[v].begin(); it != G_input->adj[v].end(); it++)
            if(it->vertex==u){
                it->k=min_sup+2;
                break;
            }
        //G'<-G\e(u,v)
        //Graph graph_before=graph_adj;
        effect_edge=graph_adj.effect_edge(u,v);
        graph_adj.removeEdge(u,v);
        
        
        //effect edge(u,v) after delete u,v
        //effect_edge=graph_before.effect_edge(u,v);
        
        for(int i=0; i<effect_edge.size(); i++)
            for(auto it = graph_adj.adj[effect_edge[i].s].begin(); it != graph_adj.adj[effect_edge[i].s].end(); it++)
                if(it->vertex==effect_edge[i].t && it->sup>min_sup)
                    graph_adj.compute_edge_support(effect_edge[i].s,effect_edge[i].t);
        

        
        // cout<<"\n effect_edge size: "<<effect_edge.size()<<endl;
        // for(int i=0;i<effect_edge.size();i++){
        //     cout<<effect_edge[i].s<<" "<<effect_edge[i].t<<endl;
        // }
    }
    //graph_adj.python_draw_graph();
    min_sup++;
 }
}

void HOTdecom_plus(Graph *G_input){
       int min_k=INT32_MAX;
    G_input->all_low_bound_compute(&min_k);
    //G_input.printGraph();
    Graph graph_adj=*G_input;
    cout<<"min_k: "<<min_k<<endl;
    
    
    


//for(int b=0;b<3;b++){

while(check_any_edge(graph_adj)){
    cout<<"-----------min_k: "<<min_k<<"-----------------"<<endl;
    
    //calculate support which low bound = min_k
    for(int i=0;i<graph_adj.adj.size();i++){
        for(auto it=graph_adj.adj[i].begin();it!=graph_adj.adj[i].end();it++) {
            if(it->lowerBound_k==min_k && i<it->vertex)
                graph_adj.compute_edge_support(i,it->vertex);
        }
    }



    int u,v;
    
    while(edge_smaller_than_k(graph_adj.adj,(min_k-2),&u,&v)){
        vector<EDGE> effect_edge;
        //cout<<"1"<<endl;

        //put k_truss in edge(u,v)
        for(auto it = G_input->adj[u].begin(); it != G_input->adj[u].end(); it++)
            if(it->vertex==v){
                it->k=min_k;
                break;
            }

        for(auto it = G_input->adj[v].begin(); it != G_input->adj[v].end(); it++)
            if(it->vertex==u){
                it->k=min_k;
                break;
            }


        effect_edge=graph_adj.effect_edge(u,v);
        graph_adj.removeEdge(u,v);

        //cout<<"remove e("<<u<<", "<<v<<")"<<endl;
        for(int i=0; i<effect_edge.size(); i++)
            for(auto it = graph_adj.adj[effect_edge[i].s].begin(); it != graph_adj.adj[effect_edge[i].s].end(); it++){
                if(it->vertex==effect_edge[i].t){
                    //delay update
                    
                    // cout<<"do delay update"<<endl;
                    if(it->lowerBound_k>min_k){
                        cout<<"real delay update"<<endl;
                        break;
                    }
                    // cout<<"no delay update"<<endl;

                    cout<<"do prunVertex!!"<<endl;
                    //early prunning
                    if(graph_adj.prunVertex(G_input,effect_edge[i].s , min_k)|| graph_adj.prunVertex(G_input,effect_edge[i].t , min_k) ){
                        cout<<"real prunVertex!!"<<endl;
                        break;
                    }
                    cout<<"no prunVertex!!"<<endl;

                    //unchange support
                    if(graph_adj.unchange_support(effect_edge[i].s,effect_edge[i].t,u,v)){
                        cout<<"unchange support!!"<<endl;
                        break;
                    }
                        


                    // cout<<"do default!!"<<endl;
                    //default case
                    if((it->sup+2) > min_k){
                        // cout<<"real default!!"<<endl;
                        graph_adj.compute_edge_support(effect_edge[i].s,effect_edge[i].t);
                    }
                    // cout<<"no default!!"<<endl;   
                
                
                } 
            }
                // if(it->vertex==effect_edge[i].t && it->sup>min_k)
                //     graph_adj.compute_edge_support(effect_edge[i].s,effect_edge[i].t);
    }
    //graph_adj.python_draw_graph();
    min_k++;
}
return;

}

//-------------------Top_r function----------------------------------
bool check_any_edge_is_kmax(Graph G,int k_max){
    for(int i=0; i<G.adj.size(); i++)
        for(auto it=G.adj[i].begin(); it!=G.adj[i].end(); it++)
            if(it->k==k_max)
                return true; 
    return false;
}



int main(){
    Graph G_input;
    int node_num =0, edge_num =0;

    //-------------------input element----------------------------------
    string filename="./dataset/test.txt";//graph
    int tau=2;
    //-------------------Top_r input element----------------------------------
    int top_r=2;
    //-------------------read file----------------------------------
    if(!read_file(filename,&node_num,&edge_num,&G_input,tau))
        return EXIT_FAILURE;
    
    

    
    
    //-------------------test degree is o?----------------------------------
    // G_input.printGraph();
    // for(int i=0;i<G_input.adj.size();i++){
    //     for(auto it=G_input.adj[i].begin();it!=G_input.adj[i].end();it++){
    //         if(it->lowerBound_k==INT32_MAX){
    //             cout<<"wrong"<<endl;
    //             return 0;
    //         }
    //     }
    // }

    // cout<<"success!!"<<endl;


    
    auto start = chrono::high_resolution_clock::now();
    // cout << "Nodes: " << node_num << endl;
    // cout << "Edges: " << edge_num << endl;

    //-------------------HOTdecom----------------------------------
    //HOTdecom(&G_input);



    //-------------------HOTdecom+----------------------------------
    //HOTdecom_plus(&G_input);
    

    //-------------------Top_r algorithm----------------------------------
    int min_sup_input=INT32_MAX;
    int min_sup_top=INT32_MAX;
    int min_low=INT32_MAX;
    int max_upper_k=-1;
    //G_input.all_low_bound_compute(&min_low);
    G_input.compute_ALL_support(&min_sup_input);
    G_input.all_low_bound_compute(&min_low);
    G_input.all_upper_bound_compute(&max_upper_k);
    cout<<"----------------------------------input graph----------------------------------\n";
    G_input.printGraph();

    //-------------------Top_g element & initial----------------------------------
    Graph graph_adj;//top G
    graph_adj.adj.resize(node_num);//give node_nums
    graph_adj.tau=tau;
    
    bool first=true;
    int k_max=max_upper_k;
    //cout<<"max_upper: "<<max_upper_k<<endl;
    
    //while(check_any_edge_is_kmax(graph_adj,k_max)){
    for(unsigned int i=0;i<1;i++){
        
        if(!first){
            if(1){//find k_max(not INT_32MAX) in graph_adj
                //code here
            }else{
                k_max-=top_r;
            }
        }
        int k=k_max-top_r+1;
        first=false;

        //add edges of upperbound >kmax-r into top_G
        //delete those edges from G_input
        for(int i=0; i<G_input.adj.size(); i++){
            for(auto it=G_input.adj[i].begin(); it!=G_input.adj[i].end();){
                auto current=it;
                it++;
                if(current->upperBound_k+top_r>k_max){
                    graph_adj.addEdge(i,current->vertex,current->lowerBound_k,current->upperBound_k);
                    G_input.removeEdge(i,current->vertex);
                }
            }
        }

        // cout<<"\n----------------------------------Top_G----------------------------------\n";
        // graph_adj.printGraph();
        // graph_adj.compute_ALL_support(&min_sup_top);
        // cout<<"\n----------------------------------Top_G recompute sup----------------------------------\n";
        // graph_adj.printGraph();

        // cout<<"\n----------------------------------G_input----------------------------------\n";
        // G_input.printGraph();
        // G_input.compute_ALL_support(&min_sup_input);
        // cout<<"\n----------------------------------G_input recompute sup----------------------------------\n";
        // G_input.printGraph();

        //compute support in TOP_G
        for(auto i = 0; i <graph_adj.adj.size(); i++) {
            for(auto it=graph_adj.adj[i].begin(); it != graph_adj.adj[i].end();it++){
                if(i<it->vertex){
                    if(it->k!=-1){
                        it->sup=it->k-2;
                        for(auto it1=graph_adj.adj[it->vertex].begin(); it1 != graph_adj.adj[it->vertex].end();it1++){
                            if(it1->vertex==i){
                                it1->sup=it->sup;
                            }
                        }
                    }else{
                        graph_adj.compute_edge_support(i,it->vertex);
                    }
                }
            }
        }
        Graph G_temp=graph_adj;
        while(k<=k_max){
            //while(check_any_edge(G_temp)){
                cout<<"-----------min_k: "<<k<<"-----------------"<<endl;

                
                int u,v;    
                while(edge_smaller_than_k(G_temp.adj,(k-2),&u,&v)){
                    vector<EDGE> effect_edge;

                    //put k_truss in edge(u,v)
                    for(auto it = graph_adj.adj[u].begin(); it != graph_adj.adj[u].end(); it++)
                        if(it->vertex==v){
                            it->k=k;
                            break;
                        }

                    for(auto it = graph_adj.adj[v].begin(); it != graph_adj.adj[v].end(); it++)
                        if(it->vertex==u){
                            it->k=k;
                            break;
                        }


                    effect_edge=G_temp.effect_edge(u,v);
                    G_temp.removeEdge(u,v);

                    //cout<<"remove e("<<u<<", "<<v<<")"<<endl;
                    for(int i=0; i<effect_edge.size(); i++){
                        for(auto it = G_temp.adj[effect_edge[i].s].begin(); it != G_temp.adj[effect_edge[i].s].end(); it++){
                            if(it->vertex==effect_edge[i].t){
                                //delay update

                                // cout<<"do delay update"<<endl;
                                if(it->lowerBound_k>k){
                                    cout<<"real delay update"<<endl;
                                    break;
                                }
                                // cout<<"no delay update"<<endl;

                                cout<<"do prunVertex!!"<<endl;
                                //early prunning
                                if(G_temp.prunVertex(&graph_adj,effect_edge[i].s , k)|| G_temp.prunVertex(&graph_adj,effect_edge[i].t , k) ){
                                    cout<<"real prunVertex!!"<<endl;
                                    break;
                                }
                                cout<<"no prunVertex!!"<<endl;

                                //unchange support
                                if(G_temp.unchange_support(effect_edge[i].s,effect_edge[i].t,u,v)){
                                    cout<<"unchange support!!"<<endl;
                                    break;
                                }



                                // cout<<"do default!!"<<endl;
                                //default case
                                if((it->sup+2) > k){
                                    // cout<<"real default!!"<<endl;
                                    G_temp.compute_edge_support(effect_edge[i].s,effect_edge[i].t);
                                }
                                // cout<<"no default!!"<<endl;   


                            } 
                        }
                    }    
                }
            //}
            graph_adj.python_draw_graph();
            k++;
        }

    }
    //-------------------time recorder end----------------------------------
    // auto end = std::chrono::high_resolution_clock::now();
    // double duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
    // cout << "run time: " << duration << " s\n";

    //cout<<"algorithm end!!"<<endl;
    
    //graph_adj.printGraph();
    //G_input.python_draw_graph();


    return 0;
}