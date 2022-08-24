#include <iostream>
#include <vector>

using namespace std;

//           j       i
//           v       v
// arr[] = 1 4 2 3 1 0
//
//  1.
//    1 2 4 3 1 0
//    1 2 3 4 1 0
//    1 2 3 1 4 0
//    1 2 3 1 0 4
//              i
//  2.
//    1 2 1 3 0  4
//    1 2 1 0 3  4
//            i
//  3.
//    1 1 2 0  3 4
//    1 1 0 2  3 4
//          i
//  4.
//    1 0 1  2 3 4
//        i
//  5.
//    0 1  1 2 3 4
//      i
//
// arr[] = 0 1 1 2 3 4
//
void bubble_sort(vector<int> &arr)
{
	for(size_t i = arr.size() - 1; i > 0; --i){

		for(size_t j = 1; j <= i; ++j){

			if(arr[j] < arr[j - 1]){
				int tmp = arr[j];
				arr[j] = arr[j - 1];
				arr[j - 1] = tmp;
			}
		}
	}
}

//     
// arr[] = 1 3 2 5 0
//            
// 1. elem = 3 
//     i,j
//     vv
//   1 3 2 5 0
//
// 2. elem = 2 
//      i,j
//       vv 
//   1 3 2 5 0
//    
//     j i
//     v v 
//   1 3 3 5 0 
//     
//    elem
//     v
//   1 2 3 5 0
//   
// 3. elem = 5
//
// 4. elem = 0
//   1 2 3 5 5
//          ^^
//          i,j
//
//   1 2 3 3 5
//         j
//
//   1 2 2 3 5
//       j
//
//   1 1 2 3 5
//     j
//
//   1 1 2 3 5
//   j
//
//   elem-> 0 1 2 3 5  
//
void insert_sort(vector<int> &arr)
{
	for(size_t i = 1; i < arr.size(); ++i){

		size_t j = i;
		int elem = arr[i];

		while((j > 0) && (elem < arr[j - 1])){
			arr[j] = arr[j - 1];
			--j;
		}
		arr[j] = elem;
	}
}

//     
//      mid (pivot)
//       v
//   1 4 3 2 0 
//
// i - greater than pivot
// j - less or equal pivot
//
// i j       pivot  
//   1 4 0 2  3
//
//   i j
//   1 4 0 2  3
//
//   i   j
//   1 4 0 2  3
//    
//     i   j
//   1 0 4 2  3
//  
//       i j
//   1 0 2 4  3
//
//          pivot
//       i   v
//   1 0 2   3   4
//
size_t partition(vector<int> &arr, size_t left, size_t right)
{
	size_t mid = left + (right - left) / 2;

	size_t last = right - 1;
	size_t i = left - 1;

	std::swap(arr[mid], arr[last]);
	const int &pivot = arr[last];

	for(size_t j = left; j < last; ++j){

		if(arr[j] <= pivot){
			++i;
			std::swap(arr[j], arr[i]);
		}
	}

	std::swap(arr[i + 1], arr[last]);
	return i + 1;
}

// [left, right)
void quick_sort(vector<int> &arr, size_t left, size_t right)
{
	if((right <= left) || (right - left) < 2){
		return;
	}

	size_t pivot_idx = partition(arr, left, right);

	quick_sort(arr, left, pivot_idx);
	quick_sort(arr, pivot_idx + 1, right);
}

// [left, right)
//
// left = 0
// right = 6
//      
//    mid = 3 (pivot)          pivot
//        v                      v
//  5 4 3 2 1 10             3 6 5 10
//  l         r              l      r
//
//  5 4 3 2 1 10 - swap      3 6 5 10 - swap
//  l       r                  l r
//
//  1 4 3 2 5 10 - swap      3 5  6 10
//    l   r                    r  l
//
//  1 2 3 4 5 10
//     l,r  
//
//  1 2  3  4 5 10
//    r     l
//
//  pivot
//   v
// 4 5 10
// l   r 
//
// 4 5 10
//  l,r
//
// 4  5  10
// r     l
//
void quick_sort_v2(vector<int> &arr, size_t left, size_t right)
{
	if((right <= left) || (right - left) < 2){
		return;
	}

	size_t mid = left + (right - left) / 2;
	const int &pivot = arr[mid];

	size_t l = left;
	size_t r = right - 1;

	while(l <= r){

		while(arr[l] < pivot){
			++l;
		}

		while(arr[r] > pivot){
			--r;
		}

		if(l <= r){
			std::swap(arr[l], arr[r]);
			++l;
			--r;
		}
	}

	quick_sort_v2(arr, left, r + 1);
	quick_sort_v2(arr, l, right);
}


//      mid
//       v
// 1 4 9 1 10 13 20
//
// sorted parts
// 1 4 9   1 10 13 20
// l       r
// res <- 1
// 
// 1 4 9   1 10 13 20
//   l     r
// res <- 1
//
// 1 4 9   1 10 13 20
//   l        r
// res <- 4 
//
// 1 4 9   1 10 13 20
//     l      r
// res <- 9
//
// 1 10 13 20
//   r
// res <- 10, 13, 20
//
void merge(vector<int> &arr, 
			size_t left_begin, size_t left_end,
			size_t right_begin, size_t right_end)
{
	size_t l = left_begin;
	size_t r = right_begin;

	vector<int> res;

	while(l < left_end && r < right_end){

		if(arr[l] <= arr[r]){
			res.push_back(arr[l]);
			++l;
		}
		else{
			res.push_back(arr[r]);
			++r;
		}
	}

	while(l < left_end){
		res.push_back(arr[l]);
		++l;
	}

	while(r < right_end){
		res.push_back(arr[r]);
		++r;
	}

	// update input array part
	for(size_t i = 0; i < res.size(); ++i){
		arr[left_begin + i] = res[i];
	}

}

void merge_sort(vector<int> &arr, size_t left, size_t right)
{
	if((right <= left) || (right - left) < 2 ){
		return;
	}

	// split array
	size_t mid = left + (right - left) / 2;

	// sort two separate parts
	merge_sort(arr, left, mid);
	merge_sort(arr, mid, right);

	// merge the results
	merge(arr, left, mid, mid, right);
}





void print_array(const vector<int> &arr)
{
	for(const auto &elem : arr){
		cout << elem << " ";
	}
	cout << endl;
}


int main(int argc, char* argv[])
{
	size_t size = 0;
	cin >> size;

	vector<int> arr(size);
	for(size_t i = 0; i < size; ++i){
		cin >> arr[i];
	}

	vector<int> arr_copy(arr);

	bubble_sort(arr_copy);
	cout << "bubble_sort: ";
	print_array(arr_copy);

	arr_copy = arr;
	insert_sort(arr_copy);
	cout << "insert_sort: ";
	print_array(arr_copy);

	arr_copy = arr;
	quick_sort(arr_copy, 0, arr_copy.size());
	cout << "quick_sort: ";
	print_array(arr_copy);

	arr_copy = arr;
	quick_sort_v2(arr_copy, 0, arr_copy.size());
	cout << "quick_sort_v2: ";
	print_array(arr_copy);

	arr_copy = arr;
	merge_sort(arr_copy, 0, arr_copy.size());
	cout << "merge_sort: ";
	print_array(arr_copy);

	return 0;
}