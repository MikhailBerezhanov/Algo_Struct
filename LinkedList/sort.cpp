#include <iostream>
#include "list.hpp"

using namespace std;

// задача: дана голова односвязного списка, 
// нужно поменять узлы местами так, чтобы 
// значения шли в порядке неубывания. 
// Проще говоря, нужно отсортировать список, 
// не копируя элементы

// Когда речь идёт о списках, проще всего реализовать 
// сортировку слиянием или вставками. Но последняя 
// работает за O(N^2), поэтому лучше всего
// выбирать сортировку слиянием.


// 10 -> 3 -> 4 -> 2 -> 1 -> 3 -> 21 -> 5
// |
// curr_node
//
// 10 -> null    3      ->     4   ->  ....
// |             |             |
// new_head   curr_node    head->next
//
// ~ 4 < 10 - chain curr_node with new_head ~
//
// 3       ->    10  -> null   4       ->    2
// |                           |             |
// new_head                curr_node     head->next
//
// search a place to insert curr_node
//
// 3    ->   4  ->  10 -> null      2     ->     1   ->  ...
// |                                |            |
// new_head                     curr_node    head->next
//
// ~ 2 < 3 - chain curr_node with new_head ~
//
// 1 -> 2 -> 3 -> 3 -> 4 -> 10 -> 21
//

ListNode* insert_sort(ListNode *head)
{
	ListNode *new_head = nullptr;

	while(head){

		ListNode *curr_node = head;
		head = head->next; 	// move head before head->next (curr_node->next) will be changed

		if( !new_head || (curr_node->value < new_head->value) ){
			// chain new_head with curr_node
			curr_node->next = new_head;
			new_head = curr_node;
		}
		else{
			// search a place to insert curr_node
			ListNode *start_node = new_head;
			while(start_node->next && (curr_node->value > start_node->next->value)){
				start_node = start_node->next;
			}

			curr_node->next = start_node->next;
			start_node->next = curr_node;
		}
	
	}

	return new_head;
}

// ---------------------------------------------------------------------------

// Find mid element using The Tortoise and The Hare Approach
// https://dev.to/alisabaj/floyd-s-tortoise-and-hare-algorithm-finding-a-cycle-in-a-linked-list-39af#:~:text=The%20idea%20behind%20the%20algorithm,then%20there%20is%20no%20cycle.
ListNode* find_middle(ListNode *head)
{
	ListNode *slow = head;			// turtoise
	ListNode *fast = head->next;	// hare

	// slow moves by 1 element, fast moves by 2 elements
	while(fast && fast->next){
		slow = slow->next;			
		fast = fast->next->next;
	}

	return slow;
}

ListNode* merge(ListNode *left_head, ListNode *right_head)
{
	// cout << "merging\t";
	// ListNode::print(left_head);
	// cout << "with\t";
	// ListNode::print(right_head);

	ListNode *new_head = nullptr;
	ListNode *tmp = new_head;

	auto add_sorted_part = [&](ListNode *node){

		if( !new_head ){
			// cout << "new head: " << node->value << endl;
			node->next = nullptr;
			new_head = node;
			tmp = new_head;
		}
		else{
			// cout << "adding: " << node->value << endl;
			node->next = nullptr;
			tmp->next = node;
			tmp = tmp->next;
		}

	};

	while(left_head && right_head){

		ListNode *node = nullptr;

		if(left_head->value <= right_head->value){
			node = left_head;
			left_head = left_head->next;
		}
		else{
			node = right_head;
			right_head = right_head->next;
		}

		add_sorted_part(node);
	}

	while(left_head){
		ListNode *node = left_head;
		left_head = left_head->next;
		add_sorted_part(node);
	}

	while(right_head){
		ListNode *node = right_head;
		right_head = right_head->next;
		add_sorted_part(node);
	}

	return new_head;
}

ListNode* merge_sort(ListNode *head)
{
	// 0 or 1 element
	if( !head || !head->next ){
		return head;
	}

	// find middle element
	ListNode *mid_node = find_middle(head);

	ListNode *head2 = mid_node->next;
	mid_node->next = nullptr;	// separate left side from the right side

	// left sub-list + right sub-list
	ListNode *left_sorted = merge_sort(head);
	ListNode *right_sorted = merge_sort(head2);

	return merge(left_sorted, right_sorted);
}

