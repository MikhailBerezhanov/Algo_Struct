#include <iostream>

struct ListNode
{
	ListNode(int value_ = 0, ListNode *next_ = nullptr): value(value_), next(next_) {}
	int value = 0;
	ListNode *next = nullptr;

	static void print(const ListNode *head){
		const ListNode *curr_node = head;

		if( !curr_node ){
			std::cout << "null";
		}

		while(curr_node){
			std::cout << curr_node->value << " -> ";
			curr_node = curr_node->next;
		}
		std::cout << std::endl;
	}
};

class ForwardList
{
public:

	ForwardList() = default;
	~ForwardList() { this->clear(); }

	// inserts an element to the beginning
	void push_front(const int &value)
	{
		ListNode *new_node = new ListNode{value, nullptr};

		if( !head ){
			head = new_node;
			return;
		}

		new_node->next = head;
		head = new_node;
	}

	// 
	ListNode* find_node(const int &value)
	{
		ListNode *curr_node = head;

		while(curr_node){
			if(curr_node->value == value){
				return curr_node;
			}
			curr_node = curr_node->next;
		}
	}

	// inserts elements after an element
	void insert_after(ListNode *node, const int &value)
	{
		ListNode *new_node = new ListNode{value, node->next};
		node->next = new_node;
	}

	// reverses the order of the elements
	void reverse()
	{
		ListNode *curr_node = head;
		ListNode *prev_node = nullptr;

		while(curr_node){

			ListNode *node = curr_node;
			curr_node = curr_node->next;

			node->next = prev_node;
			prev_node = node;
		}

		head = prev_node;
	}

	// erases an element after an element
	void erase_after(ListNode *node)
	{
		ListNode *to_delete = node->next;
		if( !to_delete ){
			return;
		}

		node->next = node->next->next;
		delete(to_delete);
	}

	// clears the contents
	void clear()
	{
		ListNode *curr_node = head;

		while(curr_node){
			ListNode *to_delete = curr_node;
			curr_node = curr_node->next;
			delete(to_delete);
		}

		head = nullptr;
	}

	ListNode* get_head() const { return head; }

private:
	ListNode *head = nullptr;

};