#include "PrintEx.h"

PrintEx::PrintEx(Print *p): _p(p) { }

PrintEx& PrintEx::newln(){
    _p->println();
    return *this;
}

PrintEx& PrintEx::print(const String &s){
    _p->print(s);
    return *this;
}

PrintEx& PrintEx::println(const String &s){
    _p->println(s);
    return *this;
}

PrintEx& PrintEx::println(const uint8_t data){
    _p->println(data);
    return *this;
}

PrintEx& PrintEx::printHEX(const uint8_t data){
    _p->print((data & 0xF0) >> 4, HEX);
    _p->print((data & 0x0F) >> 0, HEX);
    return *this;
}

PrintEx& PrintEx::printHEXln(const uint8_t data){
    printHEX(data);
    newln();
    return *this;
}


PrintEx& PrintEx::printHEX(const uint16_t data){
    _p->print((data & 0xF000) >> 12, HEX);
    _p->print((data & 0x0F00) >> 8, HEX);
    _p->print((data & 0x00F0) >> 4, HEX);
    _p->print((data & 0x000F) >> 0, HEX);

    return *this;
}


PrintEx& PrintEx::printHEXln(const uint16_t data){
    printHEX(data);
    newln();

    return *this;
}
