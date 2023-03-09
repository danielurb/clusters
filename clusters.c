#include "clusters.h"

/***** Clusters functions *****/
void skipComments(FILE *fp)
{
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#')
    {
        fgets(line, sizeof(line), fp);
        skipComments(fp);
    }
    else
    {
        fseek(fp, -1, SEEK_CUR);
    }
}

Color **writeToArray(FILE *file, char *fileFormat, int width, int height)
{
    Color **array = (Color **)malloc(sizeof(Color *) * height);
    for (int i = 0; i < height; i++)
    {
        array[i] = (Color *)calloc(sizeof(Color), width);
    }

    unsigned char c;
    if (strcmp(fileFormat, "P4") == 0)
    {
        for (int i = 0; i < height; i++)
        {
            int counter = 0;
            for (int j = 0; j < ceil((double)width / 8); j++)
            {
                fscanf(file, "%c", &c);
                for (int ctr = 7; ctr >= 0; ctr--)
                {
                    if (counter < width)
                    {
                        array[i][counter].color = (c >> ctr) & 1;
                    }
                    else
                    {
                        break;
                    }
                    counter++;
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                fscanf(file, "%c", &c);
                array[i][j].color = c;
            }
        }
    }
    return array;
}

void countColors(Color **array, int width, int height, char *filename)
{
    Node *head = NULL;
    Node *newNodeP = NULL;
    StackNode *root = NULL;

    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            if (!array[x][y].visit)
            {
                unsigned short color = array[x][y].color;
                unsigned short count = 1;
                Position pos = {x, y};
                push(&root, pos);
                array[x][y].visit = true;

                while (!isEmpty(root))
                {
                    Position Color;
                    pop(&root, &Color);
                    int X = Color.x;
                    int Y = Color.y;

                    // above
                    if (X > 0 && !array[X - 1][Y].visit && array[X - 1][Y].color == color)
                    {
                        push(&root, (Position){X - 1, Y});
                        array[X - 1][Y].visit = true;
                        count++;
                    }
                    // left
                    if (Y > 0 && !array[X][Y - 1].visit && array[X][Y - 1].color == color)
                    {
                        push(&root, (Position){X, Y - 1});
                        array[X][Y - 1].visit = true;
                        count++;
                    }
                    // right
                    if (Y < width - 1 && !array[X][Y + 1].visit && array[X][Y + 1].color == color)
                    {
                        push(&root, (Position){X, Y + 1});
                        array[X][Y + 1].visit = true;
                        count++;
                    }
                    // bottom
                    if (X < height - 1 && !array[X + 1][Y].visit && array[X + 1][Y].color == color)
                    {
                        push(&root, (Position){X + 1, Y});
                        array[X + 1][Y].visit = true;
                        count++;
                    }
                }
                newNodeP = newNode(color, count);
                insertionSort(&head, newNodeP); // insert the node (color, count) into the list in the correct position
            }
        }
    }
    saveResults("results.txt", filename, head);
    freeList(&head);
}

void freeArray(void **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
    array = NULL;
}

void printArray(Color **array, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("%3d) ", i + 1);
        for (int j = 0; j < width; j++)
        {
            printf("%3d ", array[i][j].color);
        }
        printf("\n");
    }
}

/***** Stack *****/
StackNode *newStackNode(Position data)
{
    StackNode *stackNode = (StackNode *)malloc(sizeof(StackNode));
    stackNode->data = data;
    stackNode->next = NULL;
    return stackNode;
}

bool isEmpty(StackNode *root)
{
    return !root;
}

void push(StackNode **root, Position data)
{
    StackNode *stackNode = newStackNode(data);
    stackNode->next = *root;
    *root = stackNode;
}

bool pop(StackNode **root, Position *data)
{
    if (isEmpty(*root))
        return false;
    StackNode *temp = *root;
    *root = (*root)->next;
    *data = temp->data;
    free(temp);
    return true;
}

bool peek(StackNode *root, Position *data)
{
    if (isEmpty(root))
        return false;
    *data = root->data;
    return true;
}

/***** Linked list *****/
// function to print the linked list
void display(Node *node)
{
    while (node != NULL)
    {
        printf("%d : %d\n", node->color, node->count);
        node = node->next;
    }
}

Node *newNode(unsigned short color, unsigned short count)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->color = color;
    newNode->count = count;
    newNode->next = NULL;
    return newNode;
}

// function to insert data in sorted position
void insertionSort(Node **head, Node *newNode)
{
    // If linked list is empty or color of head is higher than color of new node
    if (*head == NULL || (*head)->color > newNode->color)
    {
        newNode->next = *head;
        *head = newNode;
    }
    else
    {
        Node *current = *head;
        while (current->next != NULL && current->next->color <= newNode->color)
        {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
}

// function to free allocated memory of linked list
void freeList(Node **head)
{
    Node *current = NULL;
    while (*head)
    {
        current = *head;
        *head = (*head)->next;
        free(current);
    }
    head = NULL;
}

void saveResults(char *resultFileName, char *bitmapFileName, Node *head)
{
    FILE *out = fopen(resultFileName, "a");
    fprintf(out, "%s\n", bitmapFileName);
    fprintf(out, "color : size\n");
    fprintf(out, "%5d : %d", head->color, head->count);

    while (head->next)
    {
        if (head->color == head->next->color)
        {
            fprintf(out, " %d", head->next->count);
        }
        else
        {
            fprintf(out, "\n%5d : %d", head->next->color, head->next->count);
        }
        head = head->next;
    }
    fprintf(out, "\n\n");
    fclose(out);
}