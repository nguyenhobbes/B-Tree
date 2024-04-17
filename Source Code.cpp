// Source:  Geeksforgeeks

#include <iostream>
using namespace std;

// Create a Struct Node
struct Node {
	int* key; // An array of keys
	Node** child; // An array of child pointers
	int numberofkeys; // Current number of keys 
	bool leaf; // True when node is leaf, otherwise false
};

// Prototype
Node* newNode(int degree, bool checkleaf);
void search(Node* root, int x);
Node* insert(Node* root, int degree, int k);
Node* insertNonFull(Node* root, int degree, int k);
Node* splitChild(Node* root, Node* a, int i, int degree);
void display(Node* root);

int main() {
	Node* root = nullptr; // Initialize root as empty 

	// Initialize a array with values
	int arr[] = { 10, 20, 50, 60, 70, 80, 90, 100 };
	int size = sizeof(arr) / sizeof(arr[0]); // Size of array

	// Insert elements of array in the root
	for (int i = 0; i < size; i++) {
		root = insert(root, 3, arr[i]);
	}

	cout << "----- Source: Geeksforgeeks -----" << endl << endl;
	int select;
	do {
		cout << "\n1. Search\n2. Insert\n3. Display\n0. Exit\nSelection: "; cin >> select;
		if (select == 1) { // Search
			int x;
			cout << "Tree: ";
			/*for (int i = 0; i < size; i++) {
				cout << arr[i] << " ";
			}
			cout << endl;*/
			display(root);
			cout << endl;
			cout << "Key : "; cin >> x;
			search(root, x);
		}
		if (select == 2) { // Insert
			int k;
			cout << "Tree: ";
			display(root);
			cout << endl;
			cout << "key: "; cin >> k;
			insert(root, 3, k);
			cout << "After inserting: ";
			display(root);
		}
		if (select == 3) { // Display
			cout << "\nOutput: ";
			display(root); // The function prints keys in the root
			cout << endl;
		}
	} while (select != 0);
	
	return 0;
}

// Create a new Node
Node* newNode(int degree, bool checkleaf) {
	Node* temp = new Node; 
	temp->leaf = checkleaf; // distinguish this node is leaf or not leaf

	// Allocate memory for maximum number of possible keysand child pointers
	temp->key = new int[2 * degree - 1];
	temp->child = new Node * [2 * degree];
	temp->numberofkeys = 0; // Initialize the number of keys as 0
	return temp;
}


// Search
void search(Node* root, int x) {
	int i = 0;

	// Using loops to find position i that key >= x
	while (i < root->numberofkeys && root->key[i] < x) {
		i++;
	}
	if (root->key[i] == x) { // key == x, x exist
		cout << x << " is existed" << endl;
		return;
	}
	if (root->leaf) { // Can not find x in root
		cout << x << " is not existed" << endl;
		return;
	}
	search(root->child[i], x); // Go to the appropriate child
	return;
}

// Insertion
Node* insert(Node* root, int degree, int k) {
	// Create a new root when root is nullptr
	if (!root) {
		root = newNode(degree, true); // Create degree = 3 and bool leaf = true
		root->key[0] = k;
		root->numberofkeys = 1; // Current number of keys in root;
	}
	else { // root is not nullptr
		if (root->numberofkeys == 2 * degree - 1) { // number of keys in root is maximum
			Node* s = newNode(degree, false); // Create a new Node with bool leaf = false
			s->child[0] = splitChild(s, root, 0, degree); // Split old root and grow in height

			// Now node s is having one key and two children
			// Find appropriate child to fill new value k
			int i = 0; 
			if (s->key[0] < k) i = 1;
			s->child[i] = insertNonFull(s->child[i], degree, k); // i is appropriate position to fill k
			root = s; // Change root and s is current root;
		}
		else { // number of keys in root is not full and continues finding an appropriate position to fill k
			root = insertNonFull(root, degree, k);
		}
	}
	return root;
}

// If root is not full, this function will execute
Node* insertNonFull(Node* node, int degree, int k) {
	int i = node->numberofkeys - 1; // Initialize index
	if (node->leaf) { // if node is leaf

		// Compare k to keys in node->key
		// If k < key, that key will be moved to right
		while (i >= 0 && node->key[i] > k) {
			node->key[i + 1] = node->key[i];
			i--;
		}
		node->key[i + 1] = k; // fill k in found position
		node->numberofkeys++; // Update current number of keys 
	}
	else { // node is not leaf

		// This loops is the same as line 72
		// Find appropriate position to fill k
		while (i >= 0 && node->key[i] > k) i--; 
		if (node->child[i + 1]->numberofkeys == 2 * degree - 1) { // If this child is full
			node->child[i + 1] = splitChild(node, node->child[i + 1], i + 1, degree); // Split this child 
			if (node->key[i + 1] < k) i++; // Find appropriate position to fill k
		}
		node->child[i + 1] = insertNonFull(node->child[i + 1], degree, k); // Fill k in found position
	}
	return node;
}

Node* splitChild(Node* node, Node* a, int i, int degree) {

	// Create Node b to store (degree - 1) keys of Node a with bool leaf = a->leaf
	Node* b = newNode(degree, a->leaf); 
	b->numberofkeys = degree - 1; 
	for (int j = 0; j < degree - 1; j++) { // Move the last (degree - 1) keys of a to b
		b->key[j] = a->key[j + degree];  
	}
	if (!a->leaf) { 

		// This loops is the same as line 98 
		// If a is not leaf, we will move the last (degree) children of a to b
		for (int j = 0; j < degree; j++) {
			b->child[j] = a->child[j + degree];
		}
	}
	a->numberofkeys = degree - 1; // Update number of keys in a

	// Move children to the end until an appropriate child is found 
	// Link this child to node
	for (int j = node->numberofkeys; j >= i + 1; j--) {
		node->child[j + 1] = node->child[j];
	}
	node->child[i + 1] = b; // Link this child to b

	// Move all keys which have position greater i to one unit per time
	for (int j = node->numberofkeys - 1; j >= i; j--) {
		node->key[j + 1] = node->key[j];
	}
	node->key[i] = a->key[degree - 1]; // Copy the middle key of a to this node
	node->numberofkeys++; // Update number of keys
	return a;
}

// There are n keys and (n + 1) children
// Traverse through n keys and first n children
void display(Node* root) {
	int i = 0;
	for (i = 0; i < root->numberofkeys; i++) {

		// If node is not leaf, the function "display" will be called again
		// Traverse the subtree rooted with child[i]
		if (!root->leaf) display(root->child[i]);  
		cout << root->key[i] << " "; // Until node is leaf, the recursion stops and prints out data
	}

	// After traversing through n keys and first n children 
	// Traver the last child
	// Call recursive function like line 135
	if (!root->leaf) display(root->child[i]);
}

// Source:  GeeksforGeeks