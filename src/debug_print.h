#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...)    Serial.printf(fmt, ## args)
#else
#define DEBUG_PRINT(fmt, args...)    /* Don't do anything in release builds */
#endif

#endif
