#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TITLE 100
#define FILENAME "todolist.txt"

typedef struct{
    char *title;
    int done;} Task;

typedef struct{
    Task *tasks;
    size_t count;
    size_t capacity;} TodoList;

void initList(TodoList *list){
    list->count = 0;
    list->capacity = 4;
    list->tasks = malloc(sizeof(Task) * list->capacity);}

void freeList(TodoList *list){
    for (size_t i = 0; i < list->count; i++)
        free(list->tasks[i].title);
    free(list->tasks);}

void addTask(TodoList *list, const char *title){
    if (list->count == list->capacity) {
        list->capacity *= 2;
        list->tasks = realloc(list->tasks, sizeof(Task) * list->capacity);
    }
    list->tasks[list->count].title = malloc(strlen(title) + 1);
    strcpy(list->tasks[list->count].title, title);
    list->tasks[list->count].done = 0;
    list->count++;}

void removeTask(TodoList *list, size_t index){
    if (index >= list->count) return;
    free(list->tasks[index].title);
    for (size_t i = index; i < list->count - 1; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }
    list->count--;}

void toggleTask(TodoList *list, size_t index){
    if (index >= list->count) return;
    list->tasks[index].done = !list->tasks[index].done;}

void saveToFile(TodoList *list, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (size_t i = 0; i < list->count; i++) {
        fprintf(f, "%d|%s\n", list->tasks[i].done, list->tasks[i].title);
    }
    fclose(f);}

void loadFromFile(TodoList *list, const char *filename){
    FILE *f = fopen(filename, "r");
    if (!f) return;

    char line[MAX_TITLE + 10];
    while (fgets(line, sizeof(line), f)) {
        int done;
        char title[MAX_TITLE];
        if (sscanf(line, "%d|%[^\n]", &done, title) == 2) {
            addTask(list, title);
            list->tasks[list->count - 1].done = done;
        }
    }
    fclose(f);}

void printTasks(TodoList *list){
    for (size_t i = 0; i < list->count; i++) {
        printf("[%c] %zu: %s\n", list->tasks[i].done ? 'x' : ' ', i + 1, list->tasks[i].title);
    }}

void menu(){
    printf("TODO List Menu:\n");
    printf("1. Add task\n");
    printf("2. Remove task\n");
    printf("3. Toggle task\n");
    printf("4. Show all tasks\n");
    printf("5. Save to file\n");
    printf("6. Load from file\n");
    printf("0. Exit\n");
    printf("Choose an option: ");}

int main(){
    TodoList list;
    initList(&list);

    loadFromFile(&list, FILENAME);

    int choice;
    char buffer[MAX_TITLE];
    size_t index;

    do {
        menu();
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter task title: ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                addTask(&list, buffer);
                break;
            case 2:
                printf("Enter task number to remove: ");
                scanf("%zu", &index);
                removeTask(&list, index - 1);
                break;
            case 3:
                printf("Enter task number to toggle: ");
                scanf("%zu", &index);
                toggleTask(&list, index - 1);
                break;
            case 4:
                printTasks(&list);
                break;
            case 5:
                saveToFile(&list, FILENAME);
                printf("Saved to %s\n", FILENAME);
                break;
            case 6:
                freeList(&list);
                initList(&list);
                loadFromFile(&list, FILENAME);
                printf("Loaded from %s\n", FILENAME);
                break;
            case 0:
                saveToFile(&list, FILENAME);
                break;
            default:
                printf("Invalid option.\n");
        }
    } while (choice != 0);

    freeList(&list);
    return 0;}
