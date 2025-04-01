#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

char* duplicateString(const char* s) {
    if (s == NULL)
        return NULL;

    int l = strlen(s);
    char* copy = malloc(l + 1); 
    if (copy != NULL) strcpy(copy, s);

    return copy;
}

argument* loadArguments (FILE* fin, int* count_args) {
    argument* arguments = NULL; 
    *count_args = 0;
    
    char line[1024];
    while (fgets(line, sizeof(line), fin)) {
        line[strcspn(line, "\n")] = '\0';          // eliminam newline-ul de la sfarsit
        
        // cream un vector cu elementele de pe linie folosind spatiul ca delimitator

        char** elements = NULL;  // vector de elemente alocat dinamic
        int elem_count = 0;
        char* p = strtok(line, " ");
        while (p != NULL) {
            elements = realloc(elements, (elem_count + 1) * sizeof(char*));
            if (!elements) {
                perror("Eroare alocare memorie pentru vectorul de elemente");
                exit(1);
            }
            elements[elem_count++] = p;
            p = strtok(NULL, " ");
        }
        
        char tip = elements[elem_count - 1][0];      // ultimul element reprezintă tipul parametrului a/f/o
        
        argument arg;                           // cream un nou argument si il initializam
        arg.tip = tip;
        arg.valoare = NULL;
        arg.ids = NULL;
        arg.nr_ids = 0;
        
        // elementele de la 0 la elem_count - 2 sunt identificatori

        for (int i = 0; i < elem_count - 1; i++) {
            // daca elementul are doua cratime este identificator lung, altfel este scurt
            if (strncmp(elements[i], "--", 2) == 0) {
                arg.ids = realloc(arg.ids, (arg.nr_ids + 1) * sizeof(ID));
                arg.ids[arg.nr_ids].tip = 'l';
                arg.ids[arg.nr_ids].id = duplicateString(elements[i]+2);
                arg.nr_ids++;
            } else if(strncmp(elements[i], "-", 1) == 0) {                                        
                arg.ids = realloc(arg.ids, (arg.nr_ids + 1) * sizeof(ID));
                arg.ids[arg.nr_ids].tip = 's';
                arg.ids[arg.nr_ids].id = duplicateString(elements[i]+1);
                arg.nr_ids++;   
            }
        }
        
        arguments = realloc(arguments, (*count_args + 1) * sizeof(argument));
        arguments[*count_args] = arg;
        (*count_args)++;

        free(elements);
    }

    return arguments;
}

void writeArguments(FILE* fout, argument* arguments, int count_args) {
    for (int i = 0; i < count_args; i++) {
        if (arguments[i].nr_ids > 0) {
            for (int j = 0; j < arguments[i].nr_ids; j++) {
                if (arguments[i].ids[j].tip == 'l')
                    fprintf(fout, "--%s ", arguments[i].ids[j].id);
                else 
                    fprintf(fout, "-%s ", arguments[i].ids[j].id);
            }
        }
        
        // afisam tipul
        fprintf(fout, "%c ", arguments[i].tip);
        
        // afisam valoarea
        if (arguments[i].valoare != NULL) {
            if (arguments[i].tip == 'f') {
                fprintf(fout, "%d", *(int*)arguments[i].valoare);
            } else {
                fprintf(fout, "%s", (char*)arguments[i].valoare);
            }
        }
        fprintf(fout, " \n");
    }
}

