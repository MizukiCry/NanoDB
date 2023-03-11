#include <bits/stdc++.h>

#include "src/utils/hashtable.h"

// struct Node {
//   Node* next_;

//   Node(Node* next = nullptr) : next_(next) { puts("Inited Node"); }

//   ~Node() {
//     delete next_;
//     puts("Delete Node");
//   }
// };

// struct List {
//   size_t size_;
//   Node** head_;

//   List(const size_t size = 1) : size_(size), head_(new Node*[size]) {
//     for (size_t i = 0; i < size; ++i) {
//       head_[i] = new Node(new Node());
//     }
//     puts("Inited List");
//   }

//   ~List() {
//     //for (size_t i = 0; i < size_; ++i) head_[i]->~Node();
//     delete[] head_;
//     puts("Delete List");
//   }
// };

int main() {
  // List l;
  nanodb::hashtable::HashTable<int, int> h1, h2;
  h1 = std::move(h2);
  puts("End");
  return 0;
}