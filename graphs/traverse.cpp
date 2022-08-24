#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

// vertex_idx -> list of adjacent verticies
using adjacency_list = vector<vector<int>>;


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
	vector<bool> visited(graph.size(), false);

	for(int vertex = 0; vertex < graph.size(); ++vertex){

		if( !visited[vertex] ){
			DFS(graph, vertex, visited);
		}
	}
}

