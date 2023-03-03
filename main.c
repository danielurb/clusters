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

typedef struct Cluster
{
    unsigned short color;
    bool visit;
} Cluster;


#define SIZE 256

int top = -1;
Position stack[SIZE];

void push(Position pos)
{
    if (top == SIZE - 1)
    {
        printf("\nOverflow!!");
    }
    else
    {
        top++;
        stack[top] = pos;
    }
}

void pop(Position *pos)
{
    if (top == -1)
    {
        printf("\nUnderflow!!");
    }
    else
    {
        *pos = stack[top];
        top--;
    }
}

Cluster **writeToArray(FILE *file, char *fileFormat, int width, int height);
void countClusters(Cluster **array, int width, int height);
void freeArray(void **array, int size);
void SkipComments(FILE *fp);
void printArray(Cluster *array[], int width, int height);


int main(/*int argc, char **argv*/)
{
    FILE *file = NULL;
    char fileFormat[3];
    int width, height, colors;
    // char *filename = "img1.pbm";
    char *filename = "img2.pbm";

    // char *filename = "feep.pbm";
    // char *filename = "img3.pgm";
    // char *filename = "img4.pgm";

    file = fopen(filename, "rb");
    if (!file)
    {
        printf("Couldn't open %s file\n", filename);
        exit(0);
    }

    fscanf(file, "%s", fileFormat);
    SkipComments(file);
    fscanf(file, "%d %d", &width, &height);
    SkipComments(file);
    if (strcmp(fileFormat, "P4") == 0)
    {
        colors = 2;
    }
    else if (strcmp(fileFormat, "P5") == 0)
    {
        fscanf(file, "%d", &colors);
        SkipComments(file);
    }
    else
    {
        printf("unknown file format\n");
        exit(EXIT_FAILURE);
    }

    printf("File format: %s, width: %d, height: %d, colors: %d\n", fileFormat, width, height, colors);

    Cluster **array = writeToArray(file, fileFormat, width, height);
    printArray(array, width, height);
    countClusters(array, width, height);
    freeArray((void*)array, height);
    fclose(file);
    return 0;
}


void countClusters(Cluster **array, int width, int height)
{
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {

            if (!array[x][y].visit)
            {
                int color = array[x][y].color;
                int count = 1;
                Position pos = {x, y};
                push(pos);
                array[x][y].visit = true;

                while (top > -1)
                {
                    Position cluster;
                    pop(&cluster);
                    int X = cluster.x;
                    int Y = cluster.y;

                    // above
                    if (X > 0 && !array[X - 1][Y].visit && array[X - 1][Y].color == color)
                    {
                        push((Position){X - 1, Y});
                        array[X - 1][Y].visit = true;
                        count++;
                    }
                    // left
                    if (Y > 0 && !array[X][Y - 1].visit && array[X][Y - 1].color == color)
                    {
                        push((Position){X, Y - 1});
                        array[X][Y - 1].visit = true;
                        count++;
                    }
                    // right
                    if (Y < width - 1 && !array[X][Y + 1].visit && array[X][Y + 1].color == color)
                    {
                        push((Position){X, Y + 1});
                        array[X][Y + 1].visit = true;

                        count++;
                    }
                    // bottom
                    if (X < height - 1 && !array[X + 1][Y].visit && array[X + 1][Y].color == color)
                    {
                        push((Position){X + 1, Y});
                        array[X + 1][Y].visit = true;
                        count++;
                    }
                }
                printf("top %d, color: %d %d times\n", top, color, count);
            }
        }
    }
}

void SkipComments(FILE *fp)
{
    int ch;
    char line[100];
    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        ;
    if (ch == '#')
    {
        fgets(line, sizeof(line), fp);
        SkipComments(fp);
    }
    else
    {
        fseek(fp, -1, SEEK_CUR);
    }
}

void printArray(Cluster *array[], int width, int height)
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

Cluster **writeToArray(FILE *file, char *fileFormat, int width, int height)
{
    Cluster **array = (Cluster **)malloc(sizeof(Cluster *) * height);
    for (int i = 0; i < height; i++)
    {
        array[i] = (Cluster *)calloc(sizeof(Cluster), width);
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

void freeArray(void **array, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(array[i]);
    }
    free(array);
    array = NULL;
}