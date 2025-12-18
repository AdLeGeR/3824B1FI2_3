#include "../../../include/Lab1/tset.h"
#include <stdexcept>

using std::invalid_argument;
using std::istream;
using std::ostream;


TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
    if (mp < 1) throw invalid_argument("Invalid MaxPower");
}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() {
    return BitField;
}

int TSet::GetMaxPower(void) const {
    return MaxPower;
}

void TSet::InsElem(int Elem) {
    BitField.SetBit(Elem);
}

void TSet::DelElem(int Elem) {
    BitField.ClrBit(Elem);
}

int TSet::IsMember(int Elem) const {
    return BitField.GetBit(Elem);
}

int TSet::operator==(const TSet& s) const {
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const {
    return BitField != s.BitField;
}

TSet& TSet::operator=(const TSet& s) {
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

TSet TSet::operator+(int Elem) {
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(int Elem) {
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator+(const TSet& s) {
    if (s.MaxPower != MaxPower) {
        throw std::invalid_argument("This sets belong different universes");
    }
    return TSet(BitField | s.BitField);
}

TSet TSet::operator*(const TSet& s) {
    if (s.MaxPower != MaxPower) {
        throw std::invalid_argument("This sets belong different universes");
    }
    return TSet(BitField & s.BitField);
}

TSet TSet::operator~() {
    return TSet(~BitField);
}

istream& operator>>(std::istream& istr, TSet& bf) {
    return istr >> bf.BitField;
}

ostream& operator<<(std::ostream& ostr, const TSet& bf) {
    return ostr << bf.BitField;
}