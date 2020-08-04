#include "deps/catch.h"

#include <chrono>
#include <thread>
#include <algorithm>
#include <functional>

#include "../src/sets.h"
#include "../src/sorters.h"
#include "../src/experiment.h"

TEST_CASE("set can be generated", "[set]") {
    constexpr size_t size = 128;

    SECTION("sorted") {
        auto set = sets::sorted(size);

        REQUIRE(set.size() == size);
        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }

    SECTION("inverted") {
        auto set = sets::inverted(size);

        REQUIRE(set.size() == size);
        REQUIRE(std::is_sorted(set.begin(), set.end(), std::greater<>()));
    }

    SECTION("random") {
        auto set = sets::inverted(size);

        REQUIRE(set.size() == size);
    }
}

TEST_CASE("sort works as expected", "[sorter]") {
    constexpr size_t size = 128;
    auto set = sets::inverted(size);

    SECTION("insertion") {
        sorters::insertion(set.begin(), set.end());

        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }

    SECTION("quick") {
        sorters::quick(set.begin(), set.end());

        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }

    SECTION("heap") {
        sorters::heap(set.begin(), set.end());

        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }

    SECTION("merge") {
        sorters::merge(set.begin(), set.end());

        REQUIRE(std::is_sorted(set.begin(), set.end()));
    }
}

// Straight from https://en.cppreference.com/w/cpp/chrono/duration/abs
template <class Rep, class Period, class = std::enable_if_t<
        std::chrono::duration<Rep, Period>::min() < std::chrono::duration<Rep, Period>::zero()>>
constexpr std::chrono::duration<Rep, Period> abs(std::chrono::duration<Rep, Period> d)
{
    return d >= d.zero() ? d : -d;
}

TEST_CASE("experiment times with reasonably accuracy", "[experiment]") {
    auto duration = std::chrono::milliseconds(100);
    const auto time = experiment([&] { std::this_thread::sleep_for(duration); }).run();

    REQUIRE(abs(duration - time) <= std::chrono::milliseconds(10));
}
