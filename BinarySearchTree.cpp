// Implement a Binary Search Tree. Insert, Delete, Preorder traversal, in order traversal, post order traversal! 
#include <stdio.h>
#include <iostream>
#include <cmath> 
using namespace std;

// Anything in dynamic memory or heap is accessed through pointers
// We need to store the new node in heap using new

/* Algorithn to find the kth element in a BST 
Do a preorder traversal. Create a count variable that keeps track of which iteration 
we are on, once we hit that value we need to return the data at that node 
*/

struct Node{
	int data; 
	Node* left;
	Node* right;

	Node(int data) {
		this->data = data;
		left = NULL;
		right = NULL;
	}

	Node(){
		this->data = 0;
		left = NULL;
		right = NULL;	
	}



};

class BinarySearchTree {
	public:
		Node* root;
		BinarySearchTree(){
			root = NULL;
		}
		void insert(int data);
        void preOrderTraversal(Node *rootPtr);
		void postOrderTraversal(Node *rootPtr);
		void inOrderTraversal(Node *rootPtr);
		int kthValue(Node *rootPtr, int count, int k);
		int size(Node *rootPtr);
		Node* removeOutsideRange(Node *rootPtr, int min, int max);
		int calculateLeft(Node * rootPtr);
		Node* getMinNode(Node * rootPtr);
		int numChildren(Node *rootPtr);
		bool isGrandchildren(Node *rootPtr);
		void deleteNode(Node *rootPtr, int value);
		int checkHeight(Node* rootPtr);
	    bool isBalanced(Node * rootPtr);
		Node* getRoot() {
			return root;
		}
		void setRoot(Node* setRoot) {
			root = setRoot;
		}

};		
	/* Create new node and insert data at O(log n) */
void BinarySearchTree::insert(int data) {
	Node *insertNode = new Node(data); // creates a new node 
	Node *rootPtr = NULL; // keeps track of the current node 
	Node *parent = NULL;
	bool didEnter = false;

	if(root == NULL)
		root = insertNode;
	else{
		rootPtr = root;
		while(rootPtr){
			didEnter = true;
			parent = rootPtr;
			if(rootPtr->data > insertNode->data)
				rootPtr = rootPtr->left;
			else if(rootPtr->data < insertNode->data) 
				rootPtr = rootPtr->right; 
			}
	}

	if(didEnter == true){
		if(parent->data > insertNode->data) 
			parent->left = insertNode;
		else if(parent->data < insertNode->data)
			parent->right = insertNode;
	}
}
		
		

/* All BST traversals are O(n)*/
void BinarySearchTree::preOrderTraversal(Node *rootPtr){

	// Base Case 
	if(!rootPtr)
		return;
	cout << rootPtr->data << " ";
	preOrderTraversal(rootPtr->left);
	preOrderTraversal(rootPtr->right);
}

int BinarySearchTree::calculateLeft(Node * rootPtr){
	int count = 0;
	while(rootPtr)
	{
		rootPtr = rootPtr->left;
		count++;
	}
	return count;
}

/* Returns the kth smallest value in a BST */
int BinarySearchTree::kthValue(Node *rootPtr, int size, int k){
	if(k > size || k <= 0){
		return 0;
	}
	for(int i=0; i < size - k; i++){
		rootPtr = rootPtr->left;
	}
	return rootPtr->data;

}

void BinarySearchTree::postOrderTraversal(Node *rootPtr)
{
	if(!rootPtr)
		return;
	postOrderTraversal(rootPtr->left);
	postOrderTraversal(rootPtr->right);
	//cout << "deleted: " << rootPtr->data << endl;
	delete rootPtr;

}

int BinarySearchTree::size(Node *rootPtr)
{
	if(rootPtr==NULL)
		return 0;
	else return(size(rootPtr->left)+ 1 + size(rootPtr->right));

}

void BinarySearchTree::inOrderTraversal(Node *rootPtr)
{
//	cout << "in " << endl;
	if(!rootPtr)
		return;
//x	cout << "out " << endl;
	inOrderTraversal(rootPtr->left);
	cout << rootPtr->data << endl;
	inOrderTraversal(rootPtr->right);
}

/* Do a post order traversal to delete nodes outside of a range */
Node* BinarySearchTree::removeOutsideRange(Node *rootPtr, int min, int max){

	if (rootPtr == NULL) 
		return NULL;
	removeOutsideRange(rootPtr->left, min, max);
	removeOutsideRange(rootPtr->right, min, max);

	if(rootPtr->data < min){
		Node* rightPtr = rootPtr->right;
		delete rootPtr;
		return rightPtr;
	}

	if(rootPtr->data > max){
		Node* leftPtr = rootPtr->left;
		delete rootPtr;
		return leftPtr;
	}

	return rootPtr;
}

