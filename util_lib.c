#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct File {
    char* path;
    char** includes;
    int nr;           // nr de includes
    struct File* next;  
} File;

void freeList(File* head) {
    while (head) {
        File* temp = head;
        head = head->next;

        free(temp->path);
        for (int i = 0; i < temp->nr; i++) {
            free(temp->includes[i]);
        }
        free(temp->includes);
        free(temp);
    }
}

void addAtBeginning(File** head, const char* path, char** includes, int nr) {
    File* newNode = malloc(sizeof(File));
    newNode->path = strdup(path);
    newNode->includes = malloc(sizeof(char*) * nr); 
    for (int i = 0; i < nr; i++) 
        newNode->includes[i] = strdup(includes[i]);
    newNode->nr = nr;
    newNode->next = *head;
    *head = newNode;
}

void addAtEnd(File** head, const char* path, char** includes, int nr) {
    if (*head == NULL) {
        addAtBeginning(head, path, includes, nr);
        return;
    }

    File *aux = *head;
    while (aux->next != NULL) 
        aux = aux->next;

    File *newNode = (File*)malloc(sizeof(File));
    newNode->path = strdup(path);
    newNode->includes = malloc(sizeof(char*) * nr); 
    for (int i = 0; i < nr; i++) 
        newNode->includes[i] = strdup(includes[i]);
    newNode->nr = nr;
    newNode->next = NULL;

    aux->next = newNode;
}

File* addToFile(File* head, const char* path) {
    FILE* f = fopen(path, "r");
    if (f == NULL) return head;

    char line[1024];
    char** includes = malloc(sizeof(char*) * 20);  
    int nr = 0;

    while (fgets(line, sizeof(line), f)) {
        char* start = NULL;
        char* end = NULL;
        if (strncmp(line, "#include", 8) == 0) {
            if (strchr(line, '\"'))
                start = strchr(line, '\"');
            else if (strchr(line, '<'))
                start = strchr(line, '<');
            else
                continue;

            if (strchr(start + 1, '\"'))
                end = strchr(start + 1, '\"');
            else if (strchr(start + 1, '>'))
                end = strchr(start + 1, '>');
            else
                continue;

            *end = '\0';

            includes[nr++] = strdup(start + 1);
        }
    }

    fclose(f);
    addAtEnd(&head, path, includes, nr);

    for (int i = 0; i < nr; i++) free(includes[i]);
    free(includes);

    return head;
}

void util_ls(File** head, const char* path) {
    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Nu s-a putut deschide directorul \n");
        return;
    }

    struct stat status;
    const struct dirent* entry;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        char completePath[1024];
        snprintf(completePath, sizeof(completePath), "%s/%s", path, entry->d_name);

        if (stat(completePath, &status) == 0) {
            if (S_ISDIR(status.st_mode)) 
                util_ls(head, completePath);
            else if (strstr(entry->d_name, ".c") || strstr(entry->d_name, ".h")) 
                *head = addToFile(*head, completePath);
        }
    }
    
    closedir(dir);
}

int detectLoop(File* current, File* head, File* included[], int nr) {
    for (int i = 0; i < nr; i++)
        if (included[i] == current) 
            return 1; 

    included[nr++] = current;

    for (int i = 0; i < current->nr; i++) {
        File* temp = head;
        while (temp) {
            if (detectLoop(temp, head, included, nr)) 
                return 1;
            temp = temp->next;
        }
    }

    return 0;
}

int loop(File* head) {
    File* temp = head;

    while (temp) {
        File* included[20];
        int nr = 0;

        if (detectLoop(temp, head, included, nr)) return 1;

        temp = temp->next;
    }

    return 0;
}

void duplicates(File* head) {
    for (File* a = head; a != NULL; a = a->next) 
        for (File* b = a->next; b != NULL; b = b->next) 
            for (int i = 0; i < a->nr; i++) 
                for (int j = 0; j < b->nr; j++) 
                    if (strcmp(a->includes[i], b->includes[j]) == 0)
                        printf("WARNING double include: %s in %s %s\n", a->includes[i], a->path, b->path);
}

int main(int argc, char** argv) {

    int warnings = 0;
    if (argc == 3 && strcmp(argv[2], "--full") == 0) 
        warnings = 1;

    File* head = NULL;
    util_ls(&head, argv[1]); // ne folosim de util_ls ca sa adaugam fisierele directorului in lista

    if(warnings) duplicates(head);

    if (loop(head)) 
        printf("ERROR loop detected\n");

    freeList(head);
    return 0;
}
