#include <luino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>

#define SIGINT -1


size_t strxfrm(char* dest, const char* src, size_t n){
	/* This implementation does not know about any locale but "C"... */
	size_t n2 = strlen(src);
	if(n > n2){
		strcpy(dest,src);
	}
	return n2;
}

int strcoll(const char* s1, const char* s2){
	char t1[1 + strxfrm(0, s1, 0)];
	strxfrm(t1, s1, sizeof(t1));
	char t2[1 + strxfrm(0, s2, 0)];
	strxfrm(t2, s2, sizeof(t2));
	return strcmp(t1, t2);
}

FILE* fopen(const char* path, const char* mode){
	(void)path;
	(void)mode;

	return NULL;
}

FILE* freopen(const char* filename, const char* mode, FILE* stream){
	return NULL;
}

char* strerror(int errnum){
	return "Uknown error";
}

int luino_init(const char * lua_code) {
#ifdef ENABLE_LUA_MATH
	mathlib_open();
#endif
#ifdef ENABLE_LUA_STRING
	strlib_open();
#endif	
	lua_dostring (lua_code);
	return 0;
}
