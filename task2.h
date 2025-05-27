#ifndef TASK2_H
#define TASK2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct File {
    char* fileName;   
    long fileSize;    
    struct File* next;
}File;

typedef struct Node {
    char* path;
    char* name;
    int depth; 
    long dirSize;
    File* files;
    struct Node* next;
} Node;

// structura auxiliara pentru sortarea in ordine alfabetica
typedef struct aux {
    char* name;
    int isDir;
    long size;
    int depth;
} aux;              

int compareAUX(const void* a, const void* b);
void kbConvert(long size, char* kbSize);

void freeFiles(File* head);
void freeStack2(Node* top);

Node* push2(Node* top, const char* path, const char* name, int depth, long dirSize);
void addAtBeginning(File** head, const char* filePath, long fileSize);
void addAtEnd(File** head, const char* filePath, long fileSize);
Node* util_ls2(Node** top, const char* path, int depth);

void printRecursive(Node* Stack, const char* completePath, const char* name, int depth, FILE* fout, int humanReadable);
void printSimple(Node* Stack, const char* completePath, const char* name, int depth, FILE* fout, int humanReadable);

#endif