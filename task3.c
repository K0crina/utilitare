#include "task3.h"

void freeParts(char** parts, int len) {
    for (int i = 0; i < len; i++)
        free(parts[i]);
    free(parts);
}

void freeStack3(stackNode* top) {
    while (top) {
        stackNode* next = top->next;
        free(top->path);
        free(top->name);
        free(top);
        top = next;
    }
}

void freeTree(treeNode* Node) {
    if (Node == NULL) return;
    for (int i = 0; i < Node->nr; i++)
        freeTree(Node->children[i]);
    free(Node->children);
    free(Node->name);
    free(Node);
}

char** splitPath(const char* path, int* length) {
    char* copy = strdup(path);
    const char* p = strtok(copy, "/");
    int capacity = 1;
    int len = 0;

    char** parts = NULL;
    parts = malloc(capacity * sizeof(char*));

    while (p != NULL) {
        if (len >= capacity) {
            capacity++;
            char** temp = realloc(parts, capacity * sizeof(char*));
            if (temp == NULL) {
                freeParts(parts, len); 
                return NULL;          
            }
            parts = temp;
        }
        parts[len++] = strdup(p);
        p = strtok(NULL, "/");
    }

    free(copy);
    *length = len;
    return parts;
}

stackNode* push3(stackNode* top, const char* path, const char* name, int depth) {
    stackNode* newNode = malloc(sizeof(stackNode));
    newNode->path = strdup(path);
    newNode->name = strdup(name);
    newNode->depth = depth;
    newNode->next = top;

    return newNode;
}

stackNode* util_ls3(stackNode** top, const char* path, int depth) {

    DIR* dir = opendir(path);
    if (dir == NULL) {
        printf("Nu s-a putut deschide directorul \n");
        return *top;
    }

    struct stat status;
    const struct dirent* entry;
    
    while ((entry = readdir(dir)) != NULL) {

        char completePath[1024];
        snprintf(completePath, sizeof(completePath), "%s/%s", path, entry->d_name);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; 
        }

        if (stat(completePath, &status) == 0) {
            // verififcam daca este director
            if (S_ISDIR(status.st_mode)) {
                *top = push3(*top, completePath, entry->d_name, depth);
                util_ls3(top, completePath, depth+1);        // apelam recursiv functia pentru fiecare subdirector        
                
            }      
        }
    }
    
    closedir(dir);
    return *top;
}

treeNode* createNode(const char* name) {
    treeNode* newNode = malloc(sizeof(treeNode));
    newNode->name = strdup(name);
    newNode->nr = 0;
    newNode->children = NULL;
    return newNode;
}

void addChild(treeNode* parent, treeNode* child) {
    parent->children = realloc(parent->children, (parent->nr+1) * sizeof(treeNode*));
    parent->children[parent->nr] = child;
    parent->nr++;
}

treeNode* buildTree(stackNode* Stack, const char* commonPath) {
    stackNode* temp = Stack;
    int count = 0;

    while (temp) {
        count++;
        temp = temp->next;
    }

    treeNode** nodes = malloc(count * sizeof(treeNode*));
    char** paths = malloc(count * sizeof(char*));

    temp = Stack;
    int k = 0;
    while (temp) {
        nodes[k] = createNode(temp->name);
        paths[k] = temp->path;
        k++;
        temp = temp->next;
    }

    // conectam fiecare nod cu copiii sai
    treeNode* root = NULL;
    for (int i = 0; i < count; i++) {
        if (strcmp(paths[i], commonPath) == 0) {
            root = nodes[i];
            continue;
        }

        for (int j = 0; j < count; j++) {
            if (j == i) continue;

            // verificam daca paths[j] este prefix si subdirector direct pentru paths[i] 
            int len = strlen(paths[j]);
            if (strncmp(paths[i], paths[j], len) == 0 && paths[i][len] == '/' &&  (strchr(paths[i] + len + 1, '/') == NULL)) { 
                addChild(nodes[j], nodes[i]);
                break;
            }
        }
    }

    free(nodes); 
    free(paths);
    return root;
}

int findPath(treeNode* root, const char* target, treeNode** path, int depth) {
    if (strcmp(root->name, target) == 0) {
        path[depth] = root;
        return depth + 1;
    }

    for (int i = 0; i < root->nr; i++) {
        int d = findPath(root->children[i], target, path, depth + 1);
        if (d > 0) {
            path[depth] = root;
            return d;
        }
    }

    return 0; 
}

void util_cd(treeNode* root, const char* source, const char* dest, FILE* fout) {
    treeNode* path1[100], *path2[100];

    int len1 = findPath(root, source, path1, 0);
    int len2 = findPath(root, dest, path2, 0);

    // gasim primul director diferit din caile celor doua directoare
    int i = 0;
    while (i < len1 && i < len2 && strcmp(path1[i]->name, path2[i]->name) == 0)
        i++;

    // urcam de la sursa pana la un director comun
    for (int j = i; j < len1; j++)
        fprintf(fout, "../");

    // coboram spre directorul destinatie
    for (int j = i; j < len2; j++)
        fprintf(fout, "%s/", path2[j]->name);
}