int main(int argc, char** argv){

    char* InputFile = NULL;
    char* OutputFile = NULL;

    // cautam fisierele de input/output in apel
    for (int i = 2; i < argc; i++) {
        if (strncmp(argv[i], "--file=", 7) == 0) 
            InputFile = argv[i] + 7; 
        else if (strncmp(argv[i], "--out=", 6) == 0) 
            OutputFile = argv[i] + 6;
    }

    FILE* fin = fopen(InputFile, "r");
    if (!fin) {
        printf("Eroare la deschiderea fișierului de input! \n");
        return 1;
    }
    
    int count_args = 0;
    argument* arguments = loadArguments(fin, &count_args);

    fclose(fin);

   for (int i = 2; i < argc; i++) {
        if (strncmp(argv[i], "--file", 6) == 0 || strncmp(argv[i], "--out", 5) == 0)
            continue;
        
        // daca elementul nu incepe cu '-' => argument
        if (argv[i][0] != '-'){
            for (int j = 0; j < count_args; j++) {
                if (arguments[j].tip == 'a' && arguments[j].valoare == NULL) {
                    arguments[j].valoare = duplicateString(argv[i]);
                    break;
                }
            }
        }
        // elementul incepe cu '-' => flag sau optiune 
        else {
            if (argv[i][1] != '-') {
                // flag sau optiune de forma scurta
                char* equal = strchr(argv[i], '=');
                if (equal) {
                    // optiune scurta cu egal => extragem identificatorul si valoarea
                    char* startID = argv[i] + 1;
                    int lenID = equal - startID;
                    char id[100];
                    strncpy(id, startID, lenID);
                    id[lenID] = '\0';
                    for (int j = 0; j < count_args; j++)
                        if (arguments[j].tip == 'o' && arguments[j].nr_ids > 0 && strcmp(arguments[j].ids[0].id, id) == 0) {
                            arguments[j].valoare = duplicateString(equal + 1);
                            break;
                        }
                } 
                else if (strlen(argv[i]) > 2) {
                    // daca are mai multe caractere dupa '-' => flaguri separate
                    int len = strlen(argv[i]);
                    for (int k = 1; k < len; k++) {
                        char flag[2];
                        flag[0] = argv[i][k];
                        flag[1] = '\0';
                        for (int j = 0; j < count_args; j++)
                            if (arguments[j].tip == 'f' && arguments[j].nr_ids > 0 && strcmp(arguments[j].ids[0].id, flag) == 0 && arguments[j].valoare == NULL) {
                                int* flagVal = malloc(sizeof(int));
                                *flagVal = 1;
                                arguments[j].valoare = flagVal;
                            }
                    }
                } 
                else {
                    // daca nu are mai multe caractere dupa '-' => flag singur sau optiune cu valoare in urmatorul element
                    for (int j = 0; j < count_args; j++) 
                        if (arguments[j].nr_ids > 0 && strcmp(argv[i] + 1, arguments[j].ids[0].id) == 0) {
                            if (arguments[j].tip == 'f') {
                                if (arguments[j].valoare == NULL) {
                                    int* flagVal = malloc(sizeof(int));
                                    *flagVal = 1;
                                    arguments[j].valoare = flagVal;
                                }
                            } else if (arguments[j].tip == 'o'){ 
                                if (i + 1 < argc && argv[i+1][0] != '-') {
                                    arguments[j].valoare = duplicateString(argv[i+1]);
                                    i++;
                                } 
                            }
                            break;
                        }
                }
            }
            else {
                // flag sau optiune de forma lunga
                char* equal = strchr(argv[i], '=');
                if (equal) {
                    // optiune lunga cu egal => extragem identificatorul si valoarea
                    char* startID = argv[i] + 2;
                    int lenID = equal - startID;
                    char id[100];
                    strncpy(id, startID, lenID);
                    id[lenID] = '\0';
                    for (int j = 0; j < count_args; j++) {
                        if (arguments[j].tip == 'o' && arguments[j].nr_ids > 0 && strcmp(arguments[j].ids[0].id, id) == 0) 
                            arguments[j].valoare = duplicateString(equal + 1);
                            break;
                        }
                } 
                else {
                    //flag lung singur sau optiune lunga cu valoare in urmatorul element
                    char* startID = argv[i] + 2;
                    for (int j = 0; j < count_args; j++)
                        if (arguments[j].nr_ids > 0 && strcmp(arguments[j].ids[0].id, startID) == 0) {
                            if (arguments[j].tip == 'f') {
                                if (arguments[j].valoare == NULL) {
                                    int* flagVal = malloc(sizeof(int));
                                    *flagVal = 1;
                                    arguments[j].valoare = flagVal;
                                }
                            } else if (arguments[j].tip == 'o') {
                                if (i + 1 < argc && argv[i+1][0] != '-') {
                                    arguments[j].valoare = duplicateString(argv[i+1]);
                                    i++;
                                }
                            }
                            break;
                        }
                }
            }
        }
    }

    FILE* fout = fopen(OutputFile, "w");
    if (!fout) {
        printf("Eroare la deschiderea fișierului de output! \n");
        return 1;
    }
    
    writeArguments(fout, arguments, count_args);
    fclose(fout);

    // eliberam memoria alocata pentru argumente
    for (int i = 0; i < count_args; i++) {
        for (int j = 0; j < arguments[i].nr_ids; j++) {
            free(arguments[i].ids[j].id);
        }
        free(arguments[i].ids);
        if (arguments[i].valoare != NULL) {
            free(arguments[i].valoare);
        }
    }
    free(arguments);
    
    return 0;

}