#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#include "utils/load.cpp"
#include "utils/module.cpp"
#include "utils/main.cpp"
#include <ctime>
using namespace std;

int main(int argc, char* argv[]){
    string params_error = "The number of parameters does not match";
    if(argc < 4) throw params_error.c_str();
    clock_t tic, toc;
    string uid = "21210690132";
    string type = argv[1];
    string graph_path = argv[2];
    string OD_path = argv[3];
    string method = "auto";
    if(argc == 5) method = argv[4];

    tic = clock();
    // 创建两个图
    unordered_map<int,vector<pair<int,int>>> graph_forward; // forward图
    unordered_map<int,vector<pair<int,int>>> graph_backward; // backward图
    vector<int> graph_size; // 网络规模
    graph_size = read_graph(graph_path,graph_forward,graph_backward);
    // 创建OD_pair
    vector<vector<int>> OD_pair;
    read_OD(OD_path,OD_pair);

    //OD_pair = Dijkstra(OD_pair,graph_size,method,type,graph_forward);
    OD_pair = BiDijkstra(OD_pair,graph_size,method,type,graph_forward,graph_backward);
    toc = clock();
    long time_used = (long)(1000*(toc - tic) / CLOCKS_PER_SEC);
    cout << "\033[32;1mQuery Time Used: \033[0m" << time_used << " ms" << endl;
    write_OD(uid,type,OD_pair,graph_size,time_used);

    return 0;
}