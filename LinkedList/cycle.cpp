#include <iostream>
#include "list.hpp"

using namespace std;

// Turtoise - rabbit approach:
//
//               fs
// 1 -> 2 -> 3 -> 4
//      ^_________/
//
// s    f      
// 1 -> 2 -> 3
// ^_________/
//
//      sf               
// 1 -> 2 -> 3 -> 4 -> 5
//           ^_________/
// s    f
// 1 -> null
// 
// s             f
// 1 -> 2 -> 3  null
//
bool has_cycle(const ListNode *head)
{
	if(!head){
		return false;
	}

	ListNode *slow = head;
	ListNode *fast = head->next;

	while(fast && fast->next){

		if(fast == slow){
			return true;
		}

			fast = fast->next->next;

		if(fast == slow){
			return true;
		}

		slow = slow->next;
	}

	return false;
}



int main()
{
	

	return 0;
}