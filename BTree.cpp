#include "BTree.h"

// constructor, initialize class variables and pointers here if need.
BTree::BTree(){
  // Your code here
}

//deconstructor,
BTree::~BTree(){
}

// insert adds the given key into a b-tree rooted at 'root'.  If the
// key is already contained in the btree this should do nothing.
// 
// On exit: 
// -- the 'root' pointer should refer to the root of the
//    tree. (the root may change when we insert or remove)
// -- the btree pointed to by 'root' is valid.
void BTree::insert(shared_ptr<btree> root, int key){

}


void BTree::remove(shared_ptr<btree> root, int key){
	/*shared_ptr<btree> location = find(root, key); // find the root where key is located 
    // verify key to be removed is present in node returned
    bool found = false;
    int i; 
    for (i = 0; i < location->num_keys; i++) { //loop through keys in node
    	if (location->keys[i] == key) { // key found
    		found = true; 
    		break;
    	}
    }
    if (found == false) { //key not found  
    	return; //break and don't do anything
    }
    // the key to be removed is present in this node
    //if the node is a leaf node; call remove leaf function
    if (location->is_leaf ==  true) {
    	remove_from_leaf(location, i);
    }
    else {
    	remove_from_nonleaf(location, i, key);
    }
    return; */
}

void BTree::remove_from_leaf(shared_ptr<btree> root, int i){ 
    /*// move all keys backward one location, removing key from the node
    for (int j = i + 1; j < root->num_keys; j++) { 
        root->keys[i-1] = root->keys[i]; 
    }
    root->num_keys--; // reduce count of number of keys
    return; */
} 

void BTree::remove_from_nonleaf(shared_ptr<btree> root, int i, int key){ 
	// function to remove the i-th key from this node - which is a non-leaf node  
    return; 
}

shared_ptr<btree> BTree::find(shared_ptr<btree> root, int key){
	int i = 0; // counter to track location in B Tree
	// increment while i < #-vals in node & key value is bigger than position
	while (i < root->num_keys && key > root->keys[i]) { 
		i++;
	}
	if (root->keys[i] == key) { // if key value is found
		return root; // return node that holds the key-val
	}
	if (root->is_leaf == true) { // if it's a leaf
		return root; // return the node where it would be found 
	}
	return find(root->children[i], key); // recurse to increment through the B Tree
}

int BTree::count_nodes(shared_ptr<btree> root){
	int count = 1; // root is present 
	int i; // counter to track location in B Tree
	for (i = 0; i < root->num_keys; i++) { // loop through number of keys in root
		if (root->is_leaf == false) { // if it is not a leaf and in range i
			count += count_nodes(root->children[i]); // recurse through the children of root
		}
	}
	if (root->is_leaf == false) { // loop through subtree of last child
		count += count_nodes(root->children[i]); // recurse through related nodes
	}
	return count; // return count
}

int BTree::count_keys(shared_ptr<btree> root){
	int count = 0; // counter of number of keys 
	int i; // counter to track location in B Tree
	for (i = 0; i < root->num_keys; i++) { // loop through number of keys in root
		count++; // count each key in the node;
		if (root->is_leaf == false) {  // if it is not a leaf and in range i
			count += count_keys(root->children[i]); // recurse through other nodes
		}
	}
	if (root->is_leaf == false) { // loop through subtree of last child 
		count += count_keys(root->children[i]); // recurse through related nodes
	}
	return count; // return count
}
