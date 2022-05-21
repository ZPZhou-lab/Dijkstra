#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#pragma once
using namespace std;

class Heap{
private:
    // 存放结点在堆中位置的指针
    vector<int> _ptr; // 保存node的排序
    // 记录结点信息
    unordered_map<int,vector<int>> _dist; // key保存node，value保存[结点, 距离, 前驱结点, 结点在堆中的键key]
    // 堆的大小
    int _size;
    
public:
    // 默认构造函数
    Heap();
    // 构造函数
    Heap(vector<vector<int>>& nodes, int n);

    // 推入新的元素
    void push(vector<int> &value);

    // 弹出堆顶元素
    vector<int> pop(void);

    // 更新堆中的元素
    void update(int node, vector<int> &value);

    // 返回堆顶元素，但不弹出
    vector<int> top(void);

    // 查找结点在堆中的位置
    int query_node(int node);
    // 查找节点当前的最短路径长度
    int query_dist(int node);

    // 堆的维护函数
    int __heapify(int key);
    void __SHIFTUP(int key); // 向上维护
    void __SHIFTDOWN(int key); // 向下维护

    // 子节点和父节点的查询
    int __LEFT(int key);
    int __RIGHT(int key);
    int __PARENT(int key);

    // 交换堆中两个元素的位置
    void swap(int key1, int key2);

    // 返回堆的大小
    int size(void);
};

Heap::Heap(){
    this->_size = 0;
}

Heap::Heap(vector<vector<int>> &nodes, int n){
    this->_size = nodes.size();
    this->_ptr = vector<int>(n,0);
    for(int i=0;i<this->_size;++i){
        nodes[i][3] = i+1;
        this->_dist[nodes[i][0]] = nodes[i];
        this->_ptr[i] = nodes[i][0];
    }
    // 建堆，复杂度O(nlogn)
    for(int i=(int)this->_size / 2;i>0;i--){
        // 调用堆函数不断维护堆的性质
        this->__heapify(i);
    }
}

int Heap::__heapify(int key){
    int l = this->__LEFT(key);
    int r = this->__RIGHT(key);

    // 找到最小根节点
    int smallest = key;
    if(l - 1 < this->_size && this->_dist[_ptr[l - 1]][1] < this->_dist[_ptr[key - 1]][1]) smallest = l;
    if(r - 1 < this->_size && this->_dist[_ptr[r - 1]][1] < this->_dist[_ptr[smallest - 1]][1]) smallest = r;
    // 如果当前位置不是最小值，则进行维护
    if(smallest != key){
        this->swap(key,smallest);
        // 递归的进行维护
        this->__heapify(smallest);
    }
    return smallest;
}

void Heap::swap(int key1, int key2){
    std::swap(this->_ptr[key1 - 1],this->_ptr[key2 - 1]);
    this->_dist[this->_ptr[key1 - 1]][3] = key1;
    this->_dist[this->_ptr[key2 - 1]][3] = key2;
}

int Heap::size(void){
    return this->_size;
}

void Heap::push(vector<int> &value){
    this->_ptr[this->_size] = value[0];
    this->_size++;
    value[3] = this->_size;
    this->_dist[value[0]] = value;
    // 向上维护堆
    this->__SHIFTUP(this->_size);
}

vector<int> Heap::top(void){
    vector<int> _top;
    if(this->_size > 0){
        _top = this->_dist[_ptr[0]];
    }
    return _top;
}

vector<int> Heap::pop(void){
    vector<int> node = this->top();
    this->swap(1,this->_size);
    this->_size--;
    // 向下维护堆
    this->__SHIFTDOWN(1);
    // 记录持久化信息
    this->_dist[node[0]][3] = 0;
    return node;
}

void Heap::update(int node, vector<int> &value){
    // 当前距离更小才进行更新
    if(value[1] < this->_dist[node][1]){
        int key = this->query_node(node);
        value[3] = key;
        this->_dist[_ptr[key - 1]] = value;
        // 向上维护堆
        this->__SHIFTUP(key);
    }
}

int Heap::query_node(int node){
    if(this->_dist.find(node) == this->_dist.end()){
        throw "node imformation is not in the heap!";
    }
    return this->_dist[node][3];
}

int Heap::query_dist(int node){
    return this->_dist[node][1];
}

int Heap::__LEFT(int key){
    return (key << 1);
}

int Heap::__RIGHT(int key){
    return (key << 1) + 1;
}

int Heap::__PARENT(int key){
    return (key >> 1);
}

void Heap::__SHIFTUP(int key){
    int parent = this->__PARENT(key);
    while(parent){
        this->__heapify(parent);
        parent = this->__PARENT(parent);
    }
}

void Heap::__SHIFTDOWN(int key){
    int pre = 0;
    int cur = key;
    while(pre != cur){
        pre = cur;
        cur = this->__heapify(cur);
    }
}