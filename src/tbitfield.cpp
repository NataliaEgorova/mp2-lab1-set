// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
    if(len < 0) {
        throw "Error: Negative length";
    }
    BitLen = len;    
    MemLen = (len + BITS_IN_ONE_MEM - 1) / BITS_IN_ONE_MEM;
    pMem = new TELEM[MemLen];
    for(int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen=bf.BitLen;
    MemLen=bf.MemLen;

    pMem = new TELEM[MemLen];

    for(int i=0; i<MemLen; i++){
        pMem[i]=bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem; 
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const //индекс в массиве
{
    if(n < 0 || n >= BitLen) {
        throw "Error: Index out of bounds"; 
    }
    return n / BITS_IN_ONE_MEM;//номер ячейки 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{   
    int pos = n % BITS_IN_ONE_MEM; // находим позицию бита внутри ячейки
    return 1U << pos; // двигаем единицу влево на эту позицию
	
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if((n < 0) || (n >= BitLen)) {
        throw "Index out of bounds";
    }

    int index = GetMemIndex(n);       
    unsigned int mask = GetMemMask(n); 

    pMem[index] = pMem[index] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
    int index = GetMemIndex(n);
    unsigned int mask = GetMemMask(n);
    pMem[index] = pMem[index] & (~mask);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    int index = GetMemIndex(n);
    unsigned int mask = GetMemMask(n);
    return (pMem[index] & mask) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField & bf) // присваивание
{
    if(this != &bf) {
        delete[] pMem;

        BitLen = bf.BitLen;
        MemLen = bf.MemLen;

        pMem = new TELEM[MemLen];

        for(int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
	return *this;
}

int TBitField::operator==(const TBitField & bf) const // сравнение
{
    if(BitLen != bf.BitLen){
        return 0;
    }
    for (int i = 0; i < MemLen; i++) {
        if(pMem[i] != bf.pMem[i]){ 
            return 0; 
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField & bf) const // сравнение
{
    if(*this == bf) {
        return 0;
    }
    return 1;
}

TBitField TBitField::operator|(const TBitField & bf) // операция "или"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;//выбираем макс длину
    TBitField temp(maxLen); //ининициализируем временный объект

    for(int i = 0; i < MemLen; i++) {//копируем значения 
        temp.pMem[i] = pMem[i];
    }
   
    for(int i = 0; i < bf.MemLen; i++) {//побитовое или с элементами второго
        temp.pMem[i] |= bf.pMem[i];
    }

    return temp;
	
}

TBitField TBitField::operator&(const TBitField & bf) // операция "и"
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField temp(maxLen);

    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;// вычисляем минимальное количество элементов памяти

    for(int i = 0; i < minMemLen; i++) {
        temp.pMem[i] = pMem[i] & bf.pMem[i];// побитовое и только для пересекающихся ячеек
    }
    
    return temp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField temp(BitLen); //создаем поле такой же длины 

    //инвертируем все ячейки массива
    for(int i = 0; i < MemLen; i++) {
        temp.pMem[i] = ~pMem[i];
    }

    //очищаем лишние биты в последней ячейке массива
    int lastBits = BitLen % BITS_IN_ONE_MEM;//находим остаток битов
    if(lastBits != 0) {
        unsigned int mask = (1U << lastBits) - 1; //создаем маску для нужных битов
        temp.pMem[MemLen - 1] &= mask; //обнуляем «хвост» из лишних битов
    }

    return temp;
}

// ввод/вывод

istream& operator>>(istream & istr, TBitField & bf) // ввод
{
    int i = 0;
    char ch;
    while(istr >> ch && i < bf.GetLength()) {
        if(ch == '1') {
            bf.SetBit(i); 
        } else if(ch == '0') {
            bf.ClrBit(i);
        }
        i++;
    }
    return istr;
}

ostream& operator<<(ostream & ostr, const TBitField & bf) // вывод
{
    for(int i = 0; i < bf.GetLength(); i++) {
        if(bf.GetBit(i)) {
            ostr << '1';
        } else {
            ostr << '0';
        }
    }
    return ostr;
}
