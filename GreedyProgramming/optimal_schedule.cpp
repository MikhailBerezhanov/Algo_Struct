#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;


// есть NN соревнований по спортивному программированию, i-е из них проходит 
// в промежуток времени [a_i, b_i).
// Какое максимальное количество соревнований можно выбрать, чтобы в любой
// момент времени проходило не более одного соревнования из этого набора?

//    Time:
//    6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21  22  23 
// E                          [--------]
// B     [---------]
// A  [-----]
// C                   [------]
// D           [-------------------------------]
// G                                           [------]
// H           [-----------]
// F                                   [-------]
//
// MAX: 5 (A + C + E + F + G)

// start_time , end_time
using duration = std::pair<int, int>;

size_t max_avaliable_participations(std::vector<duration> competitions)
{
	// sort input by the end_time

	std::sort(competitions.begin(), competitions.end(), [](const duration &left, const duration &right){
		return left.second < right.second;
	});

	//
	size_t res = 0;
	int curr_end_time = -1;

	// at each step take competition, that start_time >= current_copetition.end_time
	for(const auto &elem : competitions){

		if(elem.first >= curr_end_time){
			++res;
			curr_end_time = elem.second;
		}
	}

	return res;
}

int main()
{
	{
		vector<duration> test_values{
			{13, 15},
			{7, 10},
			{11, 13},
			{9, 17},
			{17, 19},
			{9, 12},
			{15, 17},
		};

		cout << max_avaliable_participations(test_values) << endl;
	}


	return 0;
}