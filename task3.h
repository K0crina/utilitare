#ifndef TASK3_H
#define TASK3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

// structura pentru stiva
typedef struct stackNode {
    char* path;
    char* name;
    int depth;
    struct stackNode* next;
} stackNode;

// structura pentru arbore
typedef struct treeNode {
    char* name;
    int nr;      // nr de copii pentru fiecare director
    struct treeNode** children;
} treeNode;

char** splitPath(const char* path, int* length);
void freeParts(char** parts, int len);

void freeStack3(stackNode* top);
stackNode* push3(stackNode* top, const char* path, const char* name, int depth);
stackNode* util_ls3(stackNode** top, const char* path, int depth);

void freeTree(treeNode* Node);
treeNode* createNode(const char* name);
void addChild(treeNode* parent, treeNode* child);
treeNode* buildTree(stackNode* Stack, const char* commonPath);
int findPath(treeNode* root, const char* target, treeNode** path, int depth);

void util_cd(treeNode* root, const char* source, const char* dest, FILE* fout);

#endif 