// ---------------------------------------------------------------------------

void split_list(ListNode *head, ListNode **left, ListNode **right)
{
	ListNode *slow = head;
	ListNode *fast = head->next;

	while(fast && fast->next){
		slow = slow->next;
		fast = fast->next->next;
	}

	// slow is at the middle element
	*left = head;
	*right = slow->next;
	slow->next = nullptr;
}

ListNode* merge_v2(ListNode *left, ListNode *right)
{
	ListNode *new_head = nullptr;
	ListNode *curr_node = nullptr;

	auto add_node = [&](ListNode *node){

		node->next = nullptr;

		if( !new_head ){
			new_head = node;
			curr_node = new_head;
		}
		else{
			curr_node->next = node;
			curr_node = curr_node->next;
		}

	};

	while(left && right){

		ListNode *to_add = nullptr;

		if(left->value <= right->value){
			to_add = left;
			left = left->next;
		}
		else{
			to_add = right;
			right = right->next;
		}

		add_node(to_add);
	}

	while(left){
		ListNode *to_add = left;
		left = left->next;
		add_node(to_add);
	}

	while(right){
		ListNode *to_add = right;
		right = right->next;
		add_node(to_add);
	}

	return new_head;
}

void merge_sort_v2(ListNode **phead)
{
	ListNode *head = *phead;

	// Base case: 0 or 1 element is sorted
	if(head == nullptr || head->next == nullptr){
		return;
	}

	// Divide input list to two parts - left and right
	// (if size is odd, extra element goes to the left part)
	ListNode *left = nullptr;
	ListNode *right = nullptr;
	split_list(head, &left, &right);

	// Sort parts
	merge_sort_v2(&left);
	merge_sort_v2(&right);

	// Merge sorted parts, update result
	*phead = merge_v2(left, right);
}

// ---------------------------------------------------------------------------

int main()
{
	{
		// 10 -> 3 -> 4 -> 2 -> 1 -> 3 -> 21 -> 5
		ListNode node5(5, nullptr);
		ListNode node21(21, &node5);
		ListNode node3a(3, &node21);
		ListNode node1(1, &node3a);
		ListNode node2(2, &node1);
		ListNode node4(4, &node2);
		ListNode node3b(3, &node4);
		ListNode node10(10, &node3b);

		ListNode::print(&node10);
		ListNode *new_head = insert_sort(&node10);
		ListNode::print(new_head);
	}

	{
		// 10 -> 3
		ListNode node3(3, nullptr);
		ListNode node10(10, &node3);

		ListNode::print(&node10);
		ListNode *new_head = insert_sort(&node10);
		ListNode::print(new_head); 
	}


	{
		// 10 -> 3
		ListNode node3(3, nullptr);
		ListNode head(10, &node3);

		// ListNode::print(&head);
		cout << "Merge sort: ";
		ListNode *new_head = merge_sort(&head);
		ListNode::print(new_head); 
	}

	{
		// 10 -> 3 -> 4 -> 2 -> 1 -> 3 -> 21 -> 5 -> 9
		ListNode node9(9, nullptr);
		ListNode node5(5, &node9);
		ListNode node21(21, &node5);
		ListNode node3a(3, &node21);
		ListNode node1(1, &node3a);
		ListNode node2(2, &node1);
		ListNode node4(4, &node2);
		ListNode node3b(3, &node4);
		ListNode node10(10, &node3b);

		// ListNode::print(&node10);
		cout << "Merge sort: ";
		ListNode *new_head = merge_sort(&node10);
		ListNode::print(new_head);
	}



	{
		// 10 -> 3 -> 4 -> 2 -> 1 -> 3 -> 21 -> 5 -> 9

		ForwardList list;
		list.push_front(9);
		list.push_front(5);
		list.push_front(21);
		list.push_front(3);
		list.push_front(1);
		list.push_front(2);
		list.push_front(4);
		list.push_front(3);
		list.push_front(10);

		cout << "Merge sort v2: ";
		ListNode *head = list.get_head();
		ListNode::print(head);

		merge_sort_v2(&head);

		ListNode::print(head);
	}
	
	return 0;
}
