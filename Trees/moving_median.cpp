#include <iostream>
#include <set>		// BST: set, multiset, map, multimap
#include <vector>

using namespace std;

// Задача. Дан массив целых чисел длины N и число K. 
// Для каждого окна длины K найдите его медиану.


class MedianHandler
{
public:

	void insert(int element)
	{
		// lower.crbegin = lower.max

		if(lower.empty() || element <= *lower.crbegin()){
			lower.insert(element);
		}
		else{
			upper.insert(element);
		}
	}

	void erase(int element)
	{
		auto it = lower.find(element);

		if(it != lower.end()){
			lower.erase(it);
		}
		else{
			it = upper.find(element);
			upper.erase(it);
		}
	}

	double get_median()
	{
		this->rebalance();

		if(lower.size() != upper.size()){
			return *lower.rbegin(); // lower.max
		}

		return (*lower.rbegin() + *upper.begin()) / 2.0;
	}

private:
	// Holds two BST: invariant - lower.max <= upper.min
	std::multiset<int> lower;
	std::multiset<int> upper;

	// Supports invariant lower.size - upper.size = [0, 1]
	void rebalance()
	{
		while(lower.size() < upper.size()){
			// upper.begin = upper.min
			auto it = upper.begin();
			lower.insert(*it);
			upper.erase(it);
		}

		while(lower.size() > upper.size() + 1){
			auto last_it = lower.end();
			--last_it;
			upper.insert(*last_it);
			lower.erase(last_it);
		}

	}
};

// Time complixity: O(N * log K)
//
//
// 
vector<double> get_medians(const vector<int> &vec, int window_size)
{
	MedianHandler median_handler;

	for(size_t i = 0; i < window_size; ++i){
		median_handler.insert(vec.at(i));
	}

	vector<double> res{ median_handler.get_median() };

	for(size_t i = window_size; i < vec.size(); ++i){
		// remove first element of previous step
		median_handler.erase(vec.at(i - window_size));
		// add current element
		median_handler.insert(vec.at(i));
		res.push_back(median_handler.get_median());
	}

	return res;
}


int main()
{
	vector<int> vec{5, 3, 4, 1, 2};
	int K = 3;

	vector<double> medians = get_medians(vec, K);

	for(const auto &elem : medians){
		cout << elem << " ";
	}
	cout << endl;

	return 0;
}