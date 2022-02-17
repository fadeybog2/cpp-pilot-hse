#pragma once

#include <compare>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>

class Vector {
public:
    using ValueType = int;
    using SizeType = size_t;
    using DifferenceType = ptrdiff_t;

    class Iterator {
    public:
        explicit Iterator(ValueType* pointer);
        Iterator();

        ValueType& operator*() const;
        ValueType* operator->() const;

        Iterator& operator=(Iterator other);

        Iterator& operator++();
        Iterator operator++(int);
        Iterator& operator--();
        Iterator operator--(int);

        Iterator operator+(DifferenceType shift);
        DifferenceType operator-(Iterator other);
        Iterator& operator+=(DifferenceType shift);
        Iterator& operator-=(DifferenceType shift);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
        std::strong_ordering operator<=>(const Iterator& other) const;
    };

    Vector();
    explicit Vector(size_t size);
    Vector(std::initializer_list<ValueType> list);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);

    ~Vector();

    SizeType Size() const;
    SizeType Capacity() const;

    const ValueType* Data() const;

    ValueType& operator[](size_t position);
    ValueType operator[](size_t position) const;

    bool operator==(const Vector& other) const;
    bool operator!=(const Vector& other) const;
    std::strong_ordering operator<=>(const Vector& other) const;

    void Reserve(SizeType new_capacity);
    void Clear();

    void PushBack(const ValueType& new_element);
    void PopBack();

    void Swap(Vector& other);

    Iterator Begin();
    Iterator End();

    Iterator begin();  // NOLINT
    Iterator end();    // NOLINT
};
