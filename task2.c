#include "task2.h"

int compareAUX(const void* a, const void* b) {
    const aux* aux1 = (const aux*)a;
    const aux* aux2 = (const aux*)b;
    return strcmp(aux1->name, aux2->name);
}

void kbConvert(long size, char* kbSize) {
    if (size < 1024) {
        sprintf(kbSize, "%ld", size);
    } 
    else {     
        long kb = size / 1024;               
        sprintf(kbSize, "%ldK", kb);
    }
}

void freeFiles(File* head) {
    while (head) {
        File* temp = head;
        head = head->next;
        free(temp->fileName);
        free(temp);
    }
}

void freeStack2(Node* top) {
    while (top) {
        Node* temp = top;
        top = top->next;
        free(temp->path);
        free(temp->name);
        freeFiles(temp->files);
        free(temp);
    }
}

Node* push2(Node* top, const char* path, const char* name, int depth, long dirSize) {
    Node* newNode = malloc(sizeof(Node));
    newNode->path = strdup(path);
    newNode->name = strdup(name);
    newNode->depth = depth;
    newNode->dirSize = dirSize;
    newNode->files = NULL;
    
    newNode->next = top;

    return newNode;
}

void addAtBeginning(File** head, const char* filePath, long fileSize) {
    File *newNode = (File*)malloc(sizeof(File));
	newNode->fileName = strdup(filePath);
    newNode->fileSize = fileSize;
	newNode->next = *head;
	*head = newNode;
}

void addAtEnd(File** head, const char* filePath, long fileSize) {
    if (*head == NULL) {
        addAtBeginning(head, filePath, fileSize);
        return;
    }

    File *aux = *head;
    while (aux->next != NULL) 
        aux = aux->next;

    File *newNode = (File*)malloc(sizeof(File));
    newNode->fileName = strdup(filePath);
    newNode->fileSize = fileSize;
    newNode->next = NULL;

    aux->next = newNode;
}

Node* util_ls2(Node** top, const char* path, int depth) {

    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Nu s-a putut deschide directorul \n");
        return *top;
    }

    struct stat status;
    const struct dirent* entry;
    
    while ((entry = readdir(dir)) != NULL) {

        if (strcmp(entry->d_name, ".DS_Store") == 0)
            continue;

        char completePath[1024];
        snprintf(completePath, sizeof(completePath), "%s/%s", path, entry->d_name);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            if (depth == 1 && stat(completePath, &status) == 0) 
                *top = push2(*top, completePath, entry->d_name, depth, status.st_size);
            continue; 
        }

        if (stat(completePath, &status) == 0) {
            // verififcam daca este director
            if (S_ISDIR(status.st_mode)) {
                *top = push2(*top, completePath, entry->d_name, depth, status.st_size);
                util_ls2(top, completePath, depth+1);        // apelam recursiv functia pentru fiecare subdirector        
                
            }      
            // verificam daca este fisier
            else if (S_ISREG(status.st_mode)) {
                if (depth == 1) {
                    Node* parent = *top;
                    while (parent && !(parent->depth == 1 && strcmp(parent->name, ".") == 0))
                        parent = parent->next;
                    if (parent)
                        addAtEnd(&(parent->files), entry->d_name, status.st_size);
                }
                else {
                    Node* parent = *top;
                    while (parent && parent->depth >= depth) 
                        parent = parent->next;
                    if (parent) 
                        addAtEnd(&(parent->files), entry->d_name, status.st_size);
                }
            }
        }
    }
    
    closedir(dir);
    return *top;
}

