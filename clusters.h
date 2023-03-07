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

typedef struct Color
{
    unsigned short color;
    bool visit;
} Color;

// a node in linked list
typedef struct Node
{
    unsigned short color;
    unsigned short count;
    struct Node *next;
} Node;

void SkipComments(FILE *fp);
Color **writeToArray(FILE *file, char *fileFormat, int width, int height);
void countColors(Color **array, int width, int height, char *filename);
void freeArray(void **array, int size);
void printArray(Color **array, int width, int height);
// stack functions
void push(Position pos);
void pop(Position *pos);
// linked list functions
void display(Node *node);
Node *newNode(unsigned short color, unsigned short count);
void insertionSort(Node **head, Node *newNode);
void freeList(Node **head);
void saveResults(char *resultFileName, char *bitmapFileName, Node *head);

#endif