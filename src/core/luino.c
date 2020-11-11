#include <luino.h>
#include <lstate.h>
#include <lauxlib.h>

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

static int _lastSeedNumber = 38;

unsigned long int prng() {
	unsigned long int x = _lastSeedNumber++;

	if(x <= M-1){
		_lastSeedNumber = 26;//More magic numbers!
	}

	x = (x >> 16) + ((x << 15) & M)  - (x >> 21) - ((x << 10) & M);

	if(x < 0){
		x += M;
	}

	return x;
}


int luino_init(const char * lua_code) {
	lua_State* L = luaL_newstate();
	luaopen_base(L);
#ifdef ENABLE_LUA_DEBUG
	luaopen_debug(L);
#endif
#ifdef ENABLE_LUA_MATH
	luaopen_math(L);
#endif
#ifdef ENABLE_LUA_STRING
	luaopen_string(L);
#endif
#ifdef ENABLE_LUA_COROUTINE
	luaopen_coroutine(L);
#endif
#ifdef ENABLE_LUA_TABLE
	luaopen_table(L);
#endif
	int res = luaL_loadbuffer(L, lua_code, strlen(lua_code), "main");
	if (res) return -1;
	res = lua_pcall(L, 0, LUA_MULTRET, 0);
	if(res)	return -2;
}