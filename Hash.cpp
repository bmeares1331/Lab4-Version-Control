#include "Hash.h"

// implemented for you - don't change this one
unsigned int DJB2(std::string key){
  unsigned int hash = 5381;
  // Leaving the debugging stuff commented out, in case you want to
  // play. It will output the hash at each incremental step so you can
  // see how it takes shape.
  //
  //  cout << "Hashing string \"" << key << "\"" << endl;
  //  cout << bitset<32>(hash) << " " << hash << endl;
  for (size_t i=0; i < key.length(); i++) {
    char c = key[i]; 
    hash = ((hash << 5) + hash) + c;
    // cout << bitset<32>(hash) << " " << hash << endl;
  }
  // cout << "Hashed string \"" << key << "\" to " << hash << endl;
  //
  // for light reading on why djb2 might not be the best
  // production-grade hash function, go to
  // http://dmytry.blogspot.com/2009/11/horrible-hashes.html
  return hash;
}

// implemented for you - don't change this one
unsigned int ModuloBucketFunc(unsigned int hashcode, unsigned int cap){
  unsigned int b = hashcode % cap;
  return b;
}

// constructor, initialize class variables and pointers here if need.
Hash::Hash(){
}

//deconstructor,
Hash::~Hash(){
}

shared_ptr<hash_table> Hash::InitTable(unsigned int cap){
  shared_ptr<hash_table> ret(new hash_table);
  ret->capacity = cap; 
  ret->size = 0;
  ret->occupied = 0;
  shared_ptr<htable> tableptr(new htable(cap));
  ret->table = tableptr; 
  ret->hash_func = DJB2;
  ret->bucket_func = ModuloBucketFunc;
  return ret;
}

shared_ptr<hash_node> Hash::InitNode(std::string key, unsigned int hashcode, std::string val){
  shared_ptr<hash_node> ret(new hash_node);
  ret->key = key;
  ret->hashcode = hashcode;
  ret->value = val;
  ret->deleted = false;
  return ret;
}


bool Hash::SetKVP(shared_ptr<hash_table> tbl, std::string key, std::string value){ //key is a STRING
  unsigned int hash = tbl->hash_func(key);//convert string to hash value 
  unsigned int hash_num = tbl->bucket_func(hash, tbl->capacity); //convert a hash value to bucket position
  shared_ptr<hash_node> node(new hash_node); 
  node = InitNode(key, hash, value);  
  if (tbl->capacity == tbl->occupied) {
    return false; 
  }
  if (!tbl->table->at(hash_num)) { //empty position 
    tbl->occupied++;
    tbl->size++;
    tbl->table->at(hash_num) = node; 
    return true;
  }
  string current_key = tbl->table->at(hash_num)->key;  
  if (current_key == key) { //matching key is already in the table
      tbl->table->at(hash_num) = node;
  }
  else { //position full but does not match current key 
    while (tbl->table->at(hash_num) && current_key != key) {
      hash_num++; // find first open spot 
      if (hash_num >= tbl->capacity) {
          hash_num = 0;
      } 
      if (!tbl->table->at(hash_num)) { 
        tbl->table->at(hash_num) = node;
        tbl->occupied++;
        tbl->size++;
        return true;
      } 
    }
  }
  return true;
}

float Hash::Load(shared_ptr<hash_table> tbl){
  return float(tbl->occupied)/tbl->capacity;
}

std::string Hash::GetVal(shared_ptr<hash_table> tbl, std::string key){
  string value = ""; 
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    if (!tbl->table->at(i)) {
      continue;
    } 
    string current_key = tbl->table->at(i)->key;
    if (current_key == key) {
      value = tbl->table->at(i)->value; 
    }
  }
  return value;
}

bool Hash::Contains(shared_ptr<hash_table> tbl, std::string key){
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    if (!tbl->table->at(i)) {
      continue;
    } 
    string current_key = tbl->table->at(i)->key;
    if (current_key == key && !tbl->table->at(i)->deleted) {
      return true;
    } 
  }
  return false;
}

bool Hash::Remove(shared_ptr<hash_table> tbl, std::string key){
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    if (!tbl->table->at(i)) {
      continue;
    }
    string current_key = tbl->table->at(i)->key;
    if (current_key == key) {
      tbl->table->at(i)->deleted = true; 
      tbl->size--;
      return true;
    }
  }
  return false;
}

// Resize creates a new underlying backing array (tbl->table) with the
// provided new capacity, rehashes the existing backing array into the
// new array. on exit, the hash table's fields and functions
// accurately reflect the hash table's current state.
//
// this is an extra-credit function.
void Hash::Resize(shared_ptr<hash_table> tbl, unsigned int new_capacity){
  shared_ptr<hash_table> new_tbl = InitTable(new_capacity);
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    if (!tbl->table->at(i)) {
      continue;
    }
    if (!tbl->table->at(i)->deleted) { //not being deleted, need  to rehash
      string current_key = tbl->table->at(i)->key;
      string current_value = tbl->table->at(i)->value;
      SetKVP(new_tbl, current_key, current_value);
    }
  }
  tbl->table = new_tbl->table;
  tbl->capacity = new_capacity;
}

// implemented for you - feel free to change this one if you like
void Hash::PrintTable(shared_ptr<hash_table> tbl){
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl->capacity << endl;
  cout << "  size:     " << tbl->size << endl;
  cout << "  occupied: " << tbl->occupied << endl;
  cout << "  load:     " << Load(tbl) << endl;
  if (tbl->capacity < 130) {
    for (unsigned int i=0; i < tbl->capacity; i++) {
      cout << "[" << i << "]    ";
      if (!tbl->table->at(i)) {
        cout << "<empty>" << endl;
      } else if (tbl->table->at(i)->deleted) {
        cout << "<deleted>" << endl;
      } else {
        cout << "\"" << tbl->table->at(i)->key << "\" = \"" << tbl->table->at(i)->value << "\"" << endl;
      }
    }
  } else {
    cout << "    <hashtable too big to print out>" << endl;
  }
}

