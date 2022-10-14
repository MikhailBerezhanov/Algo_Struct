#include <iostream>
#include "list.hpp"

using namespace std;


// Задача: дана голова односвязного списка, в каждом 
// элементе которого записано целое положительное число. 
// Нужно удалить все узлы с нечётными значениями.


// struct ListNode
// {
// 	ListNode(int value_ = 0, ListNode *next_ = nullptr): value(value_), next(next_) {}
// 	int value = 0;
// 	ListNode *next = nullptr;

// 	static void print(const ListNode *head){
// 		const ListNode *curr_node = head;

// 		if( !curr_node ){
// 			cout << "null";
// 		}

// 		while(curr_node){
// 			cout << curr_node->value << " -> ";
// 			curr_node = curr_node->next;
// 		}
// 		cout << endl;
// 	}
// };


// 1 -> 4 -> 5 -> 6 -> 13 -> 12 -> 11 -> null
//
// 4 -> 6 -> 12 -> null
// ------------------------------------
// 2 -> 3 -> 7 -> null
//
// 2 -> null
//

void remove_next(ListNode *node)
{
	ListNode *next_next = node->next->next;
	node->next = next_next;
}

ListNode* remove_odd_nodes(ListNode *head)
{
	ListNode *new_head = head;
	ListNode *curr_node = head;

	auto is_odd = [](int value) -> bool { return value % 2; };

	while(curr_node){

		if( !curr_node->next ){
			// list end
			break;
		}

		if(is_odd(curr_node->next->value)){
			remove_next(curr_node);
		}
		else{
			curr_node = curr_node->next;
		}
	}

	if(head && is_odd(head->value)){
		new_head = head->next;
	}

	return new_head;
}

ListNode* remove_odd_nodes_v2(ListNode *head)
{
	ListNode fake_head(-1, head);

	ListNode *curr_node = &fake_head;

	while(curr_node && curr_node->next){

		if(curr_node->next->value % 2 == 1){
			remove_next(curr_node);
		}
		else{
			curr_node = curr_node->next;
		}

	}

	return fake_head.next;
}




int main()
{
	// 1 -> 4 -> 5 -> 6 -> 13 -> 12 -> 11 -> null
	//
	// 4 -> 6 -> 12 -> null
	// ------------------------------------
	{
		ListNode n11(11, nullptr);
		ListNode n12(12, &n11);
		ListNode n13(13, &n12);
		ListNode n6(6, &n13);
		ListNode n5(5, &n6);
		ListNode n4(4, &n5);
		ListNode n1(1, &n4);

		ListNode::print(&n1);
		ListNode *new_head = remove_odd_nodes_v2(&n1);
		ListNode::print(new_head);
	}

	// 2 -> 3 -> 4 -> 4 -> 3 -> 8 -> null
	//
	// 2 -> 4 -> 4 -> 8 -> null
	{
		ListNode n8(8, nullptr);
		ListNode n3a(3, &n8);
		ListNode n4a(4, &n3a);
		ListNode n4b(4, &n4a);
		ListNode n3b(3, &n4b);
		ListNode n2(2, &n3b);

		ListNode::print(&n2);
		ListNode *new_head = remove_odd_nodes_v2(&n2);
		ListNode::print(new_head);
	}

	// 8 ->
	//
	// 8 ->
	{
		ListNode n8(8, nullptr);

		ListNode::print(&n8);
		ListNode *new_head = remove_odd_nodes_v2(&n8);
		ListNode::print(new_head);
	}
	
	// 1 ->
	//
	// null
	{
		ListNode n1(1, nullptr);

		ListNode::print(&n1);
		ListNode *new_head = remove_odd_nodes_v2(&n1);
		ListNode::print(new_head);
	}

	return 0;
}