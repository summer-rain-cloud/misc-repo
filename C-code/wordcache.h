//
//  wordcache.h
//
//  Created by Yu Xia on 6/7/24.
//

#ifndef wordcahce_h
#define wordcahce_h

#include <stdio.h>

char* strget(const char* argv);
char* reallocstr(const char* word, char* dest);
void free_all(char** cache, int cache_size);
void print_all_cache(char** cache, int cache_size);
int hash(char* word, int cache_size);
char* getWordcpy(char* wordcpy, char* readcache, int len);
int actionNeeded(char* cachespace, char* word);
char* actionString(int action, char* actionstr);
char* calloc_realloc_nop(char* cache, int action, char* word);

#endif