/* Algorithm: There are three situations to take care of: 

Case 1: No Children: Just delete the Node
Case 2: Child: Move the child in place of that node
Case 3: children: Last node from the In order traversal of the right subtree will be promoted.

Returns a node to the new tree without the deleted Node.
*/ 
void BinarySearchTree::deleteNode(Node *rootPtr, int value){
	//cout << "value = " << value << endl;
	//Node *rootPtr = newTreePtr;
	//Node *current = rootPtr;
	Node *parent;
	if (rootPtr == NULL)
		return;

	while(rootPtr->data != value){
			parent = rootPtr;
			if(rootPtr->data < value){
					//cout << "right root data: " << rootPtr->data << endl;
					rootPtr = rootPtr->right;
					//cout << "new left root data: " << rootPtr->data << endl;

			}else if (rootPtr->data > value) {
					//cout << "left root data: " << rootPtr->data << endl;
					rootPtr = rootPtr->left;
			}
		}
	bool rightOfParent = false;
	bool leftOfParent = false;
	if(parent->data > rootPtr->data) {
		leftOfParent = true;
	}else if(parent->data < rootPtr->data){
		rightOfParent = true;
	}	


	cout << rootPtr->data << endl;
	// Case 1
	if(numChildren(rootPtr) == 0){
		cout << "deleting : " << rootPtr->data << endl;
		parent->left = NULL;
		parent->right = NULL;
	} // Why does delete rootPtr not work? 
// Case 2 
	if (numChildren(rootPtr) == 1 && rootPtr->right){
		if(rightOfParent == true)
			parent->right = rootPtr->right;
		else 
			parent->left = rootPtr->right;
		delete rootPtr;
		//return temp;
	}
	if(numChildren(rootPtr) == 1 && rootPtr->left){
		if(rightOfParent == true)
			parent->right = rootPtr->left;
		else
			parent->left = rootPtr->left;
		delete rootPtr;
		//return rootPtr;
	}
// Case 3
	if(numChildren(rootPtr) == 2) {
			if(isGrandchildren(rootPtr) == false)
			{
				rootPtr = rootPtr->right;
				delete rootPtr->right;
				//return rootPtr;
			} else {
				Node * minNode;
				minNode = getMinNode(rootPtr->right);
				rootPtr = minNode;
				delete rootPtr;
				//return rootPtr;
			}

	}

// Case 4: if the root we are deleting is the Root Node 
	
}


bool isBalanced(Node* rootPtr){
	Node* leftRoot = rootPtr;
	Node* rightRoot = rootPtr;

	while(leftRoot){
		if(leftRoot && leftRoot->left && leftRoot->right)
			leftRoot = leftRoot->left;
		else if(!leftRoot->left || !leftRoot->right || !leftRoot)
			return false; 
	}

	while(rightRoot){
		if(rightRoot && rightRoot->left && rightRoot->right)
			rightRoot = rightRoot->right;
		else if(!rightRoot->left || !rightRoot->right || !rightRoot)
			return false; 
	}
	return true; 

	
}

int BinarySearchTree::checkHeight(Node* rootPtr){
	if(rootPtr == NULL)
		return 0;

	int leftHeight = checkHeight(rootPtr->left);
	if(leftHeight == -1)
		return -1;

	int rightHeight = checkHeight(rootPtr->right);
	if(rightHeight == -1)
		return -1; 

	int heightDiff = leftHeight - rightHeight;
	
	if(abs(heightDiff) > 1)
		return -1;
	else 
		return fmax(leftHeight,rightHeight) + 1;
}

bool BinarySearchTree::isBalanced(Node * rootPtr){
	if(checkHeight(rootPtr) == -1)
		return false;
	else
		return true;
}


Node* BinarySearchTree::getMinNode(Node * rootPtr){
	while(rootPtr){
		rootPtr = rootPtr->left;
	}
	return rootPtr;

}

int BinarySearchTree::numChildren(Node *rootPtr){
	if(rootPtr-> left == NULL && rootPtr->right == NULL)
			return 0;
	if ( (rootPtr->left == NULL && rootPtr->right) || (rootPtr->right == NULL && rootPtr->left) )
			return 1;
	if (rootPtr->left && rootPtr->right) 
		return 2;

}

bool BinarySearchTree::isGrandchildren(Node *rootPtr){
	if(rootPtr->left->left || rootPtr->left->right || rootPtr->right->left || rootPtr->right->right){
		return true;
	}
	else
		return false;
}

/* Get linker error when there is no return 0 */
int main()
{
	BinarySearchTree tree;
	tree.insert(50);
	tree.insert(20);
	tree.insert(70);
	tree.insert(10);
	tree.insert(30);
	tree.insert(60);
	tree.insert(55);
	tree.insert(65);
	tree.insert(80);
	tree.insert(5);
	tree.insert(15);
	tree.insert(35);
	tree.insert(72);

	//tree.preOrderTraversal(tree.getRoot());
	cout << endl;


	//tree.setRoot(tree.deleteNode(tree.getRoot(),90));
	tree.deleteNode(tree.getRoot(),5);
	tree.preOrderTraversal(tree.getRoot());


	bool balancedVal = tree.isBalanced(tree.getRoot());

	cout << endl << "balance val = " << balancedVal << endl;

	//tree.preOrderTraversal(tree.getRoot());
	



	// int leftSize = tree.calculateLeft(tree.getRoot());
	// cout << "left size = " << leftSize << endl;
	
	// int dataVal = tree.kthValue(tree.getRoot(),leftSize,3);
	// cout << "data value = " << dataVal << endl;


	//tree.inOrderTraversal(tree.getRoot());	
	//BinarySearchTree newTree;

	//Node* newTreePtr = newTree.getRoot(); 
	 
	//tree.setRoot(tree.removeOutsideRange(tree.getRoot(),20,50));
	
	//cout << newTree.getRoot()->right->left->data << endl;
	
	//tree.inOrderTraversal(tree.getRoot());

	//cout << "everywhere" << endl;

	//cout << "value = " << value << endl;
	
	//tree.postOrderTraversal(tree.getRoot());
	//tree.inOrderTraversal(tree.getRoot());
	//tree.insert(40);
	//tree.preOrderTraversal(tree.getRoot());
	//tree.postOrderTraversal(tree.getRoot());
	//tree.preOrderTraversal(tree.getRoot());

	return 0;
}