void printRecursive(Node* Stack, const char* completePath, const char* name, int depth, FILE* fout, int humanReadable) {

    if (depth) {
        // indentare in functie de depth
        for (int i = 1; i < depth; i++) 
            fprintf(fout, "    ");

        long size = 0;

        Node* current = Stack;
        while (current) {
            if (strcmp(current->path, completePath) == 0) {
                size = current->dirSize;
                break;
            }
            current = current->next;
        }

        if (humanReadable) {
            char kbSize[20];
            kbConvert(size, kbSize);
            fprintf(fout, "%s %s\n", kbSize, name);
        } else 
            fprintf(fout, "%ld %s\n", size, name);
    }

    aux vsort[512];
    int count = 0;

    // adauga fisierele nodului curent in vectorul de sortat
    Node* temp = Stack;
    while (temp) {
        if ((strcmp(temp->path, completePath) == 0 && strcmp(name, ".") != 0) || (strcmp(temp->name, ".") == 0 && depth == 0)) {
            File* f = temp->files;
            while (f) {
                vsort[count].name = f->fileName;
                vsort[count].isDir = 0;
                vsort[count].size = f->fileSize;
                count++;
                f = f->next;
            }
            break;
        }
        temp = temp->next;
    }

    // adaugam subdirectoarele directorului parinte in vectorul de sortat
    Node* dir = Stack;
    long nameLen = strlen(completePath);
    while (dir) {
        if (strncmp(dir->path, completePath, nameLen) == 0 && dir->depth == depth + 1 && dir->path[nameLen] == '/') {
            vsort[count].name = dir->path + nameLen + 1;  
            vsort[count].isDir = 1;
            vsort[count].size = dir->dirSize;
            vsort[count].depth = depth + 1;
            count++;
        }
        dir = dir->next;
    }

    // sortam vectorul de subdirectoare + fisiere ale directorului parinte
    qsort(vsort, count, sizeof(aux), compareAUX);

    // afisam fisierele si apelam recursiv functia pentru directoare
    for (int i = 0; i < count; i++) {
        if (vsort[i].isDir == 0) {
            for (int j = 1; j < depth + 1; j++) 
                fprintf(fout, "    ");
            if (humanReadable) {
                char sizeStr[20];
                kbConvert(vsort[i].size, sizeStr);
                fprintf(fout, "%s %s\n", sizeStr, vsort[i].name);
            } else 
                fprintf(fout, "%ld %s\n", vsort[i].size, vsort[i].name);
        }
        else if (vsort[i].isDir == 1) {
            char nextPath[1024];
            snprintf(nextPath, sizeof(nextPath), "%s/%s", completePath, vsort[i].name);
            printRecursive(Stack, nextPath, vsort[i].name, vsort[i].depth, fout, humanReadable);
        }
    }
}

void printSimple(Node* Stack, const char* completePath, const char* name, int depth, FILE* fout, int humanReadable) {
    aux vsort[512];
    int count = 0;

    // adauga fisierele nodului curent in vectorul de sortat
    Node* temp = Stack;
    while (temp) {
        if ((strcmp(temp->path, completePath) == 0 && strcmp(name, ".") != 0) || (strcmp(temp->name, ".") == 0 && depth == 0)) {
            File* f = temp->files;
            while (f) {
                vsort[count].name = f->fileName;
                vsort[count].isDir = 0;
                vsort[count].size = f->fileSize;
                count++;
                f = f->next;
            }
            break;
        }
        temp = temp->next;
    }

    // adaugam subdirectoarele directorului parinte in vectorul de sortat
    Node* dir = Stack;
    long nameLen = strlen(completePath);
    while (dir) {
        if (strncmp(dir->path, completePath, nameLen) == 0 && dir->depth == depth + 1 && dir->path[nameLen] == '/') {
            vsort[count].name = dir->path + nameLen + 1;  
            vsort[count].isDir = 1;
            vsort[count].size = dir->dirSize;
            vsort[count].depth = depth + 1;
            count++;
        }
        dir = dir->next;
    }

    // sortam vectorul de subdirectoare + fisiere ale directorului parinte
    qsort(vsort, count, sizeof(aux), compareAUX);

    // afisam vectorul sortat
    for (int i = 0; i < count; i++) {
        if (humanReadable) {
            char sizeStr[20];
            kbConvert(vsort[i].size, sizeStr);
            fprintf(fout, "%s %s\n", sizeStr, vsort[i].name);
        } else 
            fprintf(fout, "%ld %s\n", vsort[i].size, vsort[i].name);
    }
}
