#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

int main(){
    string in[3]={"LJ","dblp","orkut"};
    string out[3]={"LJ","dblp","orkut"};
    
    for(int i=0; i<3; i++){
        ifstream myfile;
        myfile.open("../dataset/"+in[i]+".txt");
        string str;
        
        if(!myfile.is_open()){
            cout<<"Error";
            return 0;
        }
            
        
        // Read and write the first two lines
        ofstream outfile("../dataset/d/"+out[i]+".txt");

        // Construct the node map
        while(getline(myfile, str)){
            stringstream ss(str);
            int node1, node2;
            ss >> node1>>node2;

            while(ss){  
                outfile<<node1<<"\t"<<node2<<endl;
                ss >>node2;
            }
        }

       
        //cout<<"3\n";

        myfile.close();
        //outfile.close();
    }
return 0;
}
