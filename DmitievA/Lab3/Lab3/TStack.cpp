//#pragma once
//#include "TStack.h"
//#include <algorithm>
//
//template <class T>
//TStack<T>::TStack(int _size) : size(_size), top(0) {
//    if (size <= 0)
//        throw std::invalid_argument("Stack size must be positive");
//    pMem = new T[size];
//}
//
//template <class T>
//TStack<T>::TStack(const TStack<T>& s): size(s.size), top(s.top) {
//    pMem = new T[size];
//    std::copy(s.pMem, s.pMem + top, pMem);
//}
//
//template <class T>
//TStack<T>::~TStack() {
//    delete[] pMem;
//}
//
//template <class T>
//TStack<T>& TStack<T>::operator=(const TStack<T>& s) {
//    if (this == &s) return *this;
//    TStack temp(s);
//    swap(temp);
//    return *this;
//}
//
//template <class T>
//bool TStack<T>::IsEmpty() const {
//    return top == 0;
//}
//
//template <class T>
//bool TStack<T>::IsFull() const {
//    return top == size;
//}
//
//template <class T>
//void TStack<T>::Push(const T& val) {
//    if (IsFull())
//        throw std::overflow_error("Stack overflow");
//    pMem[top++] = val;
//}
//
//template <class T>
//T TStack<T>::Pop() {
//    if (IsEmpty())
//        throw std::underflow_error("Stack underflow");
//    return pMem[--top];
//}
//
//template <class T>
//T TStack<T>::Top() const {
//    if (IsEmpty())
//        throw std::underflow_error("Stack is empty");
//    return pMem[top - 1];
//}