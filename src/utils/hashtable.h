// TODO: Use std::forward to improve performance.

#ifndef NANODB_UTILS_HASHTABLE_H_
#define NANODB_UTILS_HASHTABLE_H_

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <string>
#include <utility>

#include "hash.h"
#include "lib.h"

namespace nanodb {
namespace hashtable {

static const size_t kMinHashSize = 4;
static const size_t kMaxHashSize = SIZE_MAX;

template <typename KeyType, typename ValueType,
          typename Hash = hash::HashTrait<KeyType>,
          typename Equal = hash::EqualTrait<ValueType>>
class HashTable {
 private:
  class Node {
   private:
    uint64_t hash_;

    KeyType key_;

    ValueType value_;

    Node* next_;

   public:
    Node(const uint64_t hash, const KeyType& key, const ValueType& value,
         Node* next = nullptr)
        : hash_(hash), key_(key), value_(value), next_(next) {}

    ~Node() {}

    uint64_t hash() const noexcept { return hash_; }

    KeyType key() const noexcept { return key_; }

    ValueType value() const noexcept { return value_; }

    ValueType& value_ref() noexcept { return value_; }

    Node* next() const noexcept { return next_; }

    Node*& next_ref() noexcept { return next_; }
  };

  size_t size_;

  size_t count_;

  bool auto_expand_;

  bool auto_shrink_;

  Node** head_;

 public:
  HashTable(const size_t size = kMinHashSize, const bool auto_expand = true,
            const bool auto_shrink = true)
      : size_(size),
        count_(0),
        auto_expand_(auto_expand),
        auto_shrink_(auto_shrink),
        head_(new Node*[size]) {
    for (size_t i = 0; i < size; ++i) {
      head_[i] = nullptr;
    }
  }

  ~HashTable() {
    for (size_t i = 0; i < size_; ++i) {
      Node* current = head_[i];
      while (current) {
        Node* next = current->next();
        delete current;
        current = next;
      }
    }
    delete[] head_;
  }

  void PrintData(std::ostream& out, const size_t max_line_print = 8,
                 const size_t max_node_print = 8) {
    Assert(max_line_print >= 1,
           "nanodb::hashtable::HashTable::PrintData failed: max_line_print "
           "should be >= 1.");
    Assert(max_node_print >= 1,
           "nanodb::hashtable::HashTable::PrintData failed: max_node_print "
           "should be >= 1.");
    out << "-> HashTable Data: size(" << size() << "), count(" << count()
        << ")\n";
    for (size_t i = 0; i < size_; ++i) {
      if (i == max_line_print) {
        out << "    ......" << std::endl;
        return;
      }
      Node* current = head_[i];
      out << "    head(" << i << "): ";
      for (size_t node_count = 0; current; current = current->next()) {
        if (node_count != 0) {
          out << "-->";
        }
        out << "(hash: " << current->hash() << ", key: " << current->key()
            << ", value: " << current->value() << ")";
        if (++node_count; node_count == max_node_print) {
          break;
        }
      }
      out << std::endl;
    }
  }

  HashTable& operator=(const HashTable& rhs) = delete;

  HashTable& operator=(HashTable&& rhs) {
    std::swap(size_, rhs.size_);
    std::swap(count_, rhs.count_);
    std::swap(auto_expand_, rhs.auto_expand_);
    std::swap(auto_shrink_, rhs.auto_shrink_);
    std::swap(head_, rhs.head_);
    return *this;
  }

  // The size of hash list.
  size_t size() const noexcept { return size_; }

  // The count of elements in hash table.
  size_t count() const noexcept { return count_; }

  bool TryExpand() {
    if (auto_expand() && size() < count() && size() < kMaxHashSize) {
      Resize(std::min(size() << 1, kMaxHashSize));
      return true;
    }
    return false;
  }

  bool auto_expand() const noexcept { return auto_expand_; }

  void set_auto_expand(const bool auto_expand = true) {
    auto_expand_ = auto_expand;
  }

  bool TryShrink() {
    if (auto_shrink() && size() > count() * 4 && size() > kMinHashSize) {
      Resize(std::max(size() >> 1, kMinHashSize));
      return true;
    }
    return false;
  }

  bool auto_shrink() const noexcept { return auto_shrink_; }

  void set_auto_shrink(const bool auto_shrink = true) {
    auto_shrink_ = auto_shrink;
    TryShrink();
  }

  bool Contains(const KeyType& key) {
    const uint64_t hash = Hash()(key);
    const uint64_t pos = hash % size();
    for (Node* i = head_[pos]; i; i = i->next()) {
      if (i->hash() == hash && Equal()(i->key(), key)) {
        return true;
      }
    }
    return false;
  }

  ValueType& operator[](const KeyType& key) {
    // TODO
    const uint64_t hash = Hash()(key);
    uint64_t pos = hash % size();
    for (Node* i = head_[pos]; i; i = i->next()) {
      if (i->hash() == hash && Equal()(i->key(), key)) {
        return i->value_ref();
      }
    }

    ValueType* empty_value =
        reinterpret_cast<ValueType*>(operator new(sizeof(ValueType)));
    Node* current = new Node(hash, key, *empty_value, head_[pos]);
    head_[pos] = current;
    ++count_;

    if (TryExpand()) {
      pos = hash % size();
      for (Node* i = head_[pos]; i; i = i->next()) {
        if (i->hash() == hash && Equal()(i->key(), key)) {
          return i->value_ref();
        }
      }
    }
    return current->value_ref();
  }

  void Insert(const KeyType& key, const ValueType& value) {
    (*this)[key] = value;
  }

  bool Delete(const KeyType& key) {
    const uint64_t hash = Hash()(key);
    const uint64_t pos = hash % size();

    Node* previous = nullptr;
    Node* current = head_[pos];

    while (current) {
      if (current->hash() == hash && Equal()(current->key(), key)) {
        if (previous) {
          previous->next_ref() = current->next();
        } else {
          head_[pos] = current->next();
        }
        delete current;
        --count_;
        TryShrink();
        return true;
      }
      previous = current;
      current = current->next();
    }
    return false;
  }

  ValueType Get(const KeyType& key) { return (*this)[key]; }

  void Resize(const size_t size) {
    Assert(size >= kMinHashSize,
           "nanodb::hashtable::HashTable::Resize failed: size should be >= "
           "kMinHashSize.");
    HashTable<KeyType, ValueType> new_table(size, false, false);
    for (size_t i = 0; i < size_; ++i) {
      Node* current = head_[i];
      while (current) {
        new_table.Insert(current->key(), current->value());
        current = current->next();
      }
    }
    new_table.set_auto_expand(auto_expand());
    new_table.set_auto_shrink(auto_shrink());
    *this = std::move(new_table);
  }
};

}  // namespace hashtable
}  // namespace nanodb

#endif