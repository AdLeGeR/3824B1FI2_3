#pragma once
#include <iostream>

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Шаблон вектора
template <class ValType>
class TVector
{
protected:
    int Size;       // размер вектора
    int StartIndex; // индекс первого элемента вектора
    ValType* pVector;
public:
    TVector(int s = 10, int si = 0);
    TVector(const TVector& v);                // конструктор копирования
    ~TVector();
    int GetSize() { return Size; } // размер вектора
    int GetStartIndex() { return StartIndex; } // индекс первого элемента
    ValType& operator[](int pos);             // доступ
    bool operator==(const TVector& v) const;  // сравнение
    bool operator!=(const TVector& v) const;  // сравнение
    TVector& operator=(const TVector& v);     // присваивание

    // скалярные операции
    TVector  operator+(const ValType& val);   // прибавить скаляр
    TVector  operator-(const ValType& val);   // вычесть скаляр
    TVector  operator*(const ValType& val);   // умножить на скаляр

    // векторные операции
    TVector  operator+(const TVector& v);     // сложение
    TVector  operator-(const TVector& v);     // вычитание
    ValType  operator*(const TVector& v);     // скалярное произведение

    // ввод-вывод

    friend std::istream& operator>>(std::istream& in, TVector& v) {
        for (int i = 0; i < v.Size-v.StartIndex; i++) in >> v.pVector[i];
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TVector& v) {
        for (int i = 0; i < v.StartIndex; i++) {
            out << ValType{} << (i +1 == v.Size ? "" : " ");
		}
        for (int i = 0; i < v.Size-v.StartIndex; i++) {
            out << v.pVector[i] << (i + 1 == v.Size-v.StartIndex ? "" : " ");
        }
        return out;
    }
};


template <class ValType>
TVector<ValType>::TVector(int s, int si) : Size(s), StartIndex(si), pVector(nullptr) {
    if (s < 0 || s > MAX_VECTOR_SIZE) throw std::out_of_range("invalid size");
    if (si < 0) throw std::out_of_range("invalid start index");
    if (Size > 0) pVector = new ValType[Size-StartIndex]();
}


template <class ValType>
TVector<ValType>::TVector(const TVector& v) : Size(v.Size), StartIndex(v.StartIndex) {
    pVector = (Size > 0) ? new ValType[Size] : nullptr;
    for (int i = 0; i < Size - StartIndex; i++) pVector[i] = v.pVector[i];
}

template <class ValType>
TVector<ValType>::~TVector() {
    delete[] pVector;
}

template <class ValType>
ValType& TVector<ValType>::operator[](int pos) {
    if (pos < StartIndex || pos >= StartIndex + Size)
        throw std::out_of_range("index out of range");
    return pVector[pos - StartIndex];
}

template <class ValType>
bool TVector<ValType>::operator==(const TVector& v) const {
    if (Size != v.Size || StartIndex != v.StartIndex) return false;
    for (int i = 0; i < Size - StartIndex; i++)
        if (!(pVector[i] == v.pVector[i])) return false;
    return true;
}

template <class ValType>
bool TVector<ValType>::operator!=(const TVector& v) const {
    return !(*this == v);
}

