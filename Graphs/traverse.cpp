#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

// vertex_idx -> list of adjacent verticies
using adjacency_list = vector<vector<int>>;

// Time-Complexity: 
// - for adjacent_matrix: O(V^2), 
// - for adjacent_list: O(V + E)
// Use cases:
// 1. Calculate linked components of graph
// 2. Find cycle in graph (with 3 colors - WHITE, BLACK, GRAY)
// 3. Topology sort (except graph with cycles)

void DFS(const adjacency_list &graph, int start_vertex, vector<bool> &visited)
{
	stack<int> stack_helper;
	stack_helper.push(start_vertex);

	while( !stack_helper.empty() ){
		const int curr_vertex = stack_helper.top();
		stack_helper.pop();

		cout << curr_vertex << " "; // do smth with visited vertex

		const vector<int> &adjacent_verticies = graph.at(curr_vertex);

		// Reversal traverse for making stack.top() work in suspected order
		for(auto it = adjacent_verticies.crbegin(); it != adjacent_verticies.crend(); ++it){

			if( !visited[ *it ] ){
				visited[ *it ] = true;
				stack_helper.push( *it );
			}
		}
	}

}

void full_DFS(const adjacency_list &graph)
{
	int linked_components = 0;
	vector<bool> visited(graph.size(), false);

	for(int vertex = 0; vertex < graph.size(); ++vertex){

		if( !visited[vertex] ){
			DFS(graph, vertex, visited);
			++linked_components;
		}
	}
}

//
//
// 1. Find MIN distance from start to end vertex
// 2. Check if path from start to end vertex exists
// 3. Get shortest path from start to end vertex

void BFS(const adjacency_list &graph, int start_vertex)
{
	vector<int> distances(graph.size(), -1);
	vector<bool> visited(graph.size(), false);
	queue<int> q_helper;

	q_helper.push(start_vertex);
	distances[start_vertex] = 0;

	while( !q_helper.empty() ){

		const int curr_vertex = q_helper.front();
		q_helper.pop();

		// Do smth with vertex
		cout << curr_vertex << " ";

		for(const auto &adj_vertex : graph.at(curr_vertex)){

			if( !visited.at(adj_vertex) ){
				q_helper.push(adj_vertex);
				visited[adj_vertex] = true;

				distances[adj_vertex] = distances.at(curr_vertex) + 1;
			}
		}
	}
}


/*
       
      0 - 1          
     /   / \         
    10  2   3        
     \ / \   \       
      4---5   6      
               \     
                7 

      8---9
*/

int main()
{
	adjacency_list gpaph(11);

	gpaph[0].push_back(1);
	gpaph[0].push_back(10);

	gpaph[1].push_back(2);
	gpaph[1].push_back(3);

	gpaph[2].push_back(4);
	gpaph[2].push_back(5);

	gpaph[3].push_back(6);

	gpaph[6].push_back(7);

	gpaph[8].push_back(9);

	gpaph[10].push_back(4);


	cout << "DFS: ";
	full_DFS(gpaph);
	cout << endl;

	cout << "BFS: ";
	BFS(gpaph, 0);
	cout << endl;

	return 0;
}
