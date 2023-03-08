#ifndef CLUSTERS_H
#define CLUSTERS_H

#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// A structure to represent a node in linked list
typedef struct Node
{
    unsigned short color;
    unsigned short count;
    struct Node *next;
} Node;

// A structure to represent a node in stack
typedef struct StackNode
{
    Position data;
    struct StackNode *next;
} StackNode;

/***** Clusters functions *****/
void SkipComments(FILE *fp);
Color **writeToArray(FILE *file, char *fileFormat, int width, int height);
void countColors(Color **array, int width, int height, char *filename);
void freeArray(void **array, int size);
void printArray(Color **array, int width, int height);

/***** Stack *****/
StackNode *newStackNode(Position data);
bool isEmpty(StackNode *root);
void push(StackNode **root, Position data);
bool pop(StackNode **root, Position *data);
bool peek(StackNode *root, Position *data);

/***** Linked list *****/
void display(Node *node);
Node *newNode(unsigned short color, unsigned short count);
void insertionSort(Node **head, Node *newNode);
void freeList(Node **head);
void saveResults(char *resultFileName, char *bitmapFileName, Node *head);

#endif