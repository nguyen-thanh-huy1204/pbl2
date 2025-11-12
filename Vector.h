#ifndef VECTOR_H
#define VECTOR_H


#include <stdexcept>
template <typename T>
class Vector {
private:
    T* data;
    int _size;
    int _capacity;

    void ensure_capacity(int minCap) {
        if (_capacity >= minCap) return;
        int newCap = _capacity ? _capacity * 2 : 4;
        while (newCap < minCap) newCap *= 2;
        T* newData = new T[newCap];
        for (int i = 0; i < _size; ++i) newData[i] = data[i];
        delete[] data;
        data = newData;
        _capacity = newCap;
    }

public:
    Vector(): data(nullptr), _size(0), _capacity(0) {}

    // Copy constructor
    Vector(const Vector& other): data(nullptr), _size(other._size), _capacity(other._capacity) {
        if (_capacity > 0) {
            data = new T[_capacity];
            for (int i = 0; i < _size; ++i) data[i] = other.data[i];
        }
    }

    // Move constructor
    Vector(Vector&& other) noexcept: data(other.data), _size(other._size), _capacity(other._capacity) {
        other.data = nullptr; other._size = 0; other._capacity = 0;
    }

    // Copy assignment (copy-and-swap)
    Vector& operator=(Vector other) {
        swap(other);
        return *this;
    }

    // Destructor
    ~Vector() { delete[] data; }

    void swap(Vector& other) noexcept {
        std::swap(data, other.data);
        std::swap(_size, other._size);
        std::swap(_capacity, other._capacity);
    }

    void push_back(const T& v) {
        ensure_capacity(_size + 1);
        data[_size++] = v;
    }

    void pop_back() { if (_size > 0) --_size; }
    int size() const { return _size; }
    bool empty() const { return _size == 0; }
    void clear() { _size = 0; }

    T& operator[](int idx) { if (idx < 0 || idx >= _size) throw std::out_of_range("Vector index"); return data[idx]; }
    const T& operator[](int idx) const { if (idx < 0 || idx >= _size) throw std::out_of_range("Vector index"); return data[idx]; }

    int findIndex(const T& val) const {
        for (int i = 0; i < _size; ++i) if (data[i] == val) return i;
        return -1;
    }

    void erase(int idx) {
        if (idx < 0 || idx >= _size) return;
        for (int i = idx; i < _size - 1; ++i) data[i] = data[i+1];
        --_size;
    }
};

#endif
