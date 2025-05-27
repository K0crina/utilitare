#ifndef TASK1_H
#define TASK1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char tip;   // 's' = short sau 'l' = long 
    char* id;   // string-ul propriu-zis, fără cratime (ex: "f" sau "file")
} ID;

typedef struct {
    char tip;                   // 'f' - flag, 'a' - argument, 'o' - opțiune
    void* valoare;              // pointer la valoare
                          // pentru 'a' și 'o' este pointer un șir de caractere
                         // pentru 'f' este pointer la o valoare numerică, 0 sau 1

    ID* ids;     // vector (dinamic) de ID (daca avem mai multe forme scurte/lungi)
    int nr_ids;     // numarul de identificatori pe care ii are parametrul

} argument;

char* strdup(const char* s);  
argument* loadArguments(FILE* fin, int* count_args);
void writeArguments(FILE* fout, const argument* arguments, int count_args);

#endif