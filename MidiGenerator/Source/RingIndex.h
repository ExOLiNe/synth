#pragma once

template<typename type>
class RingIndex {
private:
    size_t len;
public:
    size_t index;
    RingIndex(size_t len, size_t index = 0) : len(len), index(index) {}
    RingIndex& operator++() {
        operator+(1);
        return *this;
    }
    RingIndex& operator--() {
        operator-(1);
        return *this;
    }
    RingIndex& operator-(size_t l) {
        if (index > l) {
            index -= l;
        } else {
            index = len + (index - l);
        }
        return *this;
    }
    RingIndex& operator+(size_t l) {
        index = (index + l) % len;
        return *this;
    }
    RingIndex& operator+=(size_t l) {
        index = (index + l) % len;
        return *this;
    }
    RingIndex& operator-=(size_t l) {
        if (index > l) {
            index -= l;
        } else {
            index = len + (index - l);
        }
        return *this;
    }
    std::weak_ordering operator <=>(size_t l) {
        return index <=> l;
    }
    type operator*() {
        return index;
    }
};