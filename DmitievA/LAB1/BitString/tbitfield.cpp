#include "../../../include/Lab1/tbitfield.h"
#include <stdexcept>

using std::copy;
using std::ostream;
using std::istream;
using std::out_of_range;
using std::invalid_argument;
using std::string;
using std::max;

using std::cout;
void print_bits(TELEM n) {
    TELEM mask = 1 << sizeof(TELEM) * 8-1;
    for (int i = 0; i < sizeof(TELEM)*8; i++) {
        cout << (bool)(n & mask);
        mask = mask >> 1;
    }
    cout << ' ';
    //int mask = 1 << sizeof(TELEM) * 8 - 1;
    //for (int i = 0; i < sizeof(TELEM) * 8; i++) {
    //    cout << (bool)(n & mask);
    //    mask = mask >> 1;
    //}
}

const int BitsInMem = sizeof(TELEM) * 8;

int TBitField::GetMemIndex(int n) const {
    return n / BitsInMem;
}

TELEM TBitField::GetMemMask(int n) const {
    return (TELEM)(1 << (n % BitsInMem));
}

TBitField::TBitField(int len) {
    if (len < 1) throw invalid_argument("incorrect length");
    BitLen = len;
    MemLen = (len + BitsInMem - 1) / BitsInMem; //округление вверх
    pMem = new TELEM[MemLen]();
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) {
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    copy(bf.pMem, bf.pMem + MemLen, pMem);
}

TBitField::~TBitField() {
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetLength(void) const {
    return BitLen;
}

void TBitField::SetBit(const int n) {
    if (n < 0 || n >= BitLen) 
        throw out_of_range("SetBit");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) {
    if (n < 0 || n >= BitLen) 
        throw out_of_range("ClrBit");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}
int  TBitField::GetBit(const int n) const {
    if (n < 0 || n >= BitLen) 
        throw out_of_range("GetBit");
    return (bool)(pMem[GetMemIndex(n)] & GetMemMask(n));
}
int TBitField::operator==(const TBitField& bf) const {
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const {
    return ~(*this == bf);
}

TBitField& TBitField::operator=(const TBitField& bf) {
    if (bf.BitLen != BitLen) {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
    }
    copy(bf.pMem, bf.pMem + MemLen, pMem);
    return *this;
}

TBitField  TBitField::operator|(const TBitField& bf) {
    const TBitField& LargerTBitField = bf.BitLen > this->BitLen ? bf : *this;
    TBitField result(LargerTBitField.BitLen);
    int i = 0;
    for (; i < this->MemLen + bf.MemLen - LargerTBitField.MemLen; i++) {
        result.pMem[i] = this->pMem[i] | bf.pMem[i];
    }
    for (; i < LargerTBitField.MemLen; i++) {
        result.pMem[i] = LargerTBitField.pMem[i];
    }
    return result;
}

TBitField  TBitField::operator&(const TBitField& bf) {

    const TBitField& LargerTBitField = bf.BitLen > this->BitLen ? bf : *this;
    TBitField result(LargerTBitField.BitLen);
    int i = 0;
    for (; i < this->MemLen + bf.MemLen - LargerTBitField.MemLen; i++) {
        result.pMem[i] = this->pMem[i] & bf.pMem[i];
    }
    for (; i < LargerTBitField.MemLen; i++) {
        result.pMem[i] = 0;
    }
    return result;
}

TBitField TBitField::operator~() {
    TBitField result(*this);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = ~result.pMem[i];
    }
    int extra = MemLen * BitsInMem - BitLen;
    if (extra) {                            
        TELEM mask = (~TELEM(0)) >> extra;
        result.pMem[MemLen - 1] &= mask;
    }
    return result;
}

istream& operator>>(istream& in, TBitField& bf) {
    string s;
    in >> s;
    for (int i = 0; i < bf.BitLen && i < int(s.size()); i++)
        if (s[i] == '1') bf.SetBit(i);
        else bf.ClrBit(i);
    return in;
}

ostream& operator<<(ostream& out, const TBitField& bf) {
    for (int i = 0; i < bf.BitLen; i++)
        out << (char)('0'+((bool)bf.GetBit(i)));
    return out;
}