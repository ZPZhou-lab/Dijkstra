#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#include "MyHeap.cpp"
#include "bucket.cpp"
#pragma once
using namespace std;

// 单向堆Dijkstra
int heap_method(int start, int end, int n,
                unordered_map<int,vector<pair<int,int>>> &graph_forward){
    vector<vector<int>> init;
    vector<int> value = {start,0,start,0};
    init.emplace_back(value);
    // 创建堆
    Heap heap = Heap(init,n);
    // 两个集合T, P
    unordered_set<int> T;
    T.insert(start);
    unordered_set<int> P;
    while(heap.size()){
        // 弹出最小值
        vector<int> pop_node = heap.pop();
        P.insert(pop_node[0]);
        T.erase(pop_node[0]);
        // 判断是否找到终点
        if(pop_node[0] == end){
            return pop_node[1];
        }else{
            for(auto edge : graph_forward[pop_node[0]]){
                int to_node = edge.first;
                int dist = pop_node[1] + edge.second;
                if(P.find(to_node) == P.end()){
                    value = {to_node,dist,pop_node[0],0};
                    if(T.find(to_node) == T.end()){
                        T.insert(to_node);
                        heap.push(value);
                    }else{
                        heap.update(to_node,value);
                    }
                }
            }
        }
    }
    return INT_MAX;
}

