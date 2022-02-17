#include "vector.h"

#include <algorithm>

Vector::Vector() {
    size_ = capacity_ = 0;
    data_ = new ValueType[capacity_];
}

Vector::Vector(size_t size) {
    size_ = capacity_ = size;
    data_ = new ValueType[capacity_];
    for (SizeType i = 0; i < size_; ++i) {
        data_[i] = 0;
    }
}

Vector::Vector(std::initializer_list<ValueType> list) {
    size_ = capacity_ = list.size();
    data_ = new ValueType[capacity_];
    SizeType i = 0;
    for (const auto& val : list) {
        data_[i] = val;
        ++i;
    }
}

Vector::Vector(const Vector& other) {
    size_ = capacity_ = other.size_;
    data_ = new ValueType[capacity_];
    for (SizeType i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
}

Vector& Vector::operator=(const Vector& other) {
    size_ = capacity_ = other.size_;
    delete[] data_;
    data_ = new ValueType[capacity_];
    for (SizeType i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
    }
    return *this;
}

Vector::~Vector() {
    delete[] data_;
}

Vector::SizeType Vector::Size() const {
    return size_;
}

Vector::SizeType Vector::Capacity() const {
    return capacity_;
}

const Vector::ValueType* Vector::Data() const {
    return data_;
}

Vector::ValueType& Vector::operator[](size_t position) {
    return data_[position];
}

Vector::ValueType Vector::operator[](size_t position) const {
    return data_[position];
}

bool Vector::operator==(const Vector& other) const {
    for (SizeType i = 0; i < size_; ++i) {
        if (data_[i] != other.data_[i]) {
            return false;
        }
    }
    return true;
}

bool Vector::operator!=(const Vector& other) const {
    return !(*this == other);
}

std::strong_ordering Vector::operator<=>(const Vector& other) const {
    for (SizeType i = 0; i < std::min(size_, other.size_); ++i) {
        if (data_[i] != other.data_[i]) {
            return data_[i] <=> other.data_[i];
        }
    }
    if (size_ != other.size_) {
        return size_ <=> other.size_;
    }
    return std::strong_ordering::equal;
}

void Vector::Reserve(Vector::SizeType new_capacity) {
    if (new_capacity <= capacity_) {
        return;
    }
    ValueType* new_data = new ValueType[new_capacity];
    for (SizeType i = 0; i < size_; ++i) {
        new_data[i] = data_[i];
    }
    delete[] data_;
    capacity_ = new_capacity;
    data_ = new_data;
}

void Vector::Clear() {
    size_ = 0;
}

void Vector::PushBack(const Vector::ValueType& new_element) {
    if (capacity_ == 0) {
        Reserve(1);
    } else if (size_ == capacity_) {
        Reserve(capacity_ * 2);
    }
    data_[size_] = new_element;
    ++size_;
}

void Vector::PopBack() {
    --size_;
}

void Vector::Swap(Vector& other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

Vector::Iterator Vector::Begin() {
    return Vector::Iterator(data_);
}

Vector::Iterator Vector::End() {
    return Vector::Iterator(Begin() + size_);
}

Vector::Iterator Vector::begin() {
    return Begin();
}

Vector::Iterator Vector::end() {
    return End();
}

Vector::Iterator::Iterator(Vector::ValueType* pointer) : ptr_(pointer) {
}

Vector::ValueType& Vector::Iterator::operator*() const {
    return *ptr_;
}

Vector::ValueType* Vector::Iterator::operator->() const {
    return ptr_;
}

Vector::Iterator& Vector::Iterator::operator=(Vector::Iterator other) {
    ptr_ = other.ptr_;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator++() {
    ++ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator++(int) {
    Iterator prev = *this;
    ++ptr_;
    return prev;
}

Vector::Iterator& Vector::Iterator::operator--() {
    --ptr_;
    return *this;
}

Vector::Iterator Vector::Iterator::operator--(int) {
    Iterator prev = *this;
    --ptr_;
    return prev;
}

Vector::Iterator Vector::Iterator::operator+(Vector::DifferenceType shift) {
    Iterator result = *this;
    result += shift;
    return result;
}

Vector::DifferenceType Vector::Iterator::operator-(Vector::Iterator other) {
    return ptr_ - other.ptr_;
}

Vector::Iterator& Vector::Iterator::operator+=(Vector::DifferenceType shift) {
    ptr_ += shift;
    return *this;
}

Vector::Iterator& Vector::Iterator::operator-=(Vector::DifferenceType shift) {
    ptr_ -= shift;
    return *this;
}

bool Vector::Iterator::operator==(const Vector::Iterator& other) const {
    return ptr_ == other.ptr_;
}

bool Vector::Iterator::operator!=(const Vector::Iterator& other) const {
    return ptr_ != other.ptr_;
}

std::strong_ordering Vector::Iterator::operator<=>(const Vector::Iterator& other) const {
    return ptr_ <=> other.ptr_;
}
