#include "task4.h"

void freeLines(char** lines, int nrl) {
    for (int i = 0; i < nrl; i++) 
        free(lines[i]); 
    
    free(lines); 
}

char** readLines(const char* file, int* nrl) {
    FILE* f = fopen(file, "r");
    if (!f) {
        printf("Eroare la deschiderea fisierului %s\n", file);
        return NULL;
    }

    char** lines = NULL;
    char buffer[1024];
    *nrl = 0;

    while (fgets(buffer, sizeof(buffer), f)) {
        char** temp = realloc(lines, (*nrl + 1) * sizeof(char*));
        if (!temp) {
            for (int i = 0; i < *nrl; i++) free(lines[i]);
            free(lines);
            fclose(f);
            return NULL;
        }
        lines = temp;
        lines[*nrl] = strdup(buffer);
        (*nrl)++;
    }

    fclose(f);
    return lines;
}

int min(int replace, int insert, int delete) {
    if (replace <= delete && replace <= insert)
        return replace;
    else if (delete <= replace && delete <= insert) 
        return delete;
    else 
        return insert;
}

void util_diff(const char* ref, const char* compare, int* D, int* I, int* R) {
    // construim matricea de costuri
    int l = strlen(compare) + 1;
    int c = strlen(ref) + 1;

    int** cost = (int**) malloc(l * sizeof(int*));
    for (int i = 0; i < l; i++) {
        cost[i] = (int*) malloc(c * sizeof(int));
    }

    for (int i = 0; i < l; i++) cost[i][0] = i;
    for (int j = 0; j < c; j++) cost[0][j] = j;

    for (int i = 1; i < l; i++) {
        for (int j = 1; j < c; j++) {
            if (compare[i - 1] == ref[j - 1]) {
                cost[i][j] = cost[i - 1][j - 1];
            } else {
                int replace = cost[i - 1][j - 1] + 2;
                int insert = cost[i - 1][j] + 1;
                int delete = cost[i][j - 1] + 1;
                cost[i][j] = min(replace, insert, delete);
            }
        }
    }

    int i = l, j = c;
    *D = *I = *R = 0;

    while (i > 0 || j > 0) {
        if (i > 0 && j > 0 && compare[i - 1] == ref[j - 1]) {
            i--;
            j--;
        } else if (i > 0 && j > 0 && cost[i][j] == cost[i - 1][j - 1] + 2) {
            (*R)++;
            i--;
            j--;
        } else if (i > 0 && cost[i][j] == cost[i - 1][j] + 1) {
            (*I)++;
            i--;
        } else if (j > 0 && cost[i][j] == cost[i][j - 1] + 1) {
            (*D)++;
            j--;
        } 
    }

    for (int k = 0; k <= l; k++) {
        free(cost[k]);
    }
    free(cost);
}
