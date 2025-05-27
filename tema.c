#include <stdio.h>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"

int main(int argc, char** argv) {

    if (strcmp(argv[1], "task1") == 0) {
        const char* InputFile = NULL;
        const char* OutputFile = NULL;

        // cautam fisierele de input/output in apel
        for (int i = 2; i < argc; i++) {
            if (strncmp(argv[i], "--file=", 7) == 0) 
                InputFile = argv[i] + 7; 
            else if (strncmp(argv[i], "--out=", 6) == 0) 
                OutputFile = argv[i] + 6;
        }

        if (InputFile == NULL || OutputFile == NULL) {
            printf("Lipsesc parametrii de Input/Output\n");
            return 1;
        }

        FILE* fin = fopen(InputFile, "r");
        if (!fin) {
            printf("Eroare la deschiderea fisierului de input! \n");
            return 1;
        }
        
        int count_args = 0;
        argument* arguments = loadArguments(fin, &count_args);

        fclose(fin);

    for (int i = 2; i < argc; i++) {
            if (strncmp(argv[i], "--file", 6) == 0 || strncmp(argv[i], "--out", 5) == 0)
                continue;
            
            // daca elementul nu incepe cu '-' => argument
            if (argv[i][0] != '-') {
                for (int j = 0; j < count_args; j++) {
                    if (arguments[j].tip == 'a' && arguments[j].valoare == NULL) {
                        arguments[j].valoare = strdup(argv[i]);
                        break;
                    }
                }
            }
            // elementul incepe cu '-' => flag sau optiune 
            else {
                if (argv[i][1] != '-') {
                    // flag sau optiune de forma scurta
                    const char* equal = strchr(argv[i], '=');
                    if (equal) {
                        // optiune scurta cu egal => extragem identificatorul si valoarea
                        const char* startID = argv[i] + 1;
                        int lenID = equal - startID;
                        char id[100];
                        strncpy(id, startID, lenID);
                        id[lenID] = '\0';
                        for (int j = 0; j < count_args; j++)   
                            if (arguments[j].nr_ids > 0 && arguments[j].tip == 'o' && strcmp(arguments[j].ids[0].id, id) == 0) {
                                arguments[j].valoare = strdup(equal + 1);
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
                                if (arguments[j].nr_ids > 0 && arguments[j].tip == 'f' && strcmp(arguments[j].ids[0].id, flag) == 0 && arguments[j].valoare == NULL) {
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
                                } else if (arguments[j].tip == 'o') { 
                                    if (i + 1 < argc && argv[i+1][0] != '-') {
                                        arguments[j].valoare = strdup(argv[i+1]);
                                        i++;
                                    } 
                                }
                                break;
                            }
                    }
                }
                else {
                    // flag sau optiune de forma lunga
                    const char* equal = strchr(argv[i], '=');
                    if (equal) {
                        // optiune lunga cu egal => extragem identificatorul si valoarea
                        const char* startID = argv[i] + 2;
                        int lenID = equal - startID;
                        char id[100];
                        strncpy(id, startID, lenID);
                        id[lenID] = '\0';
                        for (int j = 0; j < count_args; j++) 
                            if (arguments[j].nr_ids > 0 && arguments[j].tip == 'o' && strcmp(arguments[j].ids[1].id, id) == 0) {
                                arguments[j].valoare = strdup(equal + 1);
                                break;
                            }
                    } 
                    else {
                        //flag lung singur sau optiune lunga cu valoare in urmatorul element
                        const char* startID = argv[i] + 2;
                        for (int j = 0; j < count_args; j++)
                            if (arguments[j].nr_ids > 0 && strcmp(arguments[j].ids[1].id, startID) == 0) {
                                if (arguments[j].tip == 'f') {
                                    if (arguments[j].valoare == NULL) {
                                        int* flagVal = malloc(sizeof(int));
                                        *flagVal = 1;
                                        arguments[j].valoare = flagVal;
                                    }
                                } else if (arguments[j].tip == 'o') {
                                    if (i + 1 < argc && argv[i+1][0] != '-') {
                                        arguments[j].valoare = strdup(argv[i+1]);
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
            printf("Eroare la deschiderea fisierului de output! \n");
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

    } else if (strcmp(argv[1], "task2") == 0) {
        const char* OutputFile = NULL;

        char* option = NULL;
        char* path = strdup(argv[2]); 

        DIR* dir = opendir(path);
        if (dir == NULL) {
            printf("Eroare la deschidereea directorului \n");
            return 1;
        }

        // cautam fisierul de output in apel
        for (int i = 3; i < argc; i++) 
            if (strncmp(argv[i], "--out=", 6) == 0) 
                OutputFile = argv[i] + 6;
            else 
                option = strdup(argv[i] + 1);

        Node* Stack = NULL;

        struct stat status;
        stat(path, &status);    
        Stack = push2(Stack, path, path, 0, status.st_size);

        Stack = util_ls2(&Stack, path, 1);
        closedir(dir);

        FILE* fout = fopen(OutputFile, "w");
        if (!fout) {
            printf("Eroare la deschiderea fisierului de output \n");
            return 1;
        }

        if (option == NULL) 
            printSimple(Stack, path, path, 0, fout, 0);
        else if (strcmp(option, "h") == 0)
            printSimple(Stack, path, path, 0, fout, 1);
        else if (strcmp(option, "r") == 0)
            printRecursive(Stack, path, path, 0, fout, 0);
        else if (strcmp(option, "rh") == 0)
            printRecursive(Stack, path, path, 0, fout, 1);

        freeStack2(Stack);
        free(option);
        free(path);
        fclose(fout);

    } else if (strcmp(argv[1], "task3") == 0) {
        const char* OutputFile = NULL;

        char* path1 = strdup(argv[2]); 
        char* path2 = strdup(argv[3]);
        OutputFile = argv[4] + 6;

        int len1 = 0, len2 = 0;
        char** parts1 = splitPath(path1, &len1);
        char** parts2 = splitPath(path2, &len2);

        int i = 0;
        while (i < len1 && i < len2 && strcmp(parts1[i], parts2[i]) == 0) 
            i++;

        if (i == 0) {
            printf("Caile nu au directoare comune\n");
            freeParts(parts1, len1);
            freeParts(parts2, len2);
            return 1;
        } 

        char* commonDir = strdup(parts1[i-1]);
        // printf("%s \n", commonDir);

        char commonPath[1024];
        strcpy(commonPath, parts1[0]);
        int j =1;
        while (strcmp(parts1[j], commonDir) != 0) {
            int len = strlen(commonPath);
            if (len + 1 + strlen(parts1[j]) + 1 < sizeof(commonPath)) {
                commonPath[len] = '/';
                commonPath[len + 1] = '\0';
                strcat(commonPath, parts1[j]);
            }
            j++;
        }

        int len = strlen(commonPath);
        if (len + 1 + strlen(parts1[j]) + 1 < sizeof(commonPath)) {
            commonPath[len] = '/';
            commonPath[len + 1] = '\0';
            strcat(commonPath, parts1[j]);
        }

        const char* commonName = parts1[i - 1];

        DIR* dir = opendir(commonPath);
        if (dir == NULL) {
            printf("Eroare la deschidereea directorului \n");
            return 1;
        }

        stackNode* Stack = NULL;
        Stack = push3(Stack, commonPath, commonName, 0);
        Stack = util_ls3(&Stack, commonPath, 1);

        closedir(dir);

        FILE* fout = fopen(OutputFile, "w");
        if (!fout) {
            printf("Eroare la deschiderea fisierului de output \n");
            return 1;
        }

        treeNode* root = NULL;
        root = buildTree(Stack, commonPath);

        util_cd(root, parts1[len1 - 1], parts2[len2 - 1], fout);
        
        freeStack3(Stack);
        freeTree(root);

        free(commonDir);
        freeParts(parts1, len1);
        freeParts(parts2, len2);
        free(path1);
        free(path2);
        fclose(fout);
        
    } else if (strcmp(argv[1], "task4") == 0) {
        if (argc < 6) {
            printf("Numar insuficient de argumente \n");
            return 1;
        }        

        char* OutputFile = NULL;

        char* file1 = strdup(argv[2]); 
        char* file2 = strdup(argv[3]);
        OutputFile = strdup(argv[5]);

        int l1, l2;
        char** lines1 = readLines(file1, &l1);
        char** lines2 = readLines(file2, &l2);

        FILE* fout = fopen(OutputFile, "w");
        if (!fout) 
            printf("Eroare la deschiderea fisierului de output \n");

        int max = INT_MAX;

        if (l1 > l2) max = l1;
        else max = l2;
        
        for (int i = 0; i < max; i++) {
            const char* ref;
            const char* compare;

            if (i < l1) ref = lines1[i];
            else ref = "";
            
            if (i < l2) compare = lines2[i];
            else compare = "";

            int delete, insert, replace;
            util_diff(ref, compare, &delete, &insert, &replace);
            fprintf(fout, "%dD %dI %dR \n", delete, insert, replace);
        }

        free(OutputFile);
        freeLines(lines1, l1);
        freeLines(lines2, l2); 
        free(file1);
        free(file2);
        fclose(fout);
    }

    return 0;
}