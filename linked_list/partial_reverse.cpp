#include "solution.h"

using namespace std;

/** Comment it before submitting
struct Node {
    int val;
    Node* next;
    Node(int val_, Node* next_) {
        val = val_;
        next = next_;
    }
};
**/


bool find_start_node(Node *head, int from, Node **start, Node **before_start)
{
    *before_start = nullptr;
    *start = head;

    while(from != 1){

        if( !(*start) ){
            return false;
        }

        *before_start = *start;
        *start = (*start)->next;
        --from;
    }

    return true;
}

//                left     right
//                 |         |
//            1 -> 2 -> 3 -> 4 -> 5
//           /     |    |    |     \  
// before_start  start  |   last  after_last
//                    curr
//
//
// 1    4 -> 3 -> 2    5
//
// 1 -> 4 -> 3 -> 2 -> 5
//
Node* Reverse(Node* head, int left, int right) 
{
    if(left == right){
        return head;
    }

    Node *start = nullptr;
    Node *before_start = nullptr;

    Node *after_last = nullptr;
    Node *last = head;

    if( !find_start_node(head, left, &start, &before_start) ){
        // left is greater than list size
        return head;
    }

    // Move to the element after start
    Node *prev = start;
    Node *curr = start->next;

    int cnt = 0;
    while((cnt < right - left) && curr){

        Node *tmp = curr;
        curr = curr->next;

        if(cnt == right - left - 1){
            after_last = tmp->next;
            last = tmp;
        }

        tmp->next = prev;
        prev = tmp;

        ++cnt;
    }
    
    start->next = after_last;

    if(before_start){
        before_start->next = last;
        return head;
    }

    return last;
}
