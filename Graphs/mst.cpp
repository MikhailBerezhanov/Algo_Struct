#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// Minimum spanning tree - contains all graph verticies connected with min-weight edges



struct Edge
{
	Edge(int s, int e, int w): start(s), end(e), weight(w) {}

	int get_key() const { return weight; }

	int start = -1;
	int end = -1;
	int weight = -1; 
};

// std::greater<T>  uses  operator>(const T&, const T&)  by default
bool operator>(const Edge &left, const Edge &right)
{
	return left.get_key() > right.get_key();
}

// OR
struct Edges_Comparator
{
	bool operator() (const Edge &left, const Edge &right)
	{
		return left.get_key() > right.get_key();
	}
};

// default comparator: std::less  -  MAX heap (max element at the top)
// use std::greater for MIN heap
priority_queue<Edge, vector<Edge>, /*std::greater<Edge> OR*/ Edges_Comparator> min_heap_edges;





// vertex -> (adjacent_vertex, edge_weight)
using adjacency_list = unordered_map<int, unordered_map<int, int>>;

vector<bool> not_added_vertices;

// Add edges (starting from start_vertex and ending NOT in current spanning tree)
// to the min_heap.
void add_edges(int start_vertex, const adjacency_list &graph)
{
	// Mark start_vertex as part of current spanning tree.
	not_added_vertices[start_vertex - 1] = false;

	auto it = graph.find(start_vertex);
	if(it == graph.end()){
		return;
	}

	// Process every adjacent vertex that is NOT in the current spanning tree
	for(const auto &adjacent_vertex : it->second){

		// Add edge to the min heap
		if(not_added_vertices[adjacent_vertex.first - 1]){
			min_heap_edges.push( {start_vertex, adjacent_vertex.first, adjacent_vertex.second} );
		}
	}

}

uint64_t fimd_minimum_spanning_tree(size_t vertices_count, const adjacency_list &graph)
{
	// Current spanning tree. Initialized as empty - no vertex added yet.
	not_added_vertices.assign(vertices_count, true);
	uint64_t min_total_weight = 0;

	// Start from any vertex - in the end all of them might be in the spanning tree
	add_edges(1, graph);
	size_t added_vertices_num = 1;

	while( (added_vertices_num < vertices_count) && !min_heap_edges.empty() ){

		// Select minimum-weight edge going from the last processed vertex 
		Edge min_weight_edge = min_heap_edges.top();
		min_heap_edges.pop();

		// Check if the end vertex of obtained edge was not processed already
		if(not_added_vertices[min_weight_edge.end - 1]){
			min_total_weight += min_weight_edge.weight;
			// Add edges of the end vertex of selected edge 
			add_edges(min_weight_edge.end, graph);
			++added_vertices_num;
		}
	}

	if(added_vertices_num != vertices_count){
		throw std::runtime_error("Graph is not linked");
	}

	return min_total_weight;
}



int main()
{
	//
	//                        [5]
	//                       / | \
	//                      /  |  \
	//                    4/   |8  \6
	//                    /    |    \ 
	//                   /  3  |     \
	//                 [4]----[6]    [1]
	//                  |    /  \     /
	//            (2)  9|  1/   7\   /2
	//                  |  /      \ /
	//                  [3]-------[2]
	//                        5	
	//
	// MIN ST from vertex n.1: 2 + 5 + 1 + 3 + 4 = 15

	adjacency_list test_graph;
	//      start end  weight
	test_graph[1][2] = 2;
	test_graph[1][5] = 6;

	test_graph[2][1] = 2;
	test_graph[2][6] = 7;
	test_graph[2][3] = 5;

	test_graph[3][2] = 5;
	test_graph[3][4] = 9;
	test_graph[3][6] = 1;

	test_graph[4][3] = 9;
	test_graph[4][5] = 4;
	test_graph[4][6] = 3;

	test_graph[5][4] = 4;
	test_graph[5][6] = 8;
	test_graph[5][1] = 6;

	test_graph[6][4] = 3;
	test_graph[6][5] = 8;
	test_graph[6][3] = 1;
	test_graph[6][2] = 7;

	try{
		cout << fimd_minimum_spanning_tree(6, test_graph) << endl;
	}
	catch(std::exception &e){
		cerr << e.what() << endl;
	}

	return 0;
}
