#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <limits>
#include <utility>
#include <stack>

using namespace std;


using adjacency_list = vector<vector<int>>;

// Note: graph vertices numeration starts from 1, but all arrays 
// have 0 index to represent emptiness

vector<int> BFS(const adjacency_list &graph, int start_vertex = 1)
{
	vector<int> previous(graph.size(), -1);		// for shortest path restoring
	vector<int> distances(graph.size(), -1);	// distances from start vertex to others
	vector<bool> visited(graph.size(), false);
	queue<int> queue_helper;

	visited[start_vertex] = true;
	queue_helper.push(start_vertex); 
	distances[start_vertex] = 0;

	while( !queue_helper.empty() ){

		const int curr_vertex = queue_helper.front();
		queue_helper.pop();

		for(const auto &adjacent_vertex : graph[curr_vertex]){

			if( !visited[adjacent_vertex]){
				distances[adjacent_vertex] = distances[curr_vertex] + 1;
				previous[adjacent_vertex] = curr_vertex;
				visited[adjacent_vertex] = true;
				queue_helper.push(adjacent_vertex);
			}
		}
	}

	return previous;
}

// Shortest path from start_vertex to end_vertex
// init phase - O(V)
// processing - each vertex added to queue one time: O(E)
// total: O(V + E)
stack<int> shortest_path(const vector<int> &previous, int end_vertex)
{
	stack<int> path;
	int prev_vertex = end_vertex;

	// start_vertex has no prev (-1)
	while(prev_vertex != -1){

		path.push(prev_vertex);
		prev_vertex = previous[prev_vertex];
	}

	return path;
}


// Task about 2-D field (or matrix)
//
// Дано описание поля размера N×M с помощью массива строк одинаковой длины, 
// в котором символ точка ‘.’ означает свободную клетку,  а решётка ‘#’ — занятую. 
// Из свободной клетки за один ход можно переместиться в любую соседнюю по стороне 
// свободную клетку. 
// Нужно определить длину кратчайшего пути из левого верхнего угла  в правый нижний. 
// Если такого пути не существует,  вернуть -1. Выходить за пределы поля нельзя.

struct Cell
{
	Cell(int row_, int col_): row(row_), col(col_) {}
	int row = 0;
	int col = 0;
};

// Up, Left, Right, Down
const vector<Cell> available_moves{ {0, 1}, {-1, 0}, {1, 0}, {0, -1} };

// 		"...#...",			// x y ->
// 		".....#.",			// |
// 		"##.##..",			// v
// 		"....##.",			//
// 		".##..#.",			// 5 x 7: x{0..4}, y{0..6}

int shortest_path_length(const vector<string> &field)
{
	if(field.empty()){
		return -1;
	}

	queue<Cell> planned;

	const int rows = field.size();
	const int cols = field[0].size();

	Cell start{0, 0};
	Cell end{rows - 1, cols - 1};

	if(field[start.row][start.col] == '#' || field[end.row][end.col] == '#'){
		return -1;
	}

	vector<bool> visited(rows * cols, false);
	vector<int> distances(rows * cols, -1);

	distances[start.row * cols + start.col] = 0;
	planned.push(start);
	visited[0] = true;		// cell_to_linear_index = [ cell.row * cols + cell.col ]

	// Start BFS
	while( !planned.empty() ){

		const Cell curr_cell = planned.front();
		planned.pop();

		for(const Cell &movement : available_moves){

			const Cell adjacent_cell{curr_cell.row + movement.row, curr_cell.col + movement.col};

			// Check if field's borders are not crossed
			if( (adjacent_cell.row < 0) || 
				(adjacent_cell.row >= rows) ||
				(adjacent_cell.col < 0) ||
				(adjacent_cell.col >= cols) )
			{
				continue;
			}

			// Do BFS with the adjacent_cell
			const int idx = adjacent_cell.row * cols + adjacent_cell.col;

			if( (field[adjacent_cell.row][adjacent_cell.col] != '#') && !visited[idx] )
			{
				distances[idx] = distances[curr_cell.row * cols + curr_cell.col] + 1;
				planned.push(adjacent_cell);
				visited[idx] = true;
			}
		}
	}

	return distances[end.row * cols + end.col];
}






int main()
{
	//              [6]     [8]
	//            // |  ^ 
	//           //  |   \ 
	//          //   v    \
	//         [5] -> [4] -> [2]
	//         ^    ^ ^    ^ ^
	//         |   /  |   /  |
	//         |  /   |  /   |
	//        [3] -> [1]    [7]

	adjacency_list graph(9, vector<int>());

	graph[1].push_back(2);
	graph[1].push_back(4);

	graph[2].push_back(6);

	graph[3].push_back(1);
	graph[3].push_back(4);
	graph[3].push_back(5);

	graph[4].push_back(2);

	graph[5].push_back(4);
	graph[5].push_back(6);

	graph[6].push_back(5);
	graph[6].push_back(4);

	graph[7].push_back(2);

	vector<int> previous = BFS(graph, 3);
	stack<int> path = shortest_path(previous, 6);

	while( !path.empty() ){
		cout << path.top() << "-";
		path.pop();
	}
	cout << endl;

	vector<string> field1{
		"...#...",	
		".....#.",
		"##.##..",
		"....#..",
		".##..#.",
	};

	vector<string> field2{
		".....",
	};

	vector<string> field3{
		".......#",
	};

	cout << "path length on field1: " << shortest_path_length(field1) << endl;
	cout << "path length on field2: " << shortest_path_length(field2) << endl;
	cout << "path length on field3: " << shortest_path_length(field3) << endl;

	return 0;
}