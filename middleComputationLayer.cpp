#ifndef USERINTERFACE_CPP
#define USERINTERFACE_CPP

#include"outerDBinterface.cpp"
#include<iostream>
#include<vector>
#include<iostream>
#include<algorithm>

using namespace std;

#define buffersize 10
vector<int> buffer;


void initializeBufferwithzero(){
	int i=0;
	while(i<buffersize){
		buffer.push_back(0);
		i++;
	}
}

void doDFSTopSort(vector<Node *> lst,int start,int &curr,vector<Node *> &visited){
	
	if(lst.empty()){
		return;
	}
	
	int i=0,bufferidx=0;
	Node *n;
	
	while(i<lst.size()){
	
		n = lst[i];
		
		if(find(visited.begin(),visited.end(),n)!=visited.end()){
			visited.push_back(n);	
				
			if(start>=curr){
				buffer[bufferidx++] = n->task;
			}
			
			if(bufferidx>=buffersize)
				break;
			
			curr++;
			doDFSTopSort(lst,start,curr,visited);
		}
	
		i++;
	}
	
	
}

vector<int> giveBuffer(int execId,int cnt){

	int layer = cnt/buffersize;
	int curr = 0;
	vector<Node *> visited;
	DBExe e;
	e = getExe(execId);
	Graph G;
	
	if(buffer.empty()){
		G = getOuterDBGraph(e.graphId);
		initializeBufferwithzero();	
		doDFSTopSort(G.FirstIndpnts,layer*buffersize,curr,visited);
		return buffer;
	}

	
	if(cnt%buffersize){
		return buffer;
	}

	G = getOuterDBGraph(e.graphId);
	visited.clear();
	curr = 0;
	doDFSTopSort(G.FirstIndpnts,layer*buffersize,curr,visited);
	
	return buffer;
}



#endif