// 双向堆Dijkstra
int heap_bidirectional(int start, int end, int n,
                       unordered_map<int,vector<pair<int,int>>> &graph_forward,
                       unordered_map<int,vector<pair<int,int>>> &graph_backward){
    vector<vector<int>> init;
    // 创建堆
    vector<int> value = {start,0,start,0};
    init.emplace_back(value);
    Heap heap_forward = Heap(init,n);
    init.pop_back();
    value = {end,0,end,0};
    init.emplace_back(value);
    Heap heap_backward = Heap(init,n);

    // 两个集合T, P
    unordered_set<int> T_forward;
    T_forward.insert(start);
    unordered_set<int> P_forward;
    unordered_set<int> T_backward;
    T_backward.insert(end);
    unordered_set<int> P_backward;

    int min_dist = INT_MAX;
    while(heap_forward.size() && heap_backward.size()){
        // 弹出最小值
        vector<int> pop_node_f = heap_forward.pop();
        vector<int> pop_node_b = heap_backward.pop();
        // 停止条件
        if(pop_node_f[1] + pop_node_b[1] >= min_dist){
            break;
        }

        // 前向更新
        P_forward.insert(pop_node_f[0]);
        T_forward.erase(pop_node_f[0]);
        // 判断是否找到终点
        if(pop_node_f[0] == end){
            return pop_node_f[1];
        }else{
            for(auto edge : graph_forward[pop_node_f[0]]){
                int to_node = edge.first;
                int dist = pop_node_f[1] + edge.second;
                if(P_forward.find(to_node) == P_forward.end()){
                    value = {to_node,dist,pop_node_f[0],0};
                    if(T_forward.find(to_node) == T_forward.end()){
                        T_forward.insert(to_node);
                        heap_forward.push(value);
                    }else{
                        heap_forward.update(to_node,value);
                    }
                }
                if(P_backward.find(to_node) != P_backward.end()){
                    int update_dist = heap_forward.query_dist(to_node) + heap_backward.query_dist(to_node);
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }

        // 后向更新
        P_backward.insert(pop_node_b[0]);
        T_backward.erase(pop_node_b[0]);
        // 判断是否找到终点
        if(pop_node_b[0] == start){
            return pop_node_b[1];
        }else{
            for(auto edge : graph_backward[pop_node_b[0]]){
                int from_node = edge.first;
                int dist = pop_node_b[1] + edge.second;
                if(P_backward.find(from_node) == P_backward.end()){
                    value = {from_node,dist,pop_node_b[0],0};
                    if(T_backward.find(from_node) == T_backward.end()){
                        T_backward.insert(from_node);
                        heap_backward.push(value);
                    }else{
                        heap_backward.update(from_node,value);
                    }
                }
                if(P_forward.find(from_node) != P_forward.end()){
                    int update_dist = heap_forward.query_dist(from_node) + heap_backward.query_dist(from_node);
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }
    }
    return min_dist;
}

int queue_method(int start, int end, int n,
                 unordered_map<int,vector<pair<int,int>>> &graph_forward){
    vector<int> Q = {start};
    // 两个集合T, P
    unordered_set<int> T;
    T.insert(start);
    unordered_set<int> P;
    // 存在最小值的指针
    unordered_map<int, pair<int,int>> L;
    L[start] = make_pair(start,0);
    int min_dist;
    int pop;
    int pop_node;
    while(Q.size()){
        min_dist = INT_MAX;
        for(int i=0;i<Q.size();++i){
            if(L[Q[i]].second < min_dist){
                min_dist = L[Q[i]].second;
                pop = i;
            }
        }
        pop_node = Q[pop];
        Q.erase(Q.begin() + pop);
        P.insert(pop_node);
        T.erase(pop_node);

        if(pop_node == end){
            return L[pop_node].second;
        }else{
            for(auto edge : graph_forward[pop_node]){
                int to_node = edge.first;
                int dist = L[pop_node].second + edge.second;
                if(P.find(to_node) == P.end()){
                    if(T.find(to_node) == T.end()){
                        T.insert(to_node);
                        Q.emplace_back(to_node);
                        L[to_node] = make_pair(pop_node,dist);
                    }else{
                        if(dist < L[to_node].second) L[to_node] = make_pair(pop_node,dist);
                    }
                }
            }
        }
    }
    return INT_MAX;
}

int queue_bidirectional(int start, int end, int n,
                        unordered_map<int,vector<pair<int,int>>> &graph_forward,
                        unordered_map<int,vector<pair<int,int>>> &graph_backward){
    vector<int> Q_forward = {start};
    vector<int> Q_backward = {end};
    // 两个集合T, P
    unordered_set<int> T_forward;
    T_forward.insert(start);
    unordered_set<int> P_forward;
    unordered_set<int> T_backward;
    T_backward.insert(end);
    unordered_set<int> P_backward;
    // 存在最小值的指针
    unordered_map<int, pair<int,int>> L_forward;
    unordered_map<int, pair<int,int>> L_backward;
    L_forward[start] = make_pair(start,0);
    L_backward[end] = make_pair(end,0);
    int min_dist = INT_MAX;
    int dist;
    int pop;
    int pop_node_f;
    int pop_node_b;
    while(Q_forward.size() && Q_backward.size()){
        dist = INT_MAX;
        for(int i=0;i<Q_forward.size();++i){
            if(L_forward[Q_forward[i]].second < dist){
                dist = L_forward[Q_forward[i]].second;
                pop = i;
            }
        }
        pop_node_f = Q_forward[pop];
        Q_forward.erase(Q_forward.begin() + pop);

        dist = INT_MAX;
        for(int i=0;i<Q_backward.size();++i){
            if(L_backward[Q_backward[i]].second < dist){
                dist = L_backward[Q_backward[i]].second;
                pop = i;
            }
        }
        pop_node_b = Q_backward[pop];
        Q_backward.erase(Q_backward.begin() + pop);
    
        // 停止条件
        if(L_forward[pop_node_f].second + L_backward[pop_node_b].second >= min_dist){
            break;
        }

        // 前向更新
        P_forward.insert(pop_node_f);
        T_forward.erase(pop_node_f);

        if(pop_node_f == end){
            return L_forward[pop_node_f].second;
        }else{
            for(auto edge : graph_forward[pop_node_f]){
                int to_node = edge.first;
                int dist = L_forward[pop_node_f].second + edge.second;
                if(P_forward.find(to_node) == P_forward.end()){
                    if(T_forward.find(to_node) == T_forward.end()){
                        T_forward.insert(to_node);
                        Q_forward.emplace_back(to_node);
                        L_forward[to_node] = make_pair(pop_node_f,dist);
                    }else{
                        if(dist < L_forward[to_node].second) L_forward[to_node] = make_pair(pop_node_f,dist);
                    }
                }
                if(P_backward.find(to_node) != P_backward.end()){
                    int update_dist = L_forward[to_node].second + L_backward[to_node].second;
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }

        // 后向更新
        P_backward.insert(pop_node_b);
        T_backward.erase(pop_node_b);

        if(pop_node_b == start){
            return L_backward[pop_node_b].second;
        }else{
            for(auto edge : graph_backward[pop_node_b]){
                int from_node = edge.first;
                int dist = L_backward[pop_node_b].second + edge.second;
                if(P_backward.find(from_node) == P_backward.end()){
                    if(T_backward.find(from_node) == T_backward.end()){
                        T_backward.insert(from_node);
                        Q_backward.emplace_back(from_node);
                        L_backward[from_node] = make_pair(pop_node_b,dist);
                    }else{
                        if(dist < L_backward[from_node].second) L_backward[from_node] = make_pair(pop_node_b,dist);
                    }
                }
                if(P_forward.find(from_node) != P_forward.end()){
                    int update_dist = L_forward[from_node].second + L_backward[from_node].second;
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }
    }
    return min_dist;
}

// 单向桶Dijkstra
int bucket_method(int start, int end, int n, int max_dist,
                  unordered_map<int,vector<pair<int,int>>> &graph_forward){
    vector<int> value = {start,0,start};
    // 创建桶
    Bucket bucket = Bucket(max_dist,n);
    bucket.push(value);
    // 两个集合T, P
    unordered_set<int> T;
    T.insert(start);
    unordered_set<int> P;
    while(bucket.size()){
        // 弹出最小值
        vector<int> pop_node = bucket.pop();
        P.insert(pop_node[0]);
        T.erase(pop_node[0]);
        // 判断是否找到终点
        if(pop_node[0] == end){
            return pop_node[1];
        }else{
            for(auto edge : graph_forward[pop_node[0]]){
                int to_node = edge.first;
                int dist = pop_node[1] + edge.second;
                if(P.find(to_node) == P.end()){
                    value = {to_node,dist,pop_node[0]};
                    if(T.find(to_node) == T.end()){
                        T.insert(to_node);
                        bucket.push(value);
                    }else{
                        bucket.update(to_node,value);
                    }
                }
            }
        }
    }
    return INT_MAX;
}


// 双向桶Dijkstra
int bucket_bidirectional(int start, int end, int n, int max_dist,
                         unordered_map<int,vector<pair<int,int>>> &graph_forward,
                         unordered_map<int,vector<pair<int,int>>> &graph_backward){
    // 创建桶
    vector<int> value = {start,0,start};
    Bucket bucket_forward = Bucket(max_dist,n);
    bucket_forward.push(value);
    value = {end,0,end};
    Bucket bucket_backward = Bucket(max_dist,n);
    bucket_backward.push(value);

    // 两个集合T, P
    unordered_set<int> T_forward;
    T_forward.insert(start);
    unordered_set<int> P_forward;
    unordered_set<int> T_backward;
    T_backward.insert(end);
    unordered_set<int> P_backward;

    int min_dist = INT_MAX;
    while(bucket_forward.size() && bucket_backward.size()){
        // 弹出最小值
        vector<int> pop_node_f = bucket_forward.pop();
        vector<int> pop_node_b = bucket_backward.pop();
        // 停止条件
        if(pop_node_f[1] + pop_node_b[1] >= min_dist){
            break;
        }

        // 前向更新
        P_forward.insert(pop_node_f[0]);
        T_forward.erase(pop_node_f[0]);
        // 判断是否找到终点
        if(pop_node_f[0] == end){
            return pop_node_f[1];
        }else{
            for(auto edge : graph_forward[pop_node_f[0]]){
                int to_node = edge.first;
                int dist = pop_node_f[1] + edge.second;
                if(P_forward.find(to_node) == P_forward.end()){
                    value = {to_node,dist,pop_node_f[0]};
                    if(T_forward.find(to_node) == T_forward.end()){
                        T_forward.insert(to_node);
                        bucket_forward.push(value);
                    }else{
                        bucket_forward.update(to_node,value);
                    }
                }
                if(P_backward.find(to_node) != P_backward.end()){
                    int update_dist = bucket_forward.query_node(to_node) + bucket_backward.query_node(to_node);
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }

        // 后向更新
        P_backward.insert(pop_node_b[0]);
        T_backward.erase(pop_node_b[0]);
        // 判断是否找到终点
        if(pop_node_b[0] == start){
            return pop_node_b[1];
        }else{
            for(auto edge : graph_backward[pop_node_b[0]]){
                int from_node = edge.first;
                int dist = pop_node_b[1] + edge.second;
                if(P_backward.find(from_node) == P_backward.end()){
                    value = {from_node,dist,pop_node_b[0]};
                    if(T_backward.find(from_node) == T_backward.end()){
                        T_backward.insert(from_node);
                        bucket_backward.push(value);
                    }else{
                        bucket_backward.update(from_node,value);
                    }
                }
                if(P_forward.find(from_node) != P_forward.end()){
                    int update_dist = bucket_forward.query_node(from_node) + bucket_backward.query_node(from_node);
                    if(update_dist < min_dist) min_dist = update_dist;
                }
            }
        }
    }
    return min_dist;
}