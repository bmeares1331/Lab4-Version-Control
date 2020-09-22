#include "Huffman.h"

// constructor, initialize class variables and pointers here if need.
Huffman::Huffman(){
  // your code here
  string  hello = "Hi my name is becca";
}

//deconstructor,
Huffman::~Huffman(){
}

shared_ptr<freq_info> Huffman::CreateLeaf(char symbol, int count) {
  shared_ptr<freq_info> freq(new freq_info); //initialze new freq info from heap memory
  freq->symbol = symbol; //update symbol value
  freq->count = count; //update count value
  freq->left = NULL; //set left child pointer to NULL
  freq->right = NULL; //set right child pointer to NULL
  freq->is_leaf = true; //new node IS a leaf
  return freq; //return pointer to node
}

shared_ptr<freq_info> Huffman::CombineNodes(shared_ptr<freq_info> left, shared_ptr<freq_info> right) {
  shared_ptr<freq_info> freq(new freq_info); //initialze new freq info from heap memory
  freq->left = left; //set left child pointer to given left
  freq->right = right;  //set right child pointer to given right
  freq->is_leaf = false; //node is NOT leaf, set is_leaft == false
  freq->count = left->count + right->count; //update count 
  return freq; //return pointer to node
}


void Huffman::IncrementLookup(map<char, int>& lookup, char symbol) {
  map<char,int>::iterator it = lookup.find(symbol); //create iterator variable/look up symbol
  if (it != lookup.end()) { //if symbol is found
    lookup[symbol]++; //increment value associated with symbol
  }
  else { //symbol is not found
    lookup[symbol] = 1; //add symbol to the map
  }
}

void Huffman::LoadQueue(const map<char, int>& lookup, tree_queue& q) {
  map<char,int>::const_iterator it; //create iterator variable, pointing at beginning of map
	for (it = lookup.begin(); it != lookup.end(); it++) { //loop through map from beginning to end
    shared_ptr<freq_info> new_node = CreateLeaf(it->first, it->second); //for each position create new node
    q.push(new_node);
  }
}

shared_ptr<freq_info> Huffman::BuildTree(tree_queue& q) {
  while (q.size() != 1) { //loops through priority queu until 1 value is left
    shared_ptr<freq_info> left = q.top(); //sets value to shared pointer of left node
    q.pop(); //removes value from priority queue
    shared_ptr<freq_info> right = q.top(); //sets value to shared pointer of right node
    q.pop(); //removes value from priority queue
    shared_ptr<freq_info> new_node = CombineNodes(left, right); //creates a combined node of left & right
    q.push(new_node); //adds node to priority queue
  } 
  return q.top(); //returns root of tree
}

map<char, string> Huffman::BuildEncodingTable(shared_ptr<freq_info> root) {
  string current = ""; //string to save value of encoded symbol
  map<char, string> lookup; //map to save lookup values
  HelperBuildEncodingTable(root, lookup, current); //encode value 
  return lookup; //return map of encoding table 
}

void Huffman::HelperBuildEncodingTable(shared_ptr<freq_info> root, map<char, string>& encoded, string current) {
  if (root->left) { //if there is a left root, not a root  
    HelperBuildEncodingTable(root->left, encoded, (current + LEFT_CHAR)); //recurse through left side
  }
  if (root->right) { //if there is a right root, not a root 
    HelperBuildEncodingTable(root->right, encoded, (current + RIGHT_CHAR)); //recurse though right side 
  } 
  if (!root->right && !root->left) { //leaf node, assign symbol to value/add to map
    encoded[root->symbol] = current; //add value to encoded map 
  }
}

// Encode returns an encoded string using the provided encoding lookup
// table on the input string.
string Huffman::Encode(map<char, string> enc_table, string input) {
  string encode = ""; //string to store encoded values
  for (unsigned int i = 0; i < input.length(); i++) { //loop through input
    encode += enc_table[input[i]];  //update value with encoded letter
  }
  return encode; //return string
}

// Decode returns a decoded string using the provided Huffman codec
// tree and input string.
string Huffman::Decode(shared_ptr<freq_info> root, string input) {
  string decode = ""; //string to hold decoded string
  shared_ptr<freq_info> current = root; //current pointer for node locaation in tree 
  for (unsigned int i = 0; i < input.length(); i++) {
    if (input[i] == '.') { //if the first character is a .; go down left-side of tree
      current = current->left; //update current to right child
    } 
    else { //if the first character is ^; go down right-side of tree
      current = current->right; //update current to right child 
    } 
    if (!current->left && !current->right) { //a leaf node has been found
      decode += current->symbol; //add the letter to the decoded string
      current = root; //return to root for next letter
    }
  }
  return decode; //return string
}