#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#include "module.cpp"
#pragma once
using namespace std;

// string pbar1 = "██████████████████████████████████████████████████"; // 50个bar
string pbar1 = ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"; // 50个bar
string pbar2 = "                                                  "; // 50个bar
string pbar3 = "--------------------------------------------------------------------------------------";

int process_size = pbar1.size();

vector<vector<int>> Dijkstra(vector<vector<int>> &OD_pair, vector<int> size, string method, string type,
                             unordered_map<int,vector<pair<int,int>>> &graph_forward){
    int total = OD_pair.size();
    string process1, process2;
    int num = 0;
    if(method == "auto"){
        if(type == "random") method = "heap";
        else if(type == "dense") method = "queue";
        else if(type == "grid") method = "queue";
    }
    if(method == "queue"){
        cout<<"Using "<< "\033[33;1mqueue-based Dijkstra \033[0m" <<"algorithm..."<<endl;
    }else if(method == "heap"){
        cout<<"Using "<< "\033[33;1mheap-based Dijkstra \033[0m" <<"algorithm..."<<endl;
    }else if(method == "bucket"){
        cout<<"Using "<< "\033[33;1mbucket-based Dijkstra \033[0m" <<"algorithm..."<<endl;
    }
    cout<<pbar3<<endl;
    for(auto &query : OD_pair){
        cout<<"\r";
        if(method == "heap"){
            query.emplace_back(heap_method(query[0],query[1],size[0],graph_forward));
        }else if(method == "queue"){
            query.emplace_back(queue_method(query[0],query[1],size[0],graph_forward));
        }else if(method == "bucket"){
            query.emplace_back(bucket_method(query[0],query[1],size[0],size[2],graph_forward));
        }
        num++;
        printf("Query from %5d to %5d | ", query[0], query[1]);
        process1 = pbar1.substr(0, 1*(int)(50 * num / total));
        process2 = pbar2.substr(1*(int)(50 * num / total));
        cout<<process1<<process2;
        printf(" %.2f%s",(double)(100*((double)(num) / total)),"%");
    }
    printf("\n");
    return OD_pair;
}

vector<vector<int>> BiDijkstra(vector<vector<int>> &OD_pair, vector<int> size, string method, string type,
                               unordered_map<int,vector<pair<int,int>>> &graph_forward,
                               unordered_map<int,vector<pair<int,int>>> &graph_backward){
    int total = OD_pair.size();
    string process1, process2;
    int num = 0;
    if(method == "auto"){
        if(type == "random") method = "queue";
        else if(type == "dense") method = "queue";
        else if(type == "grid") method = "queue";
    }
    if(method == "queue"){
        cout<<"Using "<< "\033[33;1mqueue-based Bidirectional Dijkstra \033[0m" <<"algorithm..."<<endl;
    }else if(method == "heap"){
        cout<<"Using "<< "\033[33;1mheap-based Bidirectional Dijkstra \033[0m" <<"algorithm..."<<endl;
    }else if(method == "bucket"){
        cout<<"Using "<< "\033[33;1mbucket-based Bidirectional Dijkstra \033[0m" <<"algorithm..."<<endl;
    }

    cout<<pbar3<<endl;
    for(auto &query : OD_pair){
        cout<<"\r";
        if(method == "heap"){
            query.push_back(heap_bidirectional(query[0],query[1],size[0],graph_forward,graph_backward));
        }else if(method == "queue"){
            query.emplace_back(queue_bidirectional(query[0],query[1],size[0],graph_forward,graph_backward));
        }else if(method == "bucket"){
            query.emplace_back(bucket_bidirectional(query[0],query[1],size[0],size[2],graph_forward,graph_backward));
        }
        num++;
        printf("Query from %5d to %5d | ", query[0], query[1]);
        process1 = pbar1.substr(0,1*(int)(50 * num / total));
        process2 = pbar2.substr(1*(int)(50 * num / total));
        cout<<process1<<process2;
        printf(" %.2f%s",(double)(100*((double)(num) / total)),"%");
    }
    printf("\n");
    return OD_pair;
}