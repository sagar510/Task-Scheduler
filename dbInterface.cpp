#include "dbInterface.h"





//store graph in database
int storeDBGraph(vector<DBEdge>& graph, clock_t lifespan){	
    //if graph not stored then it return graph id as -1
    int graphId = -1;
	//connection C = connect();

	try{
		connection C("dbname=graph user=tejas password=password " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
			cout << "Opened database successfully: " << C.dbname() << endl;
    
		} 
		else{
			cout << "Can't open database" << endl;
    			return graphId;
		}

		work txn(C);
		//insert graph query
		//inserting lifespan type casting clock_t to double
		string insert_query = "INSERT INTO graph (lifespan) VALUES ($1) RETURNING graph_id"; 
		result r = txn.exec_params(insert_query, static_cast<double>(lifespan));
		/*	
		string insert_query = "INSERT INTO graph (lifespan) VALUES (" + to_string(lifespan) + ") RETURNING graph_id";
		result r = txn.exec(insert_query);
		*/
		//if query gives us result
		if(!r.empty()){
			graphId = r[0][0].as<int>();
			//insert edges into edges table
			for(auto DBEdge : graph){
				int from = DBEdge.from.elt;
				int to = DBEdge.to.elt;
				string insert_edge_query = "INSERT INTO edges (from_node, to_node, graph_id) VALUES ($1, $2, $3)";
				txn.exec_params(insert_edge_query, from, to, graphId);
			}
		}
		txn.commit();
		C.disconnect();
		
	}
	//handle exception  and print error	
	catch(exception& e){
		cerr << e.what() << endl;
		
	}
	return graphId;
}


//retrieve a graph from database using graphId
DBGraph getDBGraph(int graphId){
	DBGraph d;
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
    			cout << "Opened database successfully: " << C.dbname() << endl;
    
		} 
		else{
			cout << "Can't open database" << endl;
			return d;
		}
		
		work txn(C);
		
		//retrieve graph data from database
		//counting the how many edges are present in edges table for a perticular graph
		//if edges are present then we goint to extract edges 
		string count_edge = "SELECT COUNT(*) FROM edges WHERE graph_id = $1";
		result count_result = txn.exec_params(count_edge, graphId);
		//retrieve lifespan of graph from graph table
		string get_lifespan = "SELECT lifespan FROM graph WHERE graph_id = $1";
		result lifespan_result = txn.exec_params(get_lifespan, graphId);

		int graph_lifespan = -1;
		
		//if lifespan get fetch then then we store lifespan else we put lifespan as -1
		if(!lifespan_result.empty())
			graph_lifespan = lifespan_result[0][0].as<clock_t>();

		//fetching edges 
		if(!count_result.empty()){
			int cnt_edge = count_result[0][0].as<int>();
			string select_query ="SELECT from_node, to_node FROM edges WHERE graph_id = $1";
			result edges_result = txn.exec_params(select_query, graphId);
			
			//storing retrieve edges in vector after that we put in graph
			vector<DBEdge> edge_arr; 
			for(auto row : edges_result) {
				DBEdge e;
				e.from.elt = row[0].as<int>();
				e.to.elt = row[1].as<int>();
				edge_arr.push_back(e); 
			}
			//put graph and its lifespan in dbgraph structre 

			for(auto edge : edge_arr){
    				d.graph.push_back(edge);
			}
			d.lifespan = graph_lifespan;

		}
		txn.commit();
		C.disconnect();		



	}	
	catch(exception& e){
		cerr << e.what() << endl;
    
	}
	return d;
}


