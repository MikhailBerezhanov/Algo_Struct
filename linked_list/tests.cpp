#include <iostream>
#include "list.hpp"

using namespace std;

// Tortoise and Hare approach (slow moves by 1 , fast - by 2)
//               ______________
//              /              \
//             v                \
//    1   ->   4   ->   2   ->   5
//    ^        ^
//   slow    fast
//
//             ^                 ^
//           slow              fast
//                      ^^
//                     slow
//                     fast
//
//
bool has_cycle(const ListNode *head)
{
	const ListNode *slow = head;
	const ListNode *fast = head->next;

	while(fast && fast->next){

		if(slow == fast){
			return true;
		}

		slow = slow->next;
		fast = fast->next->next;
	}	

	return false;
}


int main()
{
	{
		ForwardList list;
		// null ->
	
		list.push_front(1);
		list.push_front(2);
		list.push_front(3);
		// 3 -> 2 -> 1 -> null
		cout << "source list: " << endl;
		ListNode::print(list.get_head());
	
		list.insert_after(list.find_node(2), 5);
		list.insert_after(list.find_node(1), 4);
		list.insert_after(list.get_head(), 10);
		// 3 -> 10 -> 2 -> 5 -> 1 -> 4 -> null
		list.push_front(100);
		// 100 -> 3 -> 10 -> 2 -> 5 -> 1 -> 4 -> null
		cout << "insert_after: " << endl;
		ListNode::print(list.get_head());
	
		list.erase_after(list.get_head());
		list.erase_after(list.find_node(2));
		list.erase_after(list.find_node(4));
		// 100 -> 10 -> 2 -> 1 -> 4 -> null
		cout << "erase_after: " << endl;
		ListNode::print(list.get_head());
	
		list.reverse();
		// 4 -> 1 -> 2 -> 10 -> 100 -> null
		cout << "reverse: " << endl;
		ListNode::print(list.get_head());
	}


	{
		//      _______________________
		//     /                       \
		//    v                         \
		//    1   ->   4   ->   2   ->   5
		//

		ListNode node5{5, nullptr};
		ListNode node2{2, &node5};
		ListNode node4{4, &node2};
		ListNode node1{1, &node4};
		node5.next = &node1;

		cout << "has cycle: " << has_cycle(&node1) << endl;

	}




	return 0;
}