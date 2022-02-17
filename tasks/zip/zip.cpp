#include "zip.h"

Zipped::Zipped(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end)
    : it_a_begin_(a_begin), it_a_end_(a_end), it_b_begin_(b_begin), it_b_end_(b_end) {
}

Zipped::ZipIterator Zipped::begin() const {
    return ZipIterator(it_a_begin_, it_b_begin_);
}

Zipped::ZipIterator Zipped::end() const {
    return ZipIterator(it_a_end_, it_b_end_);
}

ZippedPair Zipped::ZipIterator::operator*() const {
    return ZippedPair(*it_a_, *it_b_);
}

bool Zipped::ZipIterator::operator!=(const ZipIterator& other) const {
    return it_a_ != other.it_a_ && it_b_ != other.it_b_;
}

Zipped::ZipIterator::ZipIterator(Iterator it_a, Iterator it_b) : it_a_(it_a), it_b_(it_b) {
}

void Zipped::ZipIterator::operator++() {
    ++it_a_;
    ++it_b_;
}

Zipped Zip(Iterator a_begin, Iterator a_end, Iterator b_begin, Iterator b_end) {
    return Zipped(a_begin, a_end, b_begin, b_end);
}
