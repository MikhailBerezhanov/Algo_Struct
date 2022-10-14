#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// Топологическая сортировка ориентированного графа — это такое упорядочивание 
// всех вершин этого графа, что если существует ребро (v,w)(v,w), то в итоговой 
// последовательности vv располагается раньше, чем ww.
//
// Из этого определения можно вывести следствие: если в графе есть путь из 
// некоторой вершины ss до некоторой другой вершины tt, то топологическая 
// сортировка должна поставить ss раньше, чем tt.

// NOTE: Topology sort is for directed Graphs with no cycles only.
// DAG (directed acyclic graphs)

// index - vertex number 
// vector - set of adjacent verticies
using adjacency_list = std::vector<std::vector<int>>;


void DFS(const adjacency_list &graph, int start_vertex, vector<char> &colors, stack<int> &sort_res)
{
	std::stack<int> stack_helper;

	// 1. Do DFS. 
	stack_helper.push(start_vertex);

	// Say that there is a path vetween verticies 'S' and 'T'.
	// Topology sort must place position[S] earlier than position[T] at output
	// 		position[S] < position[T]
	//
	// At the same time, when we do DFS, vertex 'T' will be processed (colored with black) 
	// earlier than 'S'. It means that leave_time of 'S' is greater than 'T' 
	//		leave[T] < leave[S]
	//
	// So if leave[T] < leave[S] while DFS, than position[S] < position[T] 

	while( !stack_helper.empty() ){

		const int curr_vertex = stack_helper.top();
		stack_helper.pop();

		// Check if was already visited (such vertex will appear in backward moving)
		if(colors[curr_vertex] == 1){
			// 2. When vertex is fully processed, add it at the top of result stack.
			colors[curr_vertex] = 2;
			sort_res.push(curr_vertex);
			continue;
		}

		// Vertex may be processed (black) in other path, so check if it is white yet
		//
		//    2
		//  /   \
		// 4 --- 5 <- first time we push it white, second time (4 -> 5) we push it white, 
		//            than 5 becomes gray -> black; and we'll pop it again after first push
		//			  (it is not white, it is already processed)
		//
		if(colors[curr_vertex] == 0){
			colors[curr_vertex] = 1; // visited
			stack_helper.push(curr_vertex);

			for(const auto &adjacent_vertex : graph[curr_vertex]){

				if(colors[adjacent_vertex] == 0){
					stack_helper.push(adjacent_vertex);
				}
			}
		}
	}
}



std::stack<int> topology_sort(const adjacency_list &graph)
{
	std::stack<int> res;

	// 0 - white (not visited), 1 - gray (visited), 2 - black (fully processed)
	vector<char> colors(graph.size(), 0);
	
	for(int vertex = 0; vertex < graph.size(); ++vertex){

		if(colors[vertex] == 0){
			DFS(graph, vertex, colors, res);
		}

	}

	return res;
}




int main()
{
	//
	//			 [f]      [c]
	//			 |  \     / |
	//			 |   \   /  |
	//			 v    v v   |
	//	[e] --> [a]   [g]   |
	//		   /   \   |    |
	//		  /     \  |    |
	//       v       v v    v
	//		[b]      [h]   [d]
	//
	//
	// Tsort possible outputs: f, e, a, b, c, g, h, d
	//
	//						   f, e, c, g, d, a, b, h


	adjacency_list graph;

	graph.assign(8, vector<int>()); // create verticies

	graph['a' - 'a'].push_back('b' - 'a');
	graph['a' - 'a'].push_back('h' - 'a');

	graph['c' - 'a'].push_back('g' - 'a');
	graph['c' - 'a'].push_back('d' - 'a');

	graph['e' - 'a'].push_back('a' - 'a');

	graph['f' - 'a'].push_back('a' - 'a');
	graph['f' - 'a'].push_back('g' - 'a');

	graph['g' - 'a'].push_back('h' - 'a');

	stack<int> sort_res = topology_sort(graph);

	while( !sort_res.empty() ){
		int elem = sort_res.top();
		sort_res.pop();

		cout << static_cast<char>(elem + 'a') << " ";
	}
	cout << endl;

	return 0;
}