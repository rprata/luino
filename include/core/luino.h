#ifndef ___LUINO_H__
#define ___LUINO_H__ 

#define HUGE_VAL	(__builtin_huge_val())
#define BUFSIZ 256
#define M 0x7FFFFFFF

#if defined(lua_getlocaledecpoint)
undef lua_getlocaledecpoint
#endif

#ifndef lua_getlocaledecpoint
#define lua_getlocaledecpoint() '.'
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
 
#define l_signalT signed char volatile
size_t strxfrm(char* dest, const char* src, size_t n);
int strcoll(const char* s1, const char* s2);
FILE* fopen(const char* path, const char* mode);
FILE* freopen(const char* filename, const char* mode, FILE* stream);
char* strerror(int errnum);
unsigned long int prng();

#define luai_makeseed prng
#define l_randomizePivot prng

void luino_init();
#endif