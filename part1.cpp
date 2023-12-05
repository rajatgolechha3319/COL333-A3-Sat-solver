#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

// add an offset of 2 in variables
// make some functions in the code

int main(int argc, char **argv){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::string constraints="";
    int num_vars = 0;
    int num_clauses = 0;
    std::ifstream infile(std::string(argv[1]) + ".graph");
    int V,E,k1,k2;
    infile >> V >> E >> k1 >> k2;
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
    num_vars += 2*V;
    for(int i=0; i < V; i++){
        constraints += "-" + std::to_string(i+1) + " -" + std::to_string(i+1+V) + " 0\n";
        num_clauses++;
        if(degree[i]<k1-1){
            constraints += "-" + std::to_string(i+1) + " 0\n";
            num_clauses++;
        }
        if(degree[i]<k2-1){
            constraints += "-" + std::to_string(i+1+V) + " 0\n";
            num_clauses++;
        }
        for(int j=i+1;j<V;j++){
            if(adj[i][j] == 0){
                constraints += "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                constraints += "-" + std::to_string(i+1+V) + " -" + std::to_string(j+1+V) + " 0\n";
                num_clauses+=2;
            }
            else{
                int x1 = degree[i];
                int x2 = degree[j];
                if((x1 < k1 -1) ||(x2 < k1-1) ){
                    constraints += "-" + std::to_string(i+1) + " -" + std::to_string(j+1) + " 0\n";
                    num_clauses++;
                }
                if((x1 < k2 -1) ||(x2 < k2-1) ){
                    constraints += "-" + std::to_string(i+1+V) + " -" + std::to_string(j+1+V) + " 0\n";
                    num_clauses++;
                }
            }
        }
    }
    num_vars += (k1+1)*(V+1);
    for(int i=0;i<=V;i++){
        constraints += std::to_string(2*V+i+1) + " 0\n";
    }
    num_clauses += V+1;
    for(int i=1;i<=k1;i++){
        constraints += "-" + std::to_string((2*V+1)+(V+1)*i) + " 0\n";
    }
    num_clauses += k1;
    for(int j=1;j<=k1;j++){
        for(int i=1;i<=V;i++){
                std::string a,b,c,d;
                a = std::to_string(2*V+1+i+(V+1)*j);
                b = std::to_string(2*V+1+i-1+(V+1)*j);
                c = std::to_string(i);
                d = std::to_string(2*V+1+i-1+(V+1)*(j-1));
                constraints += "-"+b+" "+a+" 0\n";
                constraints += "-"+c+" -"+d+" "+a+" 0\n";
                constraints += c+" "+b+" -"+a+" 0\n";
                constraints += d+" "+b+" -"+a+" 0\n";
                num_clauses += 4;
        }
    }
    constraints += std::to_string(2*V+(V+1)*(k1+1)) + " 0\n";
    num_clauses++;
    num_vars += (k2+1)*(V+1);
    for(int i=0;i<=V;i++){
        constraints += std::to_string(2*V+(k1+1)*(V+1)+i+1) + " 0\n";
    }
    num_clauses += V+1;
    for(int i=1;i<=k2;i++){
        constraints += "-" + std::to_string(2*V+(k1+1)*(V+1)+1+(V+1)*i) + " 0\n";
    }
    num_clauses += k2;
    for(int j=1;j<=k2;j++){
        for(int i=1;i<=V;i++){
                std::string a,b,c,d;
                a = std::to_string(2*V+(k1+1)*(V+1)+1+i+(V+1)*j);
                b = std::to_string(2*V+(k1+1)*(V+1)+1+i-1+(V+1)*j);
                c = std::to_string(i+V);
                d = std::to_string(2*V+(k1+1)*(V+1)+1+i-1+(V+1)*(j-1));
                constraints += "-"+b+" "+a+" 0\n";
                constraints += "-"+c+" -"+d+" "+a+" 0\n";
                constraints += c+" "+b+" -"+a+" 0\n";
                constraints += d+" "+b+" -"+a+" 0\n";
                num_clauses += 4;
        }
    }
    constraints += std::to_string(2*V+(k1+1)*(V+1)+(V+1)*(k2+1)) + " 0\n";
    num_clauses++;
    std::ofstream outfile(std::string(argv[1]) + ".satinput");
    outfile << "p cnf " << num_vars << " " << num_clauses << "\n";
    outfile << constraints;
    return 0;
}