// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp)
{
    if(mp < 0) {
        throw "Error: Negative MaxPower"; 
    }
    MaxPower = mp;

}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
	MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
	MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
	return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
	return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
	return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if(this != &s) {
        MaxPower = s.MaxPower;  
        BitField = s.BitField;   
    }
	return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
	return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
	return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
	int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    TSet temp(maxPower);
    temp.BitField = BitField | s.BitField;
    temp.MaxPower = maxPower;
	return temp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet temp(*this);   //создаем копию текущего множества
    temp.InsElem(Elem); //добавляем элемент в копию через готовую функцию
    return temp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet temp(*this);   //создаем копию текущего множества
    temp.DelElem(Elem); //удаляем элемент из копии через готовую функцию
    return temp;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    int maxPower = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    //результат пересечения — это побитовое И двух битовых полей
    TSet temp(maxPower);
    temp.BitField = BitField & s.BitField;
    temp.MaxPower = maxPower;

    return temp;
}

TSet TSet::operator~(void) // дополнение
{
  //дополнение — это побитовое НЕ 
    TSet temp(MaxPower);
    temp.BitField = ~BitField;
    return temp;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int elem;
    while(istr >> elem) {
        if(elem >= 0 && elem < s.GetMaxPower()) {
            s.InsElem(elem);
        }
    }    
  return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << "{";
    int isFirst = 1; 

    for(int i = 0; i < s.GetMaxPower(); i++) {
        if(s.IsMember(i)) {
            if(!isFirst) {
                ostr << ", "; 
            }
            ostr << i;
            isFirst = 0;
        }
    }
    ostr << "}";
	return ostr;
}
