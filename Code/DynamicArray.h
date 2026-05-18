#pragma once
template <typename T>
class DynamicArray {
    T* arr;
    int capacity, size;
public:
    DynamicArray() : capacity(10), size(0) { arr = new T[capacity]; }
    ~DynamicArray() { delete[] arr; }
    void add(T item) {
        if (size < capacity) arr[size++] = item;
    }
};
