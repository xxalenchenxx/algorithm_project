#ifndef class_H
#define class_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <set>
#include <queue>
#include <map>
using namespace std;

//struct node
typedef struct node{
    int vertex=-1;
    int sup=INT32_MAX;
    int k=-1;
    int lowerBound_k=INT32_MAX;
    int upperBound_k=INT32_MAX;

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

        void addEdge(int u, int v,int lowerBound_k,int upperBound_k){
            node* s = new node; 
            s->vertex = v;
            s->lowerBound_k = lowerBound_k;
            s->upperBound_k = upperBound_k;
            adj[u].push_back(*s); 
            s = new node; 
            s->vertex = u;
            s->lowerBound_k = lowerBound_k;
            s->upperBound_k = upperBound_k;
            adj[v].push_back(*s);
            return; 
        }
        void removeEdge(int u, int v){
            adj[u].remove_if([v](NODE& n) { return n.vertex == v; });
            adj[v].remove_if([u](NODE& n) { return n.vertex == u; });
            return;
        }

        void printGraph(){
            for(int i=0;i< adj.size();i++){
                cout<<i<<" ";
                for(auto it = adj[i].begin(); it != adj[i].end(); it++)
                    cout << it->vertex <<"("<<it->upperBound_k<<")"<< " -> ";
                
                cout<<"null\tlist size: "<<adj[i].size()<<endl;
            }
            return;
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
                cout << "Unable to open file for writing." << endl;
                return;
            }

            // call python plt
            std::string cmd = "python show_graph.py output.txt";
            system(cmd.c_str());
            return;
        }
        
        void compute_edge_support(int v, int w){
            vector<int> Q1, Q2;
            int sup=0;
            Q1=tau_hop_neighbor(v,tau);
            Q2=tau_hop_neighbor(w,tau);
            //find MNN nodes
            for(int i=0; i<Q1.size(); i++){
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j])
                        sup++;     
            }
            

            // e(u,v) sup
            for(auto it = adj[v].begin(); it != adj[v].end(); it++)
                if(it->vertex==w){
                    it->sup=sup;
                    break;
                }
                    


            // e(v,u) sup
            for(auto it = adj[w].begin(); it != adj[w].end(); it++)
                if(it->vertex==v){
                    it->sup=sup;
                    break;
                }
                    

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
                return;
        }


        vector<EDGE> effect_edge(int u,int v){
            vector<int> Q1, Q2;
            vector<EDGE> edge;
            
            Q1=tau_hop_neighbor(u,tau);
            Q2=tau_hop_neighbor(v,tau);
            
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


        void all_low_bound_compute(int *small_low_bound){
            
            for (int i=0;i< adj.size();i++) {
                for(auto it = adj[i].begin(); it != adj[i].end(); it++)
                    if(i < it->vertex){
                        low_bound_compute(i,it->vertex);
                        if(it->lowerBound_k < *small_low_bound)
                            *small_low_bound=it->lowerBound_k;
                    }
                            
            }
            return;
        }

        bool prunVertex(Graph *G_input,int v, int min_k){
            vector<int> V;
            queue<int> Q2; //BFS
            V=tau_hop_neighbor(v,tau);
            int degree_v = V.size();
            
            // cout<<v<<" of "<<"degree: "<<degree_v<<endl;
            if((degree_v+1)<=min_k){
                Q2.push(v);
                while(!Q2.empty()){
                    
                    int q=Q2.front();
                    Q2.pop();
                    
                    V=tau_hop_neighbor(q,tau);
                   
                    //put truss in e(u,v) & e(v,u)
                    //cout<<"1"<<endl;
                    while(!adj[q].empty()){
                        auto it_q=adj[q].begin();
                        //cout<<"2"<<endl;
                        for(auto it = G_input->adj[q].begin(); it!= G_input->adj[q].end();it++){
                            if(it->vertex==it_q->vertex){
                                it->k=min_k;
                                break;
                            }
                        }
                        
                        for(auto it = G_input->adj[it_q->vertex].begin(); it!= G_input->adj[it_q->vertex].end();it++){
                            if(it->vertex==q){
                                it->k=min_k;
                                break;
                            }
                        }
                        //cout<<"3"<<endl;
                        //cout<<"remove edge:( "<<q<<" , "<<it_q->vertex<<" )"<<endl;
                        removeEdge(q,it_q->vertex);
                        //cout<<"4"<<endl;
                    }

                    vector<int> remain;
                    //cout<<"5"<<endl;
                    for(int i=0; i<V.size(); i++){ //vertex q neighbor
                        //cout<<"Vertex "<<V[i]<<" ";
                        vector<int> C =tau_hop_neighbor(V[i],tau);
                        if((C.size()+1)<=min_k)
                            Q2.push(V[i]);
                        else
                            remain.push_back(V[i]);
                    }
                    
                
                    //test ?
                    for(int i=0;i<remain.size();i++){
                        for(int j=0;j<remain.size();j++){
                            if(remain[i]<remain[j]){
                                for(auto it=adj[remain[i]].begin();it!=adj[remain[i]].end();it++)
                                    if(it->lowerBound_k<=min_k)
                                        compute_edge_support(remain[i],it->vertex);
                            }
                        }
                    }

                }
                
                //cout<<endl;
                return true;
            }else
                return false;
            
            
        }
    
        bool unchange_support(int s,int t,int u,int v){
            int s_to_u,s_to_v,t_to_u,t_to_v;
            int s_to_u_add,s_to_v_add,t_to_u_add,t_to_v_add;
            distance_node(s,u,v,&s_to_u,&s_to_v);
            distance_node(t,u,v,&t_to_u,&t_to_v);
            //add removed edge
            addEdge(u,v,0,0);
            // cout<<"--------------add edge:( "<<u<<","<<v<<" )------------------"<<endl;
            distance_node(s,u,v,&s_to_u_add,&s_to_v_add);
            distance_node(t,u,v,&t_to_u_add,&t_to_v_add);
            removeEdge(u,v);
            // cout<<"--------------remove edge:( "<<u<<","<<v<<" )------------------"<<endl;
            if(s_to_u_add==s_to_u&&s_to_v_add==s_to_v&&t_to_u_add==t_to_u&&t_to_v_add==t_to_v)
                return true;
            return false;
        }
    
        void upper_bound_compute(int u,int v){
            int l=0;
            int r=0;
            int mid=0;
            int temp_upper_bound=l;
            set<int> mnn;
            vector<int> Q1, Q2;
            for(auto it=this->adj[u].begin();it!=this->adj[u].end();it++){
                if(it->vertex==v){
                    r=it->sup;
                    break;
                }
            }
            Q1=tau_hop_neighbor(u,tau); //u
            Q2=tau_hop_neighbor(v,tau); //v
            //find MNN
            for(int i=0; i<Q1.size(); i++)
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j])
                        mnn.insert(Q1[i]);
            
            //binary search more tied upper bound      
            while(l<=r){
                int number_V=mnn.size();  // |V(G'')| no u & v
                mid=(l+r)>>1;

                if(Q1.size()<Q2.size()){
                    for(auto it=adj[u].begin();it!=adj[u].end();it++)
                        if(mnn.find(it->vertex)!=mnn.end()&& it->sup<mid)
                            number_V--;
                }
                else{
                    for(auto it=adj[v].begin();it!=adj[v].end();it++)
                        if(mnn.find(it->vertex)!=mnn.end()&& it->sup<mid)
                            number_V--;
                }
            
                if(number_V<mid)
                    r=mid-1;
                else
                    temp_upper_bound=mid;l=mid+1;
            
            }
            //insert upper bound in e(u,v) 
            for(auto it = adj[u].begin(); it != adj[u].end(); it++)
                if(it->vertex==v){
                    it->upperBound_k=temp_upper_bound+2;
                    break;
            }

            for(auto it = adj[v].begin(); it != adj[v].end(); it++)
                if(it->vertex==u){
                    it->upperBound_k=temp_upper_bound+2;
                    break;
            }

            return;
        }

        void all_upper_bound_compute(int *max_upper_bound){
            for (int i=0;i< adj.size();i++) {
                for(auto it = adj[i].begin(); it != adj[i].end(); it++)
                    if(i < it->vertex){
                        upper_bound_compute(i,it->vertex);
                        if(it->upperBound_k > *max_upper_bound)
                            *max_upper_bound=it->upperBound_k;
                    }
                            
            }
            return;
        }
    private:
        vector<int> tau_hop_neighbor(int v,int tau1){
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
                
                if(distances[node]<tau1){
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

        void low_bound_compute(int u,int v){
            int max_low=0;
            vector<int> Q1, Q2;

            Q1=tau_hop_neighbor(u,(tau>>1));
            Q2=tau_hop_neighbor(v,(tau>>1));
            
            // cout<<"\n u neighbors:";
            // for(int i=0; i<Q1.size(); i++)
            //     cout<<Q1[i]<<" ";
            // cout<<"\n v neighbors:";
            // for(int i=0; i<Q2.size(); i++)
            //     cout<<Q2[i]<<" ";
            // cout<<endl;

            if((tau&1)==0){ //even
                int omega_u=Q1.size()+1;
                int omega_v=Q2.size()+1;
                max_low=max(omega_u,omega_v);
            }else{          //odd
                set<int> S1;
                //cout<<" S1 set: ";
                for(int i=0;i<Q1.size();i++)
                    S1.insert(Q1[i]);
                for(int i=0;i<Q2.size();i++)
                    S1.insert(Q2[i]);
                
                // for(auto i=S1.begin(); i!=S1.end(); i++)
                //     cout<<*i<<" ";

                cout<<endl;
                max_low=max(max_low, int(S1.size()) );
            }
            
            

            for(int i=0; i<Q1.size(); i++)
                for(int j=0; j<Q2.size(); j++)
                    if(Q1[i]==Q2[j]){
                        vector<int> Q3;
                        Q3=tau_hop_neighbor(Q1[i],(tau>>1));
                        // cout<<Q1[i]<<"MNN's neighbor: ";
                        // for(int c=0 ;c<int(Q3.size()); c++)
                        //     cout<<Q3[c]<<" ";
                        // cout<<endl;
                        max_low=max(max_low, int(Q3.size()+1)); //Q3.size()+"1" : plus itself
                    }
                
            // cout<<"\n max_low: "<<max_low<<endl;

            // e(u,v) sup
            for(auto it = adj[v].begin(); it != adj[v].end(); it++)
                if(it->vertex==u){
                    it->lowerBound_k=max_low;
                    break;
                }
                    

            // e(v,u) sup
            for(auto it = adj[u].begin(); it != adj[u].end(); it++)
                if(it->vertex==v){
                    it->lowerBound_k=max_low;
                    break;
                }
                    

            return;
        }
        
        void distance_node(int s,int u, int v,int *dis_to_v,int *dis_to_u){
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
        
            *dis_to_u = (distances[u])?distances[u]:-1;
            *dis_to_v = (distances[v])?distances[v]:-1;
            return;
        }

};


#endif