#pragma once
#include <stdexcept>
#include <algorithm>


template <class T>
class TStack {
private:
    T* pMem;
    int capacity;      // максимальный размер
    int size;       // индекс верхнего элемента / количество элементов в стеке

    void swap(TStack& other) {
        std::swap(pMem, other.pMem);
        std::swap(capacity, other.capacity);
        std::swap(size, other.size);
    }

public:
    TStack(int capacity_ = 100);
    TStack(const TStack<T>& s);
    ~TStack();

    TStack<T>& operator=(const TStack<T>& s);

    bool IsEmpty() const;
    bool IsFull()  const;

    void Push(const T& val); // положить в стек
    T Pop();              // снять со стека
    T Top() const;        // посмотреть вершину
	size_t Size() const { return size; }
};



template <class T>
TStack<T>::TStack(int capacity_) : capacity(capacity_), size(0) {
    if (capacity <= 0)
        throw std::out_of_range("Stack capacity must be > 0");
    pMem = new T[capacity];
}

template <class T>
TStack<T>::TStack(const TStack<T>& s) : capacity(s.capacity), size(s.size) {
    pMem = new T[capacity];
    std::copy(s.pMem, s.pMem + size, pMem);
}

template <class T>
TStack<T>::~TStack() {
    delete[] pMem;
}

template <class T>
TStack<T>& TStack<T>::operator=(const TStack<T>& s) {
    if (this == &s) return *this;
    TStack temp(s);
    swap(temp);
    return *this;
}

template <class T>
bool TStack<T>::IsEmpty() const {
    return size == 0;
}

template <class T>
bool TStack<T>::IsFull() const {
    return size == capacity;
}

template <class T>
void TStack<T>::Push(const T& val) {
    if (IsFull())
        throw std::overflow_error("Stack overflow");
    pMem[size++] = val;
}

template <class T>
T TStack<T>::Pop() {
    if (IsEmpty())
        throw std::underflow_error("Stack underflow");
    return pMem[--size];
}

template <class T>
T TStack<T>::Top() const {
    if (IsEmpty())
        throw std::underflow_error("Stack is empty");
    return pMem[size - 1];
}