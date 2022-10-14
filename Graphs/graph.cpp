#include <iostream>
#include <vector>
#include <unordered_map>
#include <forward_list>
#include <stack>			// LIFO - for DFS
#include <queue> 			// FIFO - for BFS
#include <limits>

using namespace std;

const size_t v = 10;

// Graph implementations (V - vertices number, E - edges number)
// 1. Adjacency matrix (memory - O(V^2)) 
// matix[n][m] cell represents the edge (or it's absence) between n amd m verticies.

vector<vector<int>> adjacency_matrix(v, vector<int>(v));

// 2. Adjacency list (memory - O(V + E))
// map each vertex with it's adjacent vertices list

unordered_map<int, int> adjacency_list_map;
vector<vector<int>> adjacency_list;

// 3. Edges list (memory - O(E))
// from - where - weight
// {<from: 1, to: 2, weight: -3>, 
//  <from: 2, to: 1, weight: 2>, 
//  <from: 3, to: 1, weight: 2>, 
//  <from: 1, to: 3, weight: 1>,
//  <from: 2, to: 3, weight: 7>}


class AdjacencyListGraph
{
public:

	enum class color: char{
		WHITE = 0,
		GRAY,
		BLACK,
	};

	AdjacencyListGraph(size_t vertices_num)
	{
		adjacency_list.resize(vertices_num);
		//vectices_colors.resize(vertices_num, color::WHITE);
	}

	// vertices numeration from 1
	void add_edge(int vertex_from, int vertex_to)
	{
		adjacency_list[vertex_from - 1].push_back(vertex_to - 1);
	}

	const vector<int>& get_adjacent_vertices(int vertex)
	{
		return adjacency_list[vertex - 1];
	}

	// Time-Complexity: for adjacent_matrix: O(V^2), for adjacent_list: O(V + E)
	// DFS helps:
	// 1. find cycles in graph
	// 2. find linked components in graph
	// 3. topology sort (except graphs with cycles)
	forward_list<int> dfs()
	{
		vector<color> visited_vertices(adjacency_list.size(), color::WHITE);
		forward_list<int> result;

		// linked components counter
		// if needed - verticies from one component may be colored with 
		// components_cnt colour instead of BLACK
		int components_cnt = 0;

		// Check all of the vericies (graph may have different unlinked parts)
		for(size_t i = 0; i < adjacency_list.size(); ++i){
			if( visited_vertices[i] == color::WHITE ){
				++components_cnt;
				dfs_iterative(i, visited_vertices, result);
			}
		}

		cout << "linked components: " << components_cnt << endl;

		// Restore original traversal order
		result.reverse();
		return result;
	}

	// Time-Complexity: O(V + E)
	// BFS helps;
	// 1. find MIN distances between verticies (for non-weighted graphs)
	// 2. check if path between vertices exists
	// 3. find shortest path between vertices
	forward_list<int> bfs(int start_vertex = 0, vector<int> *pdistances = nullptr)
	{
		vector<color> visited_vertices(adjacency_list.size(), color::WHITE);
		forward_list<int> result;

		// distances from start vertex to another
		vector<int> distances(adjacency_list.size(), numeric_limits<int>::max()); 	
		distances[start_vertex] = 0;

		// for shortest path restoring [vertex] -> vertex from previous layer
		vector<int> previous(adjacency_list.size());

		queue<int> queue_helper;
		queue_helper.push(start_vertex);

		while( !queue_helper.empty() ){

			const int curr_vertex = queue_helper.front();
			queue_helper.pop();

			result.push_front(curr_vertex + 1);

			const vector<int> &adjacent_vertices = adjacency_list.at(curr_vertex);

			for(const auto &vertex : adjacent_vertices){

				if(visited_vertices.at(vertex) == color::WHITE){
					// mark as visited
					visited_vertices[vertex] = color::BLACK;
					// update distance to the next layer (= curr layer + 1)
					distances[vertex] = distances[curr_vertex] + 1;
					previous[vertex] = curr_vertex;
					queue_helper.push(vertex);
				}
			}
		}

		if(pdistances){
			*pdistances = distances;
		}

		result.reverse();
		return result;
	}

