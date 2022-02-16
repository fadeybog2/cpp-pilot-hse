#include "zip.h"

Zipped::Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end)
    : a_begin_(a_begin), a_end_(a_end), b_begin_(b_begin), b_end_(b_end) {
}

Zipped::ZipIterator Zipped::begin() {
    return ZipIterator(a_begin_, b_begin_);
}

Zipped::ZipIterator Zipped::end() {
    return ZipIterator(a_end_, b_end_);
}

ZippedPair Zipped::ZipIterator::operator*() const {
    return ZippedPair(*it_a_, *it_b_);
}

bool Zipped::ZipIterator::operator!=(const ZipIterator& other) {
    return it_a_ != other.it_a_ && it_b_ != other.it_b_;
}

Zipped::ZipIterator::ZipIterator(Iterator it1, Iterator it2) : it_a_(it1), it_b_(it2) {
}

void Zipped::ZipIterator::operator++() {
    ++it_a_;
    ++it_b_;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
}