void storeLifeSpan(vector<DBNode>& lifespanArr, int graphId){
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
			cout << "Opened database successfully: " << C.dbname() << endl;
    
		} 
		else{
			cout << "Can't open database" << endl;
			return;
		}

		work txn(C);
		//revtrieving element and its lifespan from lifespan array and store it in nodelifespan table
		for(int i = 0; i < lifespanArr.size(); i++){
			int n = lifespanArr[i].elt;
			//cout << n << " ********";
			int lp = lifespanArr[i].time;
			string insert_query = "INSERT INTO nodelifespan (data, lifespan, graph_id) VALUES ($1, $2, $3)";
			txn.exec_params(insert_query, n, lp, graphId);
		}
		/*
		for(int &n: node, int &lp : lifespan){
    			string insert_query = "INSERT INTO nodelifespan (data, lifespan, graph_id) VALUES ($1, $2, $3)";
    			txn.exec_params(insert_query, n, lp, graphId);
		}
		*/
		txn.commit();
		C.disconnect();
	} 
	catch(exception& e){
		cerr << e.what() << endl;
		return;
	}
}



vector<DBNode> retrieveNodeLifespans(int graphId){
	vector<DBNode> nodelifespan_arr;
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
			cout << "Opened database successfully: " << C.dbname() << endl;
    
		} 
		else{
			cout << "Can't open database" << endl;
			return nodelifespan_arr;
		}
		work txn(C);
		//retrieve node element and its lifespan from graph
		string select_query = "SELECT data, lifespan FROM nodelifespan WHERE graph_id = $1";
		result res = txn.exec_params(select_query, graphId);
		
		//create a node and put retrieved element and time in to that node then put that node into the nodelifespan array
		for(auto row : res){
			DBNode n;
			(n.elt) = row[0].as<int>();			
			(n.time) = row[1].as<clock_t>();
			nodelifespan_arr.push_back(n);
			/*
			cout << row[0] << " ****** " << enldl;
			cout << typeid(row[0]).name() << endl;
			cout << row[0].as<int>() << endl;			
			printf("%x--\n",element);
			cout << element << endl;
			(n.elt) = element;
			cout << typeid((n.elt)).name() << endl;
			cout << (int)(n.elt) << endl;
			printf("%x==\n",n.elt);
			n.elt = row[0].as<int>();
			cout << n.elt << " ********* " << endl;
			*/
		}
		txn.commit();
		C.disconnect();
	} 
	catch(const exception &e){
    		cerr << e.what() << endl;
	}
	return nodelifespan_arr;
}

//return execution id
int storeExe(int graphId){
	int exe_id = -1;
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
			cout << "Opened database successfully: " << C.dbname() << endl;

		}
		else{
			cout << "Can't open database" << endl;
    			return exe_id;
		}

		work txn(C);

		int zero = 0;
		string insert_query = "INSERT INTO execution (count, graph_id) VALUES ($1, $2) RETURNING exe_id";
		result r = txn.exec_params(insert_query, zero, graphId);

		if(!r.empty()){
			exe_id = r[0][0].as<int>();
		}
		
		txn.commit();
		C.disconnect();
	}
	catch(exception& e){
		cerr << e.what() << endl;
	}
	return exe_id;


}



//return DBExe which consist graphId and its count
DBExe getExe(int exeId){
	DBExe d;
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
    			cout << "Opened database successfully: " << C.dbname() << endl;

		}
		else{
			cout << "Can't open database" << endl;
			return d;
		}

		work txn(C);

		string select_query = "SELECT count, graph_id FROM execution WHERE exe_id = $1";
		result r = txn.exec_params(select_query, exeId);

		if(!r.empty()){

			for(auto row : r){
				d.count = row[0].as<int>();
				d.graphId = row[1].as<int>();
			}
		}
		txn.commit();
		C.disconnect();
	}
	catch(const exception &e){
    		cerr << e.what() << endl;
	}
	return d;

}	
//update count for exeId
void updateCount(int exeId, int cnt){
	try{
		connection C("dbname=graph user=yash password=yash123 " \
             	"hostaddr=127.0.0.1 port=5432");
		if(C.is_open()){
			cout << "Opened database successfully: " << C.dbname() << endl;
    
		} 
		else{
			cout << "Can't open database" << endl;
			return;
		}
		work txn(C);

		string update_query = "UPDATE execution SET count = $1 where exe_id = $2";
		txn.exec_params(update_query, cnt, exeId);

		txn.commit();
		C.disconnect();
	}
	catch(exception& e){
		cerr << e.what() << endl;
		return;
	}
}
