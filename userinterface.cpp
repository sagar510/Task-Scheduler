// This file consist of implementation of methods of userinterface.h

#ifndef USERINTERFACE_H
#define USERINTERFACE_H


#include"middleComputationLayer.cpp"
#include"userinterface.h"
#include<iostream>
#include<vector>

using namespace std;

bool isCyclic(Edge *,int,int);

ResultGraph createGraph(Edge *lst,unsigned int k,unsigned int n){

	ResultGraph resultGraph;
	
	if(isCyclic(lst,k,n)){
		resultGraph.s.state = cycleDetected;
		return resultGraph;
	}
	
	
	vector<DBEdge> DBedgs;
	DBEdge tempedg;
	DBNode n1,n2;
	
	int i=0;
	
	while(i<k){
		n1.elt = lst[i].from;
		n2.elt = lst[i].to;
		
		tempedg.from = n1;
		tempedg.to = n2;
		
		DBedgs.push_back(tempedg);
		i++;
	} 
	
		
	int newgraphId;
	
	newgraphId = storeDBGraph(DBedgs,0);
	
	resultGraph.s.state = sucess;	
	
	return resultGraph;
}


ResultExecution startExecution(int gid){

	ResultExecution resultExecution;
	
	int newExecId = storeExe(gid);
	
	updateCount(newExecId,0);
	
	resultExecution.EID = newExecId;
	resultExecution.s.state = sucess;
	
	return resultExecution;
}


ResultNode nextTask(int execId){
	
	DBExe e = getExe(execId);
	
	int count = e.count;
	int bufferIdxRequired = (count%buffersize)+1;
	
	vector<int> buffer_here;
	
	buffer_here = giveBuffer(execId,count);

	ResultNode resultNode;
	
	if(bufferIdxRequired<buffer_here.size()){
		int nexTask = buffer_here[bufferIdxRequired];
		resultNode.Task = nexTask;
		updateCount(execId,count+1);
		resultNode.s.state = sucess; 	
	}
	else{
		resultNode.s.state = nodeNotExist;
	}
	
	return resultNode;
}


// Cycle Detection Code starts Here

bool checkCycle(int node, vector<vector<int>>& adj, vector<int>& vis, vector<int>& dfsVis) {
        vis[node] = 1;
        dfsVis[node] = 1;
        for (auto it : adj[node]) {
            if (!vis[it]) {
                if (checkCycle(it, adj, vis, dfsVis)) return true;
            }
            else if (dfsVis[it]) {
                return true;
            }
        }
        dfsVis[node] = 0;
        return false;
    }

    void addEdge(vector<vector<int>>& adj, const Edge& e) {
        adj[e.from].push_back(e.to);
    }


    bool isCyclic(int N, vector<vector<int>>& adj) {
        vector<int> vis(N, 0);
        vector<int> dfsVis(N, 0);

        for (int i = 0; i < N; i++) {
            if (!vis[i]) {
                if (checkCycle(i, adj, vis, dfsVis)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool isCyclic(Edge* lst, int noOfNode, int noOfEdges) {
        vector<vector<int>> adj(noOfNode);

        for (int i = 0; i < noOfEdges; i++) {
            addEdge(adj, lst[i]);
        }

        return isCyclic(noOfNode, adj);
    }




#endif