	// finds path from start_vertex of BFS to the end_vertex
	// stack<int> shortest_path(int end_vertex, const vector<int> &distances)
	// {
	// 	stack<int> path;


	// }

private:	
	vector<vector<int>> adjacency_list;
	
	void dfs_(int start_vertex, vector<color> &visited_vertices, forward_list<int> &result)
	{
		result.push_front(start_vertex + 1);

		visited_vertices[start_vertex] = color::GRAY;
		const vector<int> &adjacent_vertices = get_adjacent_vertices(start_vertex + 1);

		for(const auto &vertex : adjacent_vertices){

			if( visited_vertices[vertex] == color::WHITE ){
				dfs_(vertex, visited_vertices, result);
			}

		}

		visited_vertices[start_vertex] = color::BLACK;
	}

	void dfs_iterative(int start_vertex, vector<color> &visited_vertices, forward_list<int> &result)
	{
		// Using 3 colors helps in finding cycles in graph

		stack<int> stack_helper;

		stack_helper.push(start_vertex);

		while( !stack_helper.empty() ){
			const int curr_vertex = stack_helper.top();
			stack_helper.pop();

			if(visited_vertices[curr_vertex] == color::GRAY){
				// Mark vertex as completly proccessed after found that it was in use
				visited_vertices[curr_vertex] = color::BLACK;
				continue;
			}

			if(visited_vertices[curr_vertex] == color::WHITE){
				// Add all adjacent vertices to stack, mark current vertex as GRAY
				visited_vertices[curr_vertex] = color::GRAY;
				result.push_front(curr_vertex + 1);

				// Return current vertex to the stack, to be marked as BLACK after 
				// proceeding all of it's adjacent neighbours
				stack_helper.push(curr_vertex);

				auto adjacent_vertices = get_adjacent_vertices(curr_vertex + 1);

				// for(const auto &vertex : adjacent_vertices){
				for(auto it = adjacent_vertices.crbegin(); it != adjacent_vertices.crend(); ++it){
					// if order is important, push in stack in reverse adjacent 
					if(visited_vertices[*it] == color::WHITE){
						stack_helper.push(*it);
					}
				}
			}
		}
	}

	void dfs_iterative2(int start_vertex, vector<color> &visited_vertices, forward_list<int> &result)
	{
		// With 2 colors only

		stack<int> stack_helper;
		stack_helper.push(start_vertex);

		while( !stack_helper.empty() ){

			const int curr_vertex = stack_helper.top();
			stack_helper.pop();

			result.push_front(curr_vertex + 1);

			auto adjacent_vertices = get_adjacent_vertices(curr_vertex + 1);

			for(auto it = adjacent_vertices.crbegin(); it != adjacent_vertices.crend(); ++it){
				if(visited_vertices[*it] == color::WHITE){
					// Mark vertex as completly proccessed
					visited_vertices[*it] = color::BLACK;
					stack_helper.push(*it);
				}
			}
		}
	}


};




int main()
{
/*
          1          
         / \         
        2   3        
       / \   \       
      4---5   6      
               \     
                7 

      8---9
*/

	AdjacencyListGraph graph(9);

	graph.add_edge(1, 2);
	graph.add_edge(1, 3);
	graph.add_edge(2, 4);
	graph.add_edge(2, 5);
	graph.add_edge(4, 5);
	graph.add_edge(5, 2); // cycle

	graph.add_edge(3, 6);
	graph.add_edge(6, 7);

	graph.add_edge(8, 9);

	forward_list<int> res = graph.dfs();
	cout << "DFS: ";
	for(const auto &item : res){
		cout << item << " ";
	}
	cout << endl;

	forward_list<int> res2 = graph.bfs();
	cout << "BFS: ";
	for(const auto &item : res2){
		cout << item << " ";
	}
	cout << endl;

	return 0;
}