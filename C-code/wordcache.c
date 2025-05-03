//
//  wordcache.c
//
//  Created by Yu Xia on 6/7/24.
//

#include "wordcache.h"

#include <stdio.h>	//standard c input and output
#include <stdlib.h>	//standard c library
#include <string.h>	//for string manipulation
#include <ctype.h>	//for character checking
#include <unistd.h>	//for file manipulation commands
#include <fcntl.h>	//for file manipulation keywords

int DEFAULT_SIZE = 8;

int main(int argc, const char** argv) {
    int errorfile = 0;
    //input: program cache-size *.txt ... [there can be multiple *.txt]
    //main parsing [0: the program name] [1: the cache size] [2+: the files]
    if(argc < 3) {fprintf(stderr, "ERROR: Not enough arguments to run program.\n"); return 0;}
    
    int text;
    int cache_size = atoi(*(argv+1));
    if(cache_size == 0) {fprintf(stderr, "ERROR: invalid cache size.\n"); return 0;}
    char* curr_filename = strget(*(argv+2));
    char** cache = calloc(cache_size, DEFAULT_SIZE);
    int curr_word_len = 0;
    char* read_cache = calloc(1, 129);
    char* read_input = calloc(1, 1);
    char* wordcpy = calloc(1, 129);
    
    //parsing a file requires the word to be 3 letters minimum, case sensitive
    //hash: ASCII sum modulus cache size
    //the cache size remains constant, the cache of words will change
    //if index [15] had "example" but another word's hash is also [15], overwrite "example" with new word
    //if a word is "word!" or "word.", ignore word, else continue
    int texIndex = 2;
    int action = -1;
    char* actionStr = calloc(1, DEFAULT_SIZE);
    while(texIndex < argc) {
        text = open(curr_filename, O_RDONLY);
        if(text == -1) {perror("ERROR: file does not exist.\n"); errorfile = 1; break;}
        int readErr = 1;
        while(readErr > 0) {
            readErr = (int) read(text, read_input, 1);
            if(ispunct(*read_input) > 0) {
                //stop and ignore word, reset allocations and such
                if(curr_word_len > 2) {
                    //function to add word to cache
                    //strncpy(wordcpy, read_cache, curr_word_len);
                    //strcpy(wordcpy, read_cache);
                    //printf("%s\n", wordcpy);
                    wordcpy = getWordcpy(wordcpy, read_cache, curr_word_len);
                    int indexHash = hash(wordcpy, cache_size);
                    action = actionNeeded(*(cache+indexHash), wordcpy);
                    actionStr = actionString(action, actionStr);
                    *(cache + indexHash) = calloc_realloc_nop(*(cache + indexHash), action, wordcpy);
                    printf("Word \"%s\" ==> %d (%s)\n", wordcpy, indexHash, actionStr);
                }
                curr_word_len = 0;
            }
            else if (iscntrl(*read_input) > 0) {
                //stop and add word to cache, reset allocations and such
                if(curr_word_len > 2) {
                    //function to add word to cache
                    //strncpy(wordcpy, read_cache, curr_word_len);
                    //strcpy(wordcpy, read_cache);
                    //printf("%s\n", wordcpy);
                    wordcpy = getWordcpy(wordcpy, read_cache, curr_word_len);
                    int indexHash = hash(wordcpy, cache_size);
                    action = actionNeeded(*(cache+indexHash), wordcpy);
                    actionStr = actionString(action, actionStr);
                    *(cache + indexHash) = calloc_realloc_nop(*(cache + indexHash), action, wordcpy);
                    printf("Word \"%s\" ==> %d (%s)\n", wordcpy, indexHash, actionStr);
                }
                curr_word_len = 0;
            }
            else if (isspace(*read_input) > 0) {
                //stop and add word to cache, reset allocations and such
                if(curr_word_len > 2) {
                    //function to add word to cache
                    //strncpy(wordcpy, read_cache, curr_word_len);
                    //strcpy(wordcpy, read_cache);
                    //printf("%s\n", wordcpy);
                    wordcpy = getWordcpy(wordcpy, read_cache, curr_word_len);
                    int indexHash = hash(wordcpy, cache_size);
                    action = actionNeeded(*(cache+indexHash), wordcpy);
                    actionStr = actionString(action, actionStr);
                    *(cache + indexHash) = calloc_realloc_nop(*(cache + indexHash), action, wordcpy);
                    printf("Word \"%s\" ==> %d (%s)\n", wordcpy, indexHash, actionStr);
                }
                curr_word_len = 0;
            }
            else {
                if(isalnum(*read_input) == 0) {curr_word_len = 0;}
                *(read_cache + curr_word_len) = *read_input;
                curr_word_len++;
            }
            
            //printf("%c", *read_input);
        }
        texIndex++;
        close(text);
        if(texIndex >= argc) {continue;}
        else {curr_filename = reallocstr(*(argv+texIndex), curr_filename);}
    }
    
    if(errorfile == 0) {
        //output file in cache
        printf("\nCache:\n");
        for(int i = 0; i < cache_size; i++) {
            printf("%c%d%c ==> \"%s\"\n", 91, i, 93, *(cache+i));
        }
    }
    
    free(curr_filename);
    free(read_cache); free(read_input); free(wordcpy); free(actionStr);
    free_all(cache, cache_size);
    return 0;
}

