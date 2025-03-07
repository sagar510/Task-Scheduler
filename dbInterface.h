//#include <bits/stdc++.h>
#include <iostream>
#include <time.h>
#include <iterator>
#include <stdio.h>
#include <pqxx/pqxx>
#include <vector>

using namespace std;
using namespace pqxx;

enum dbError { SUCCESS, ERROR, GRAPH_EXISTS, GRAPHID_EXISTS, CYCLE_DETECTED};


typedef struct DBNode{
	int elt;	
	clock_t time;		//Time required to execute a task(node)
}DBNode;


typedef struct DBEdge{
	//unsigned int count;	//Count of No. of destinations from source
	DBNode from;			//Source node
	DBNode to;		//All the destination nodes from the source
}DBEdge;


typedef struct DBGraph{
	clock_t lifespan;	//Lifespan of a graph to execute
	vector<DBEdge> graph;
	unsigned int graphId;
}DBGraph;

typedef struct DBError{
	enum dbError e;

}DBError;

typedef struct DBExe{
	int count;
	int graphId;
}DBExe;


//function defination
int storeDBGraph(vector<DBEdge>& graph, clock_t lifespan);
DBGraph getDBGraph(int graphId);
void storeLifeSpan(int graphId, vector<int>& node, vector<int>& lifespan);
vector<DBNode> retrieveNodeLifespans(int graphId);

//return execution id
int storeExe(int graphId);

//return DBExe which consist graphId and its count
DBExe getExe(int exeId);

//update count for exeId
void updateCount(int exeId, int count);







/*
int storeGraph(DBGraph **graph, int graphId, clock_t lifespan);
//Store graph and return its id

DBError storeExeOrder(int *arr);
//it stores sequence in order where user have a choice to select an execution node 
//from the graph where multiple nodes is available to execute .
//we give any of node to execute if user wants any another node to execute then we can give node 
//from the available node to execute
//2d array stores possible node avaible in every stage and we update array after node selection
//from selecting repeated array

DBError storeTask(Node node);
//we can store current node or which is the next task which is going to execute

DBGraph** getGraph(int graphId);
//access graph from the database

int** getExeOrder(int graphId);
//getting the sequece of node which is going to be execute

Node getTask();
//retieve current or next task
*/


