#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

int main(int argc, char **argv){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::ifstream infile2(std::string(argv[1]) + ".graph");
    int V,E,k1,k2;
    infile2 >> V >> E >> k1 >> k2;
    std::ifstream infile(std::string(argv[1]) + ".satoutput");
    std::string sat;
    infile >> sat;
    if(sat=="UNSAT"){
        std::ofstream outfile(std::string(argv[1])+".mapping");
        outfile<<"0";
        return 0;
    }
    else{
        std::vector<int> chosen1;
        std::vector<int> chosen2;
        for(int i=0;i<V;i++){
            int x;
            infile >> x;
            if(x>0){
                chosen1.push_back(i+1);
            }
        }
        for(int i=0;i<V;i++){
            int x;
            infile >> x;
            if(x>0){
                chosen2.push_back(i+1);
            }
        }
        std::ofstream outfile(std::string(argv[1])+".mapping");
        outfile<<"#1"<<"\n";
        for(int i=0;i<k1-1;i++){
            outfile<<chosen1[i]<<" ";
        }
        outfile<<chosen1[k1-1]<<"\n";
        outfile<<"#2"<<"\n";
        for(int i=0;i<k2-1;i++){
            outfile<<chosen2[i]<<" ";
        }
        outfile<<chosen2[k2-1];
    }
return 0;
}