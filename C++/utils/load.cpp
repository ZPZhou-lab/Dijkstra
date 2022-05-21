#include<fstream>
#include<bits/stdc++.h>
#include<vector>
#include<string>
#include<cstdio>
using namespace std;

vector<int> read_graph(const string &path, 
                       unordered_map<int,vector<pair<int,int>>> &graph_forward,
                       unordered_map<int,vector<pair<int,int>>> &graph_backward)
{
	ifstream infile;
    infile.open(path.c_str()); //读取文件
	string strtmp; //temp
    vector<int> size;
    pair<int,int> p;
    int max_dist = 0;
	// 文件成功打开
	if(!infile){
		cout<<"Fail to open file!"<<endl;
		exit(1);
	}
    int i = 0;
    while(getline(infile, strtmp)){
        stringstream linestr(strtmp);
        string str;
        if(i == 0){
            size.emplace_back(atoi(strtmp.substr(2).c_str()));
        }else if(i == 1){
            size.emplace_back(atoi(strtmp.substr(2).c_str()));
        }
        else if(i >= 3){
            vector<int> lineArray;
            while(getline(linestr,str,',')){
                lineArray.push_back(atoi(str.c_str()));
            }
            p = make_pair(lineArray[1],lineArray[2]);
            graph_forward[lineArray[0]].emplace_back(p);
            p = make_pair(lineArray[0],lineArray[2]);
            graph_backward[lineArray[1]].emplace_back(p);
            if(lineArray[2] > max_dist) max_dist = lineArray[2];
        }
        i++;
    }   
    infile.close();
    size.emplace_back(max_dist);
    return size;
}

void read_OD(const string &path, vector<vector<int>> &OD)
{
	ifstream infile;
    infile.open(path.c_str()); //读取文件
	string strtmp; //temp
	// 文件成功打开
	if(!infile){
		cout<<"Fail to open file!"<<endl;
		exit(1);
	}
    int i = 0;
    while(getline(infile, strtmp)){
        stringstream linestr(strtmp);
        string str;
        if(i >= 1){
            vector<int> lineArray;
            while(getline(linestr,str,',')){
                lineArray.push_back(atoi(str.c_str()));
            }
            OD.emplace_back(lineArray);
        }
        i++;
    }   
    infile.close();
    return;
}

bool isFileExist(string &path){
    ifstream f(path.c_str());
    return f.good();
}

void write_OD(string uid,string type,
              vector<vector<int>> &OD_pair,vector<int> &size,
              long time_used){
    ofstream outfile;
    string file = "";
    file += uid; file += "_output_";
    file += type; file += ".txt";
    if(isFileExist(file)) remove(file.c_str());
    outfile.open(file,ios::out);
    outfile << "origin,destination,distance" << endl;
    for(auto query : OD_pair){
        outfile << query[0] << ","<< query[1] << "," << query[2] << endl;
    }
    outfile.close();

    file = "";
    file += uid; file += "_time_";
    file += type; file += ".txt";
    if(isFileExist(file)) remove(file.c_str());
    outfile.open(file,ios::out);
    outfile << "n=" << size[0] << endl;
    outfile << "m=" << size[1] << endl;
    outfile << "time=" << time_used << endl;
    outfile.close();
    return;
}