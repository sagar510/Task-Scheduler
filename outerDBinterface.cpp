#ifndef OUTERDBRINTERFACE_CPP
#define OUTERDBINTERFACE_CPP

#include "dbInterface.cpp"
#include<iostream>
#include<time.h>
#include<vector>

struct Node{
	int task;
	vector<struct Node *> connectedNodes;   // List of All nodes address which are the next connected nodes from this node
	unsigned int timestamp;
};

struct Graph{
	vector<Node *> FirstIndpnts; // First Independent node from all nodes where we can start our graph, its addreess list
	clock_t lifespan;
	unsigned int graphId;
};

void recFillGraph(Node *,vector<DBEdge> &edgs);


Graph getOuterDBGraph(int GID){
	
	DBGraph DBg;
	
	DBg = getDBGraph(GID);
	
	vector<DBEdge> edgsLst;
		
	edgsLst = DBg.graph;
	
	int i,j=0,from,n=edgsLst.size();
	
	vector<Node *> firstIndpnts;
	
	for(i=0;i<n;i++){
	
		from = edgsLst[i].from.elt;
		
		while(j<n && from!=edgsLst[j].to.elt){
			j++;
		}
		
		if(j>=n){
			Node *n = new Node;
			n->task = from;
			firstIndpnts.push_back(n);
		}
	}
		
	for(i=0;i<firstIndpnts.size();i++){
		recFillGraph(firstIndpnts[i],edgsLst); 
	}	
	
	Graph outerGraph;
	outerGraph.FirstIndpnts = firstIndpnts;
	outerGraph.lifespan = DBg.lifespan;
	outerGraph.graphId = DBg.graphId;
	
	return outerGraph;
}


void recFillGraph(Node *n,vector<DBEdge> &edgs){
	int tsk = n->task,i;
	
	bool found = false;
	for(i=0;i<edgs.size() && !found;i++){
		if(edgs[i].from.elt == tsk){
			found = true;
		}
	}
	
	if(found==false){
		return;
	}
	
	
	
	vector<Node *> connNodes;
	
	Node *newnode;
	
	i -= 1;   // After checking above condition, It will move pointer forward so getting back

	while(i<edgs.size()){
	
		if(edgs[i].from.elt==tsk){
			newnode = new Node;
			newnode->task = edgs[i].to.elt;
			connNodes.push_back(newnode);
		}
	
		i++;
	}
	
	for(i=0;i<connNodes.size();i++){
		recFillGraph(connNodes[i],edgs); 
	}
	
	n->connectedNodes = connNodes;
}


#endif
