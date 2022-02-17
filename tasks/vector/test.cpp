#include <catch.hpp>

#include "vector.h"

#include <vector>

namespace {

using StdVector = std::vector<Vector::ValueType>;

void RequireEquality(const Vector& actual, const StdVector& expected) {
    REQUIRE(actual.Size() == expected.size());
    for (size_t i = 0; i < actual.Size(); ++i) {
        REQUIRE(actual[i] == expected[i]);
    }
}
}  // namespace

TEST_CASE("Constructors") {
    {
        Vector a;
        REQUIRE(a.Size() == 0u);
        REQUIRE(a.Capacity() == 0u);
    }
    {
        Vector a{1, 2, 3, 4};
        RequireEquality(a, StdVector{1, 2, 3, 4});
    }
    {
        Vector a(5);
        RequireEquality(a, StdVector(5));
    }
}

TEST_CASE("Basic methods") {
    Vector a{1, 3, 5};
    REQUIRE(a.Capacity() == 3u);
    RequireEquality(a, StdVector{1, 3, 5});

    a.PopBack();
    RequireEquality(a, StdVector{1, 3});
    a.Clear();
    REQUIRE(a.Capacity() == 3u);
    RequireEquality(a, StdVector());

    a.PushBack(6);
    RequireEquality(a, StdVector{6});

    Vector b{3, 4};

    auto* a_data = a.Data();
    auto* b_data = b.Data();

    a.Swap(b);
    RequireEquality(a, StdVector{3, 4});
    RequireEquality(b, StdVector{6});

    REQUIRE(a_data == b.Data());
    REQUIRE(b_data == a.Data());
}

TEST_CASE("Modifications with []") {
    Vector a{3, 7, 8};
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    RequireEquality(a, StdVector{1, 2, 3});
}

TEST_CASE("Iterators 1") {
    Vector a(5);
    for (size_t i = 0; i < a.Size(); ++i) {
        a[i] = i;
    }
    {
        Vector::ValueType cur = 0;
        for (auto elem : a) {
            REQUIRE(cur++ == elem);
        }
    }

    {
        auto first = a.Begin();
        auto last = a.End();
        REQUIRE(static_cast<Vector::SizeType>(last - first) == a.Size());
        REQUIRE(*first == 0);

        REQUIRE(first == a.Begin());
        REQUIRE(first != last);
        REQUIRE(first < last);
        REQUIRE(first <= last);
        REQUIRE(last > first);
        REQUIRE(last >= first);

        ++first;
        REQUIRE(*first == 1);
        auto tmp = first++;
        REQUIRE(*tmp == 1);
        REQUIRE(*first == 2);
        auto end = last--;
        REQUIRE(end == a.End());
        REQUIRE(*last == 4);
        --last;
        REQUIRE((first += 1) == last);
        last -= 3;
        REQUIRE(*last == 0);
    }

    {
        auto first = a.Begin();
        int size = static_cast<int>(a.Size());
        for (int i = 0; i < size; ++i) {
            for (int diff = -2; diff <= 2; ++diff) {
                if (i + diff >= 0 && i + diff < size) {
                    REQUIRE(*(first + i + diff) == i + diff);
                }
            }
        }
    }
}

TEST_CASE("Iterators 2") {
    Vector a{1, 3, 5};
    *(a.begin().operator->()) = 2;
    *((--a.end()).operator->()) = 4;

    RequireEquality(a, StdVector{2, 3, 4});

    Vector::Iterator it;
    it = a.Begin() + 1;
    REQUIRE(*it == 3);
}

TEST_CASE("Reallocation") {
    const int steps = 16;
    Vector data;
    for (int step = 0; step <= steps; ++step) {
        StdVector ok_data;
        for (Vector::ValueType i = 0; i <= (1 << step); ++i) {
            data.PushBack(i);
            ok_data.push_back(i);
        }
        RequireEquality(data, ok_data);
        REQUIRE(data.Capacity() == (1u << (step + 1)));
        data.Clear();
    }
}

TEST_CASE("Comparison") {
    REQUIRE(Vector({1, 2, 3}) == Vector({1, 2, 3}));
    REQUIRE(Vector({1, 2, 3}) != Vector({2, 3, 4}));

    REQUIRE(Vector({1, 2, 3}) < Vector({2, 3, 4}));
    REQUIRE(Vector({1, 2, 3}) < Vector({1, 2, 3, 4}));
    REQUIRE(Vector({1, 2, 3, 4}) <= Vector({2}));
    REQUIRE(Vector({1, 2, 3, 4}) > Vector());
    REQUIRE(Vector({1, 2, 3, 4}) >= Vector({1}));
}

TEST_CASE("Reserve") {
    Vector a;
    a.Reserve(5);
    REQUIRE(a.Size() == 0u);
    REQUIRE(a.Capacity() == 5u);

    for (Vector::ValueType i = 0; i < 5; ++i) {
        a.PushBack(i);
    }

    REQUIRE(a.Capacity() == 5u);
    a.PushBack(4);
    REQUIRE(a.Capacity() == 10u);
    a.Reserve(3);
    REQUIRE(a.Capacity() == 10u);
    a.Clear();
    REQUIRE(a.Size() == 0u);
    REQUIRE(a.Capacity() == 10u);
}
