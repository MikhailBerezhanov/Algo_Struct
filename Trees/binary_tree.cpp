#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;



struct TreeNode
{
	TreeNode(int val, TreeNode *l, TreeNode *r): value(val), left(l), right(r) {}

	int value = -1;
	TreeNode *left = nullptr;
	TreeNode *right = nullptr;
};


// Visit current node first. than visit it's subtrees
// Used in:
void preorder_traverse(const TreeNode *root)
{
	if( !root ){
		return;
	}

	cout << root->value << " ";
	preorder_traverse(root->left);
	preorder_traverse(root->right);
}

void preorder_traverse_iterative(const TreeNode *root)
{
	if( !root ){
		return;
	}

	stack<const TreeNode*> to_visit;
	const TreeNode *curr_node = root;

	to_visit.push(curr_node);

	while( !to_visit.empty() ){
		curr_node = to_visit.top();
		to_visit.pop();

		cout << curr_node->value << " ";

		// Правый поток заносится раньше, чтобы 
		// левое поддерево обработалось первым (LIFO)
		if(curr_node->right){
			to_visit.push(curr_node->right);
		}

		if(curr_node->left){
			to_visit.push(curr_node->left);
		}
	}
}

// Visit left subtree first, then current node, then right subtree
// Used in: traverse all Binary Search Tree (BST) nodes in non-decreasing order
void inorder_traverse(const TreeNode *root)
{
	if( !root ){
		return;
	}

	inorder_traverse(root->left);
	cout << root->value << " ";
	inorder_traverse(root->right);
}

// Visit all subtrees first, then current node
// Used in:
void postorder_traverse(const TreeNode *root)
{
	if( !root ){
		return;
	}

	postorder_traverse(root->left);
	postorder_traverse(root->right);
	cout << root->value << " ";
}


static void traverse_by_layer(const TreeNode *root, size_t depth, vector<vector<int>> &res)
{
	if( !root ){
		return;
	}

	if(res.size() < (depth + 1)){
		res.push_back(vector<int>());
	} 
	res[depth].push_back(root->value);

	traverse_by_layer(root->left, depth + 1, res);
	traverse_by_layer(root->right, depth + 1, res);
}

void layers_traverse(const TreeNode *root)
{
	// list of lists - values by layer
	vector<vector<int>> vec;

	traverse_by_layer(root, 0, vec);

	for(size_t i = 0; i < vec.size(); ++i){

		cout << "layer[" << i << "]: ";
		for(const auto &elem : vec[i]){ 
			cout << elem << " ";
		}
		cout << endl;
	}
}

void layers_traverse_iterative(const TreeNode *root)
{
	if( !root ){
		return;
	}

	queue<const TreeNode*> to_visit;
	to_visit.push(root);

	while( !to_visit.empty() ){
		const TreeNode *curr_node = to_visit.front();
		to_visit.pop();

		cout << curr_node->value << " ";

		if(curr_node->left){
			to_visit.push(curr_node->left);
		}

		if(curr_node->right){
			to_visit.push(curr_node->right);
		}
	}
}



// Дано: Бинарное дерево со взвешанными узлами
// Найти: Ветку с максимальным весом (максимальную сумму значений в узлах)

static void calc_max_weight(const TreeNode *root, int &max_weight, int curr_weight = 0)
{
	if( !root ){
		return;
	}

	curr_weight += root->value;
	max_weight = std::max(max_weight, curr_weight);

	calc_max_weight(root->left, max_weight, curr_weight);
	calc_max_weight(root->right, max_weight, curr_weight);
}

static int calc_max_weight_v2(const TreeNode *root)
{
	if( !root ){
		return 0; // для пустого узла максимальный вес нулевой
	}

	// Выберем максимальный вес из левого и правого поддеревьев
	// и прибавим текущий вес узла 
	return std::max(calc_max_weight_v2(root->left), calc_max_weight_v2(root->right)) + root->value;
}

int find_max_branch_weight(const TreeNode *root)
{
	int max_weight = 0;

	// calc_max_weight(root, max_weight);

	max_weight = calc_max_weight_v2(root);

	return max_weight;
}


// Переворот дерева относительно корня
void mirror(TreeNode *root)
{
	if( !root ){
		return;
	}

	TreeNode *tmp = root->left;
	root->left = root->right;
	root->right = tmp;

	mirror(root->left);
	mirror(root->right);
}

static int get_max_height(const TreeNode *root, int curr_height = 0)
{
	if( !root ){
		return curr_height;
	}

	int left_height = get_max_height(root->left, curr_height + 1);
	int right_height = get_max_height(root->right, curr_height + 1);

	return std::max(left_height, right_height);
}

void print_tree(const TreeNode *root, int max_height, int curr_height = 0)
{
	// int height = get_max_height(root);

	if( !root ){
		return;
	}

	string padding();

	// cout << 

}


int main(int argc, char* argv[])
{
	//              5
	//           /     \
	//          1       2
	//        /   \   /   \
	//       3     4 7     8
	//      /     /   \   /
	//     9     6    10 11
	//            \
	//            43


	TreeNode node43{43, nullptr, nullptr};
	TreeNode node9{9, nullptr, nullptr};
	TreeNode node6{6, nullptr, &node43};
	TreeNode node10{10, nullptr, nullptr};
	TreeNode node11{11, nullptr, nullptr};

	TreeNode node3{3, &node9, nullptr};
	TreeNode node4{4, &node6, nullptr};
	TreeNode node1{1, &node3, &node4};

	TreeNode node7{7, nullptr, &node10};
	TreeNode node8{8, &node11, nullptr};
	TreeNode node2{2, &node7, &node8};

	TreeNode root{5, &node1, &node2};

	cout << "Max height (5): " << get_max_height(&root) << endl;

	TreeNode root2{5, nullptr, nullptr};
	cout << "Max height (1): " << get_max_height(&root2) << endl;
	TreeNode root3{5, &node7, &node8};
	cout << "Max height (3): " << get_max_height(&root3) << endl;
	cout << "Max height (0): " << get_max_height(nullptr) << endl;

	cout << "Preorder_traverse: ";
	preorder_traverse(&root);
	cout << endl;

	cout << "Preorder_traverse_iterative: ";
	preorder_traverse_iterative(&root);
	cout << endl;

	cout << "Inorder_traverse: ";
	inorder_traverse(&root);
	cout << endl;

	cout << "Postorder_traverse: ";
	postorder_traverse(&root);
	cout << endl;

	cout << "Layers_travers:\n";
	layers_traverse(&root);
	cout << endl;

	cout << "Layers_travers_iterative:\n";
	layers_traverse_iterative(&root);
	cout << endl;

	cout << "Max weight: " << find_max_branch_weight(&root) << endl;

	mirror(&root);
	cout << "Preorder_traverse after mirroring: ";
	preorder_traverse(&root);
	cout << endl;

	return 0;
}
