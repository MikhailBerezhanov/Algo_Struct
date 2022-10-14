#include <iostream>
#include <vector>

using namespace std;

// есть N предметов, вес каждого — положительное целое число. 
// Веса записаны в массиве arr. Рюкзак может выдержать содержимое 
// не тяжелее X. Выберите такое подмножество предметов, чтобы их 
// суммарный вес был максимальный, но не превосходил X.

// N = 3
// X = 12
// arr = [6, 10, 5]
//
// Жадный лагорит не подойдет, он выберет 10,  в том время как 5+6 обсепечит
// бОльший суммарный вмещающийся вес.

//               j -> возможный вес рюкзака до Х включительно
// i - взятые  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
// | предметы  --------------------------------------------------------  
// v 0         | + | - | - | - | - | - | - | - | - | - |  - |  - |  - |
//   1 (6kg)   | + | - | - | - | - | - | + | - | - | - |  - |  - |  - |
//   2 (10kg)  | + | - | - | - | - | - | + | - | - | - |  + |  - |  - |
//   3 (5kg)   | + | - | - | - | - | + | + | - | - | - |  + |  + |  - |
//     
// dp - матрица булевых значений
// dp[i][j] - возможно ли получить вес j, используя набор предметов i


// 1. Что содержит матрица dp ? dp[N + 1][X + 1]
// Булевые Флаги - возможности получения веса j, используя набор предметов i

// 2. Какой базовый случай?
// Не взяв любой предмет можно получить рюкзак весом 0 кг.

// 3. Формула перехода dp[i][j]
//
//                        / dp[i - 1][j] = true - встречался набор, дающий вес j.
// dp[i][j] == true , если
//                        \ dp[i - 1][j - arr[i - 1]] = true 
// 	                         - взять текущий предмет, в оставшийся вес 
//                           (j - current_weight) положить встретившийся  
//                           ранее набор и получить вес j.

// 4. Как будет заполняться матрица dp?
// С увеличением i матрица будет заполняться построчно

// 5. Где хранится ответ на задачу?
// последняя заполненная строка матрицы с максимальный j , таким что 
// dp[N][j] == true

void ver1(const vector<uint> &arr, uint X)
{
	size_t N = arr.size();

	vector<vector<bool>> dp(N + 1, vector<bool>(X + 1, false));

	for(size_t i = 0; i <= N; ++i){
		dp[i][0] = true;
	}


	for(int i = 1; i <= N; ++i){

		for(int j = 1; j <= X; ++j){

			if(dp[i - 1][j]){
				dp[i][j] = true;
			}
			else{
				int col = j - arr[i - 1];

				if(col >= 0 && dp[i - 1][col]){
					dp[i][j] = true;
				}
			}
		}
	}

	// Получаение результата
	uint max_weight = 0;
	for(size_t j = 1; j <= X; ++j){
		if(dp[N][j]){
			max_weight = j;
		}
	}

	cout << "v1: max_weight: " << max_weight << endl;

	// Восстановление набор выбранных предметов
	vector<uint> path;

	int prev_row = N;
	int prev_col = max_weight;

	while((prev_row > 0) && (prev_col > 0)){

		int col_idx = prev_col - arr[prev_row - 1];

		if(col_idx >= 0 && dp[prev_row - 1][col_idx]){
			prev_col = col_idx;
			path.push_back(arr[prev_row - 1]);
		}

		prev_row = prev_row - 1;
	}


	cout << "v1: using elements: ";
	for(const auto &elem : path){
		cout << elem << " ";
	}
	cout << endl;
}


// оптимизация - хранить не всю матрицу, а только 2 строки - текущую и предыдущую
// если нет необходимости восстановить набор используемых предметов
void ver2(const vector<uint> &arr, uint X)
{
	size_t N = arr.size();

	vector<vector<bool>> dp(2, vector<bool>(X + 1, false));

	for(size_t i = 0; i < 2; ++i){
		dp[i][0] = true;
	}

	// Проход по виртуальной матрице из N строк
	for(int i = 1; i <= N; ++i){

		for(int j = 1; j <= X; ++j){

			//  Virtual table
			//                  j:  0   1   2   ... X
			//  Real     [  i = 0 |___|___|___|_______| <--- placed at i = 0
			//  part     [  i = 1 |___|___|___|_______|     \  <--- placed at i = 1
			//                                              |      \
			//  Virtual                                     |      |
			//  part        i = 2 |___|___|___|_______|    /       |    prev is i = 1
			//              i = 3 |___|___|___|_______|           /     prev is i = 0
			//              i = 4 |___|___|___|_______|  placed at i = 0, prev is i = 1 
			//              i = 5 |___|___|___|_______|  placed at i = 1, prev is i = 0 
			//               ...

			int prev_row = (i - 1) % 2;
			int curr_row = i % 2;

			if(dp[prev_row][j]){
				dp[curr_row][j] = true;
			}
			else{
				int col = j - arr[i - 1];

				if(col >= 0 && dp[prev_row][col]){
					dp[curr_row][j] = true;
				}
			}
		}
	}

	// Получаение результата
	uint max_total_weight = 0;
	for(size_t j = 1; j <= X; ++j){
		if(dp[1][j]){
			max_total_weight = j;
		}
	}

	cout << "v2: max_total_weight: " << max_total_weight << endl;
}

// Версия, где у каждого элемента есть вес и ценность
void ver3(const vector<uint> &weights, const vector<uint> costs, uint X)
{
	// кольцо:    вес 1кг, ценность 1500
	// ожерелье:  вес 2кг, ценность 2100
	// серьги:    вес 3кг, ценность 3500
	//
	// Рюкзак вместимостью Х = 3кг
	//
	//                     j ->
	//                  i    |  0кг  |  1кг  |  2кг  |  3кг  |
	//                  |    ---------------------------------
	//    ничего (0кг)  v  0 |   0   |   0   |   0   |   0   |
	//    кольцо (1кг)     1 |   0   |  1500 |  1500 |  1500 |
	//  ожерелье (2кг)     2 |   0   |  1500 |  2100 |  3600 | (2100 + 1500 > 1500)
	//    серьги (3кг)     3 |   0   |  1500 |  2100 |  3600 | (3600 > 3500)
	//                                                   ^
	//                                                   |
	//            Ответ - максимально возможная общая ценность

	size_t N = weights.size();

	// содержит максимальную ценность из i элементов для рюкзака вместимостью j
	vector<vector<uint>> dp(N + 1, vector<uint>(X + 1, 0));

	for(int i = 1; i <= N; ++i){

		for(int j = 1; j <= X; ++j){

			uint prev_cost = dp[i - 1][j];

			uint curr_with_prev_cost = 0;

			int col = j - weights[i - 1];
			if(col >= 0){
				// можем взять текущий предмет и что-то из предыдущих (в том числе 0)
				curr_with_prev_cost = costs[i - 1] + dp[i - 1][col];
			}
			 
			dp[i][j] = max(prev_cost, curr_with_prev_cost);
		}

	}

	cout << "v3: max_total_cost: " << dp[N][X] << endl;
}



int main()
{

	vector<uint> arr{1, 4, 2, 7, 5, 3, 4, 10};
	uint X = 26;

	ver1(arr, X);

	ver2(arr, X);

	vector<uint> weights{1, 2, 3};
	vector<uint> costs{1500, 2100, 3500};
	ver3(weights, costs, 3);

	return 0;
}