import yiran.algorithm;
import yiran.std_overloads;

#include <gtest/gtest.h>
#include <vector>
#include <ranges>
#include <algorithm>

TEST(algorithm, quick_sort_1) {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    yiran::algorithm::quick_sort(v);
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(algorithm, quick_sort_0) {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    yiran::algorithm::quick_sort(v.begin(), v.end());
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(algorithm, quick_sort_2) {
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    yiran::algorithm::quick_sort()(v);
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(algorithm, quick_sort_3) {
    using namespace yiran::std_overloads;
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    v | yiran::algorithm::quick_sort();
    EXPECT_TRUE(std::ranges::is_sorted(v));
}

TEST(algorithm, quick_sort_4) {
    using namespace yiran::std_overloads;
    std::vector<int> v{3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    auto sortFn = yiran::algorithm::quick_sort([](int a, int b) { return a > b; });
    v | std::move(sortFn);
    EXPECT_TRUE(std::ranges::is_sorted(v, std::greater{}));
}

TEST(algorithm, for_each_0) {
    using namespace yiran::std_overloads;
    std::vector arr0{1, 2, 3, 4, 5};
    std::vector<int> arr1;
    arr1.reserve(arr0.size());
    arr0 | yiran::algorithm::for_each([&](int i) { arr1.push_back(i); });
    EXPECT_EQ(arr0, arr1);
}

TEST(algorithm, for_each_1) {
    using namespace yiran::std_overloads;
    std::vector arr0{1, 2, 3, 4, 5};
    std::vector<int> arr1;
    arr1.reserve(arr0.size());
    arr0 | yiran::algorithm::for_each([&](const int &i, yiran::algorithm::control_flow_hint &hint) {
        if (i >= 4) {
            hint = yiran::algorithm::control_flow_hint::request_break;
            return;
        }
        arr1.push_back(i);
    });
    const auto ans = std::vector{1, 2, 3};
    EXPECT_EQ(arr1, ans);
}
