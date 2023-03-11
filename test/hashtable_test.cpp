#include "../src/utils/hashtable.h"

#include <gtest/gtest.h>
#include <unordered_map>

#include "lib.h"

using nanodb::hash::MurmurHash3_32;
using nanodb::hashtable::HashTable;

TEST(HashTableTest, HashTest) {
  std::string s1("\"Hello, my name is Shirayuki Mizuki.\"");
  ASSERT_EQ(MurmurHash3_32(s1), 2871394218u);

  std::string s2("MurmurHash3 algorithm test.");
  ASSERT_EQ(MurmurHash3_32(s2), 2053471688u);

  std::string s3("");
  ASSERT_EQ(MurmurHash3_32(s3), 3094723104u);
}

TEST(HashTableTest, BasicHashTableTest) {
  HashTable<int, int> table;

  const int k = 10000;

	// std::cerr << "Start Basic HashTable Test" << std::endl;
	// table.PrintData(std::cerr);

  for (int i = 1; i <= k; ++i) table.Insert(i, i + 1);
	// table.PrintData(std::cerr);

  for (int i = 1; i <= k; ++i) {
    ASSERT_TRUE(table.Contains(i));
		ASSERT_FALSE(table.Contains(i + k));
		ASSERT_EQ(table.Get(i), i + 1);
		ASSERT_EQ(table[i], i + 1);
  }

	// table.PrintData(std::cerr);

	for (int i = 1; i <= k; ++i) {
		ASSERT_TRUE(table.Delete(i));
	}

	// table.PrintData(std::cerr);

	for (int i = 1; i <= k; ++i) {
    ASSERT_FALSE(table.Contains(i));
		ASSERT_FALSE(table.Contains(i + k));
  }
}