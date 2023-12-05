#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

int main(int argc, char **argv){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::string constraints="";
    int num_vars = 0;
    int num_clauses = 0;
    std::ifstream infile(std::string(argv[1]) + ".graph");
    int V,E;
    infile >> V >> E;
    std::vector<std::vector<int> > adj(V,std::vector<int>(V,0));
    for(int i = 0; i < E; i++){
        int u,v;
        infile >> u >> v;
        u--;
        v--;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }
    std::vector<int> degree(V,0);
    for(int i=0; i< V;i++){
        for(int j=0; j<V;j++){
            if(i==j){
                continue;
            }
            else{
                if(adj[i][j] == 1){
                    degree[i]++;
                }
            }
        }
    }
    num_vars += V;
    for(int i=0; i < V; i++){
        for(int j=i+1;j<V;j++){
            if(adj[i][j] == 0){
                constraints += "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                num_clauses++;
            }
        }
    }
    if(E==0){
        std::ofstream outfile(std::string(argv[1])+".mapping");
        outfile<<"0";
        return 0;
    }
    if(num_clauses==0){
        std::ofstream outfile(std::string(argv[1])+".mapping");
        outfile<<"#1"<<"\n";
        for(int i=0;i<V-1;i++){
            outfile<<i+1<<" ";
        }
        outfile<<V;
        return 0;
    }

    //implementing the binary search
    int low = 1;
    int high = V; 
    while(low <= high){
        std::string new_constraints = "";
        int new_clauses = 0;
        int new_vars = 0;
        int k = low + (high-low)/2 ;
        std::cout<<"Checking for size "<<k<<'\n';
        for(int i=0; i < V; i++){
            if(degree[i]<k-1){
            constraints += "-" + std::to_string(i+1) + " 0\n";
            num_clauses++;
            }
            for(int j=i+1;j<V;j++){
                if(adj[i][j] == 1){
                    int x1 = degree[i];
                    int x2 = degree[j];
                    if((x1 < k -1) ||(x2 < k-1) ){
                        new_constraints += "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                        new_clauses++;
                    }
                }
            }
        }
        new_vars += (k+1)*(V+1);
        for(int i=0;i<=V;i++){
            new_constraints += std::to_string(V+i+1) + " 0\n";
        }
        new_clauses += V+1;
        for(int i=1;i<=k;i++){
            new_constraints += "-" + std::to_string((V+1)+(V+1)*i) + " 0\n";
        }
        new_clauses += k;
        for(int j=1;j<=k;j++){
            for(int i=1;i<=V;i++){
                    std::string a,b,c,d;
                    a = std::to_string(V+1+i+(V+1)*j);
                    b = std::to_string(V+1+i-1+(V+1)*j);
                    c = std::to_string(i);
                    d = std::to_string(V+1+i-1+(V+1)*(j-1));
                    new_constraints += "-"+b+" "+a+" 0\n";
                    new_constraints += "-"+c+" -"+d+" "+a+" 0\n";
                    new_constraints += c+" "+b+" -"+a+" 0\n";
                    new_constraints += d+" "+b+" -"+a+" 0\n";
                    new_clauses += 4;
            }
        }
        new_constraints += std::to_string(V+(V+1)*(k+1)) + " 0\n";
        new_clauses++;
        for(int i=0;i<V;i++){
            if(degree[i]<k-1){
                new_constraints += "-" + std::to_string(i+1) + " 0\n";
                new_clauses++;
            }
        }
        std::ofstream outfile(std::string(argv[1]) + ".satinput");
        outfile << "p cnf " << num_vars+new_vars << " " << num_clauses+new_clauses << "\n";
        outfile << constraints;
        outfile << new_constraints;
        outfile.close();
        std::string cmd = "minisat "+ std::string(argv[1]) + ".satinput " + " " + std::string(argv[1]) + ".satoutput";
        std::system(cmd.c_str());
        std::ifstream infile(std::string(argv[1]) + ".satoutput");
        std::string sat;
        infile >> sat;
        if(sat=="UNSAT"){
            high = k-1;
        }
        else{
            low = k+1;
            std::vector<int> chosen;
            for(int i=0;i<V;i++){
                int x;
                infile >> x;
                if(x>0){
                    chosen.push_back(i+1);
                }
            }
            std::ofstream outfile(std::string(argv[1])+".mapping");
            outfile << "#" << 1 << "\n";
            for(int i=0;i<chosen.size()-1;i++){
                outfile << chosen[i] << " ";
            }
            outfile << chosen[chosen.size()-1];
            outfile.close();
        }
    }
   
    return 0;
}