//due to the nature of strcpy, anytime a str is to be copied, a free must be called before this
char* strget(const char* argv) {
    char* tempname = calloc(1, (int) strlen(argv));
    strcpy(tempname, argv);
    return tempname;
}

char* reallocstr(const char* word, char* dest) {
    int byteSize = (int)sizeof(word) + 1;
    dest = (char*) realloc(dest, byteSize);
    strcpy(dest, word);
    return dest;
}

void free_all(char** cache, int cache_size) {
    for(int i = 0; i < cache_size; i++) {
        /*
        if(strlen(*(cache+i)) == 0) {}
        else {free(*(cache+i));}
         */
        free(*(cache+i));
    }
    free(cache);
}

void print_all_cache(char** cache, int cache_size) {
    for(int i = 0; i < cache_size; i++) {
        printf("[%d] ==> \"%s\"\n", i, *(cache+i));
    }
}

int hash(char* word, int cache_size) {
    int char_Num = (int)strlen(word);
    int hash_Num = 0;
    for(int i = 0; i < char_Num; i++) {
        hash_Num += *(word + i);
    }
    return hash_Num %= cache_size;
}

char* getWordcpy(char* wordcpy, char* readcache, int len) {
    wordcpy = realloc(wordcpy, len+1);
    strncpy(wordcpy, readcache, len);
    *(wordcpy+len) = '\0';
    return wordcpy;
}

int actionNeeded(char* cachespace, char* word) {
    if(cachespace == NULL) {return 0;}
    int space = (int) strlen(cachespace);
    int newspace = (int) strlen(word);
    if(space == 0) {return 0;} //calloc
    else if(space < newspace || space > newspace ) {return 1;} //realloc
    return 2; //nop
}

char* actionString(int action, char* actionstr) {
    if(action == 0) {return reallocstr("calloc", actionstr);}
    else if(action == 1) {return reallocstr("realloc", actionstr);}
    else {return reallocstr("nop", actionstr);}
}

char* calloc_realloc_nop(char* cache, int action, char* word) {
    int wordsize = (int) strlen(word);
    if(action == 0) {
        int size_c = (int) strlen(word) + 1;
        cache = calloc(1, size_c);
        strcpy(cache, word);
        return cache;
    }
    else if(action == 1) {
        cache = getWordcpy(cache, word, wordsize);
        return cache;
    }
    else {
        strcpy(cache, word);
        return cache;
    }
}
