#ifndef NANODB_TEST_LIB_H_
#define NANODB_TEST_LIB_H_

#include <chrono>
#include <random>
#include <iostream>

namespace nanodb {
    namespace test {

        std::mt19937_64 RandGenerator(std::chrono::system_clock::now().time_since_epoch().count());

        size_t Rand(const size_t l, const size_t r) {
            return RandGenerator() % (r - l + 1) + l;
        }

    }
}

#endif