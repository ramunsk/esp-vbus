#ifndef _PRINTEX_H_
#define _PRINTEX_H_

#include <Print.h>

class PrintEx {
public:
    PrintEx(Print *p);

    PrintEx& print(const String &s);
    PrintEx& println(const String &s);

    PrintEx& newln();
    PrintEx& println(const uint8_t data);

    PrintEx& printHEX(const uint8_t data);
    PrintEx& printHEXln(const uint8_t data);

    PrintEx& printHEX(const uint16_t data);
    PrintEx& printHEXln(const uint16_t data);
private:
    Print *_p;
};

#endif
