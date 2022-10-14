#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm> // std::max(), std::max_element()

//          v  v  v                 v
// Input: [ 2, 3, 6, 4, 1, 3, 5, 4, 7 ]
//          ^  ^     ^        ^     ^
// Answer: [2, 3, 4, 5, 7] - length is 5
size_t max_increasing_subsequence(const std::vector<int> &sequence)
{
	if(sequence.empty()){
		return 0;
	}

	// 1. What does dp[] contain? 
	// dp[i] holds max subsequence length ends at element 'i' 

	// 2. What is the base case?
	// first element is the increasing subsequence too, it has length of 1

	// 3. How to obtain dp[i] value, knowning dp[i - 1] result?
	// Check every 'j' element of dp[], before 'i'. 
	// Choose MAX dp[j] such as (sequence[j] < sequence[i])
	// Get this dp[j] value and increment it by 1, cuz 'i' element increases total length.
	// If no such dp[j] found (every dp[j] >= dp[i]), then dp[i] = 1

	// 4. In which ordred will dp[] be filled? 
	// Startiing from i = 1, increasing by 1, to the last element (sequence.size)

	// 5. Where is final answer stored?
	// Answer is the MAX value of dp[]

	std::vector<size_t> dp(sequence.size(), 1);

	// Time complexity: O( N^2 )
	for(size_t i = 1; i < sequence.size(); ++i){

		// check every dp[j < i]
		for(size_t j = 0; j < i; ++j){
			if(sequence[j] < sequence[i]){
				// update dp[i] on each step
				dp[i] = std::max(dp[i], dp[j] + 1);
			}
		}
	}

	return *std::max_element(dp.cbegin(), dp.cend());
}


int main()
{
	{
		std::vector<int> vec{2, 3, 6, 4, 1, 3, 5, 4, 7};
		assert(5 == max_increasing_subsequence(vec) && printf("OK\n") );
	}
	
	{
		std::vector<int> vec;
		std::cout << max_increasing_subsequence(vec) << std::endl;
	}

	return 0;
}