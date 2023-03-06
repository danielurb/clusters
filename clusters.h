#ifndef CLUSTERS_H
#define CLUSTERS_H

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 256

typedef struct Position
{
    short x;
    short y;
} Position;

typedef struct Cluster
{
    unsigned short color;
    bool visit;
} Cluster;

// creating a node in linked list
typedef struct Node
{
    unsigned short color;
    unsigned short count;
    struct Node *next;
    // Pointer pointing towards next node
} Node;

Cluster **writeToArray(FILE *file, char *fileFormat, int width, int height);
void countClusters(Cluster **array, int width, int height, char * filename);
void freeArray(void **array, int size);
void SkipComments(FILE *fp);
void printArray(Cluster *array[], int width, int height);
// stack functions
void push(Position pos);
void pop(Position *pos);
// linked list functions
void display(Node *node);
Node *newNode(unsigned short color, unsigned short count);
void insertion_sort(Node **head, Node *newNode);
void free_list(Node **head);
void save_results(char *result_file_name, char *bitmap_file_name, Node *head);

#endif