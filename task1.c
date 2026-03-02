#include "task1.h"

char* strdup(const char* s) {
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
        line[strcspn(line, "\n")] = '\0';         
        
        // cream un vector cu elementele de pe linie folosind spatiul ca delimitator

        char** elements = NULL;  
        int elem_count = 0;
        char* p = strtok(line, " ");
        while (p != NULL) {
            elements = realloc(elements, (elem_count + 1) * sizeof(char*));
            if (!elements) {
                printf("Eroare alocare memorie pentru vectorul de elemente\n");
                exit(1);
            }   
            elements[elem_count++] = p;
            p = strtok(NULL, " ");
        }

        char tip = '\0';
        
        if (elem_count > 0) 
            tip = elements[elem_count - 1][0];      // ultimul element reprezintă tipul parametrului a/f/o

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
                arg.ids[arg.nr_ids].id = strdup(elements[i]+2);
                arg.nr_ids++;
            } else if(strncmp(elements[i], "-", 1) == 0) {                                        
                arg.ids = realloc(arg.ids, (arg.nr_ids + 1) * sizeof(ID));
                arg.ids[arg.nr_ids].tip = 's';
                arg.ids[arg.nr_ids].id = strdup(elements[i]+1);
                arg.nr_ids++;   
            }
        }
        
        argument* temp = realloc(arguments, (*count_args + 1) * sizeof(argument));
        if (!temp) {
            free(arguments);  
            printf("Eroare la realocare \n");
            exit(1);
        }
        arguments = temp;

        arguments[*count_args] = arg;
        (*count_args)++;

        free(elements);
    }

    return arguments;
}

void writeArguments(FILE* fout, const argument* arguments, int count_args) {
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
            fprintf(fout, " ");
        }
        fprintf(fout, "\n");
    }
}
