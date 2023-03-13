cc_test(
    name = "hashtable_test",
    size = "small",
    srcs = [
        "hashtable_test.cpp",
    ],
    copts = ["/std:c++17"],
    deps = [
        "//src/utils:hashtable",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "slice_test",
    size = "small",
    srcs = ["slice_test.cpp"],
    copts = ["/std:c++17"],
    deps = [
        "//src/utils:slice",
        "@googletest//:gtest_main",
    ],
)
