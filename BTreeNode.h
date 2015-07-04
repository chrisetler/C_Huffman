#include <stdlib.h>
#define LEFT_SIDE 0;
#define RIGHT_SIDE 1;
typedef int Type;
typedef struct BTreeNode BTreeNode;

struct BTreeNode {
	Type value;
	BTreeNode *left_child_ptr;
	BTreeNode *right_child_ptr;
	BTreeNode *parent_ptr;
	int hasChild;
	int hasParent;
	int onParentSide;
};

void BN_init(BTreeNode* node) {
	node = (BTreeNode*)malloc(sizeof(BTreeNode));
	node->hasChild = 0;
	node->hasParent = 0;
	node->left_child_ptr = NULL;
	node->right_child_ptr = NULL;
	node->parent_ptr = NULL;
}

//set the value of the node
void BN_setval(BTreeNode* node, Type val) {
	node->value = val;
}

//give the node a left child
void BN_add_left_child(BTreeNode* parent, BTreeNode* child) {
	//remove the old child if it exists
	// if(parent->left_child_ptr != NULL) {
	// 	BTreeNode *old_child = parent->left_child_ptr;
	// 	old_child->hasParent = 0;
	// 	old_child->parent_ptr= NULL;
	// }
	parent->left_child_ptr = child;
	child->parent_ptr = parent;
	parent->hasChild = 1;
	child->hasParent = 1;
	child->onParentSide = LEFT_SIDE;
}

//give the node a right child
void BN_add_right_child(BTreeNode* parent, BTreeNode* child) {
	//remove the old child if it exists
	// if(parent->right_child_ptr != NULL) {
	// 	BTreeNode *old_child = parent->left_child_ptr;
	// 	old_child->hasParent = 0;
	// 	old_child->parent_ptr= NULL;
	// }
	parent->right_child_ptr = child;
	child->parent_ptr = parent;
	parent->hasChild = 1;
	child->hasParent = 1;
	child->onParentSide = RIGHT_SIDE;
}

BTreeNode* BN_get_root_ptr(BTreeNode* node) {
	while(node->hasParent != 0) {
		node = node->parent_ptr;
	}
	return node;
}