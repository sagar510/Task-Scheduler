#include<vector>

using namespace std;

enum State{
	sucess,
	graphNotExist,
	executionNotExist,
	nodeNotExist,
	cycleDetected
};

struct Status{
	enum State state;
};

struct ResultGraph{
	Status s;
	int GID;
};

struct ResultExecution{
	Status s;
	int EID;
};

struct ResultNode{
	Status s;
	int Task;
};

/*struct Node{
	int task;
	vector<struct Node *> connectedNodes;   // List of All nodes address which are the next connected nodes from this node
	unsigned int timestamp;
};


struct Graph{
	vector<Node *> FirstIndpnts; // First Independent node from all nodes where we can start our graph, its addreess list
	unsigned int lifespan;
};*/



//This is Only for User
struct Edge{
	int from;
	int to;
};

// Methods

// k-> no. of edges  n-> No. of Nodes
ResultGraph createGraph(Edge *lst,unsigned int k,unsigned int n);

ResultExecution startExecution(int gid);

ResultNode nextTask(int execId);



