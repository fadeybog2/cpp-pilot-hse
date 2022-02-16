#pragma once

#include <forward_list>
#include <string>

using Value = std::string;
using Iterator = std::forward_list<Value>::const_iterator;
using ZippedPair = std::pair<const Value&, const Value&>;

class Zipped {
public:
    Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);

    class ZipIterator {
    public:
        ZipIterator(Iterator it1, Iterator it2);
        ZippedPair operator*() const;
        bool operator!=(const ZipIterator& other);
        void operator++();

    private:
        Iterator it_a_;
        Iterator it_b_;
    };
    ZipIterator begin();  // NOLINT
    ZipIterator end();    // NOLINT
private:
    Iterator a_begin_;
    Iterator a_end_;
    Iterator b_begin_;
    Iterator b_end_;
};

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end);