template <class ValType>
TVector<ValType>& TVector<ValType>::operator=(const TVector& v) {
    if (this == &v) return *this;
    if (Size != v.Size) {
        delete[] pVector;
        Size = v.Size;
        pVector = (Size > 0) ? new ValType[Size] : nullptr;
    }
    StartIndex = v.StartIndex;
    for (int i = 0; i < Size - StartIndex; i++) pVector[i] = v.pVector[i];
    return *this;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const ValType& val) {
    TVector<ValType> res(Size, StartIndex);
    for (int i = 0; i < Size - StartIndex; i++) res.pVector[i] = pVector[i] + val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const ValType& val) {
    TVector<ValType> res(Size, StartIndex);
    for (int i = 0; i < Size - StartIndex; i++) res.pVector[i] = pVector[i] - val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator*(const ValType& val) {
    TVector<ValType> res(Size, StartIndex);
    for (int i = 0; i < Size - StartIndex; i++) res.pVector[i] = pVector[i] * val;
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator+(const TVector& v) {
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("Sizes and the start indexes do not match");
    TVector<ValType> res(Size, StartIndex);
    for (int i = 0; i < Size - StartIndex; i++) res.pVector[i] = pVector[i] + v.pVector[i];
    return res;
}

template <class ValType>
TVector<ValType> TVector<ValType>::operator-(const TVector& v) {
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("Sizes and the start indexes do not match");
    TVector<ValType> res(Size, StartIndex);
    for (int i = 0; i < Size - StartIndex; i++) res.pVector[i] = pVector[i] - v.pVector[i];
    return res;
}

template <class ValType>
ValType TVector<ValType>::operator*(const TVector& v) {
    if (Size != v.Size || StartIndex != v.StartIndex)
        throw std::invalid_argument("Sizes and the start indexes do not match");
    ValType sum{};
    for (int i = 0; i < Size - StartIndex; i++) sum = sum + (pVector[i] * v.pVector[i]);
    return sum;
}


template <class ValType>
class TMatrix : public TVector<TVector<ValType>>
{
public:
    TMatrix(int s = 10);
    TMatrix(const TMatrix& mt);                    // копирование
    TMatrix(const TVector<TVector<ValType>>& mt);  // преобразование типа
    bool operator==(const TMatrix& mt) const;      // сравнение
    bool operator!=(const TMatrix& mt) const;      // сравнение
    TMatrix& operator= (const TMatrix& mt);        // присваивание
    TMatrix  operator+ (const TMatrix& mt);        // сложение
    TMatrix  operator- (const TMatrix& mt);        // вычитание

    // ввод / вывод
    friend std::istream& operator>>(std::istream& in, TMatrix& mt) {
        for (int i = 0; i < mt.Size; i++) {
            in >> mt.pVector[i];
        }
        return in;
    }
    friend std::ostream& operator<<(std::ostream& out, const TMatrix& mt) {
        int n = mt.Size;
        for (int i = 0; i < n; i++) {
            out << mt.pVector[i] << (i + 1 == mt.Size ? "" : "\n");
        }
        return out;
    }
};

template <class ValType>
TMatrix<ValType>::TMatrix(int s) : TVector<TVector<ValType>>(s, 0) {
    if (s < 0 || s > MAX_MATRIX_SIZE) throw std::out_of_range("invalid size");
    for (int i = 0; i < s; i++) {
        this->pVector[i] = TVector<ValType>(s, i);
    }
}

template <class ValType>
TMatrix<ValType>::TMatrix(const TMatrix& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
TMatrix<ValType>::TMatrix(const TVector<TVector<ValType>>& mt) : TVector<TVector<ValType>>(mt) {}

template <class ValType>
bool TMatrix<ValType>::operator==(const TMatrix& mt) const {
    return TVector<TVector<ValType>>::operator==(mt);
}

template <class ValType>
bool TMatrix<ValType>::operator!=(const TMatrix& mt) const {
    return !(*this == mt);
}

template <class ValType>
TMatrix<ValType>& TMatrix<ValType>::operator=(const TMatrix& mt) {
    TVector<TVector<ValType>>::operator=(mt);
    return *this;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator+(const TMatrix& mt) {
    if (this->Size != mt.Size) throw std::invalid_argument("Sizes and the start indexes do not match");
    TMatrix<ValType> res(this->Size);
    for (int i = 0; i < this->Size; i++) res.pVector[i] = this->pVector[i] + mt.pVector[i];
    return res;
}

template <class ValType>
TMatrix<ValType> TMatrix<ValType>::operator-(const TMatrix& mt) {
    if (this->Size != mt.Size) throw std::invalid_argument("Sizes and the start indexes do not match");
    TMatrix<ValType> res(this->Size);
    for (int i = 0; i < this->Size; i++) res.pVector[i] = this->pVector[i] - mt.pVector[i];
    return res;
}

//template <class ValType>
//std::istream& operator>>(std::istream& in, TMatrix<ValType>& mt)
//
//template <class ValType>
//std::ostream& operator<<(std::ostream& out, const TMatrix<ValType>& mt)
