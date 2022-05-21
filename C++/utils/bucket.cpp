#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#pragma once
using namespace std;

class Bucket{
private:
    // 储存结点信息的桶，每个桶中存有一个哈希表，哈希表的key为结点编号，value为最短路，前驱结点等信息
    vector<unordered_map<int,vector<int>>> _buckets; 
    // 记录结点在桶中的指针
    unordered_map<int,int> _ptr; 
    // 桶的容量
    int _capacity;
    // 第一个非空桶的指针
    int min_index;
    // 桶中元素个数
    int _n;

public:
    // 默认构造函数
    Bucket();
    // 构造函数
    Bucket(int capacity, int n);

    // 推入新的元素
    void push(vector<int> &value);

    // 弹出堆顶元素
    vector<int> pop(void);

    // 更新堆中的元素
    void update(int node, vector<int> &value);

    // 查找结点在堆中的位置，等价于查找到了距离
    int query_node(int node);
    // 返回桶的大小
    int size(void);
};

Bucket::Bucket(){
    this->_capacity = 0;
    this->_n = 0;
    this->min_index = -1;
}

Bucket::Bucket(int capacity, int n){
    this->_buckets = vector<unordered_map<int,vector<int>>>(n*capacity+1);
    this->_capacity = n*capacity + 1;
    this->min_index = 0;
    this->_n = 0;
}

void Bucket::push(vector<int> &value){
    this->_buckets[value[1]].emplace(value[0],value);
    this->_n++;
    this->_ptr.emplace(value[0],value[1]);
    if(value[1] < this->min_index) this->min_index = value[1];
}

vector<int> Bucket::pop(void){
    if(this->_buckets[min_index].empty()){
        this->min_index = this->_capacity;
        for(int i=0;i<this->_capacity;++i){
            if(!this->_buckets[i].empty()){
                this->min_index = i;
                break;
            }
        }
    }
    auto it = this->_buckets[min_index].begin();
    vector<int> value = it->second;
    this->_buckets[min_index].erase(it->first);
    this->_n--;
    // 更新min_index
    if(this->_n == 0){
        this->min_index = this->_capacity;
    }else{
        for(int i=min_index;i<this->_capacity;++i){
            if(!this->_buckets[i].empty()){
                this->min_index = i;
                break;
            }
        }
    }
    return value;
}

void Bucket::update(int node, vector<int> &value){
    int index = this->query_node(node);
    if(value[1] < index){
        this->_buckets[value[1]].emplace(value[0],value);
        this->_buckets[index].erase(value[0]);
        this->_ptr[node] = value[1];
        if(value[1] < this->min_index) this->min_index = value[1];
    }
}

int Bucket::query_node(int node){
    return this->_ptr[node];
}

int Bucket::size(void){
    return this->_n;
}