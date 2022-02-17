#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<std::string>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
public:
    Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

    class ZipIterator {
    public:
        ZipIterator(Iterator it_a, Iterator it_b);
        ZippedPair operator*() const;
        bool operator!=(const ZipIterator& other) const;
        void operator++();

    private:
        Iterator it_a_;
        Iterator it_b_;
    };
    ZipIterator begin() const;  // NOLINT
    ZipIterator end() const;    // NOLINT
private:
    Iterator it_a_begin_;
    Iterator it_a_end_;
    Iterator it_b_begin_;
    Iterator it_b_end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
