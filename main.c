#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    short x;
    short y;
} Position;

#define SIZE 256

int top = -1;
Position stack[SIZE];

void push(Position a)
{
    if (top == SIZE - 1)
    {
        printf("\nOverflow!!");
    }
    else
    {
        top++;
        stack[top] = a;
    }
}

void pop(Position *b)
{
    if (top == -1)
    {
        printf("\nUnderflow!!");
    }
    else
    {
        // printf("\nPopped element: %d", inp_array[top]);
        b->x = stack[top].x;
        b->y = stack[top].y;
        top--;
    }
}

unsigned short **writeToArray(FILE *file, char *fileFormat, int width, int height);
void countClusters(unsigned short **array, int width, int height);
void freeArray(void **arrayP, int size);
void SkipComments(FILE *fp);
void printArray(unsigned short *array[], int width, int height);

void countClusters(unsigned short **array, int width, int height)
{
    bool **visited = (bool **)malloc(sizeof(bool *) * height);
    for (int i = 0; i < height; i++)
    {
        visited[i] = (bool *)calloc(width, sizeof(bool));
        // visited[i] = (bool *)malloc(sizeof(bool) * width);
        // memset(visited[i], -1, sizeof(bool) * width);
    }

    // printf("\n");
    // for (int i = 0; i < height; i++)
    // {
    //     printf("%3d) ", i + 1);
    //     for (int j = 0; j < width; j++)
    //     {
    //         printf("%s ", visited[i][j] ? "true" : "false");
    //     }
    //     printf("\n");
    // }

    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {

            if (!visited[x][y])
            {
                int color = array[x][y];
                int count = 1;
                Position pos = {x, y};
                push(pos);
                visited[x][y] = true;

                while (top > -1)
                {
                    Position cluster;
                    pop(&cluster);
                    int pX = cluster.x;
                    int pY = cluster.y;
                    // visited[pX][pY] = true;

                    // printf("\n");
                    // for (int i = 0; i < height; i++)
                    // {
                    //     printf("%3d) ", i + 1);
                    //     for (int j = 0; j < width; j++)
                    //     {
                    //         // printf("%s ", visited[i][j] ? "true" : "false");
                    //         printf("%d ", visited[i][j]);
                    //     }
                    //     printf("\n");
                    // }

                    // above
                    if (pX > 0 && !visited[pX - 1][pY] && array[pX - 1][pY] == color)
                    {
                        push((Position){pX - 1, pY});
                        visited[pX - 1][pY] = true;
                        count++;
                    }
                    // left
                    if (pY > 0 && !visited[pX][pY - 1] && array[pX][pY - 1] == color)
                    {
                        push((Position){pX, pY - 1});
                        visited[pX][pY - 1] = true;
                        count++;
                    }
                    // right
                    if (pY < width - 1 && !visited[pX][pY + 1] && array[pX][pY + 1] == color)
                    {
                        push((Position){pX, pY + 1});
                        visited[pX][pY + 1] = true;

                        count++;
                    }
                    // bottom
                    if (pX < height - 1 && !visited[pX + 1][pY] && array[pX + 1][pY] == color)
                    {
                        push((Position){pX + 1, pY});
                        visited[pX + 1][pY] = true;
                        count++;
                    }
                }
                printf("top %d, color: %d %d times\n", top, color, count);
            }
        }
    }

    // printf("\n");
    // for (int i = 0; i < height; i++)
    // {
    //     printf("%3d) ", i + 1);
    //     for (int j = 0; j < width; j++)
    //     {
    //         printf("%3d ", label[i][j]);
    //     }
    //     printf("\n");
    // }
    freeArray((void *)visited, height);
}

int main(/*int argc, char **argv*/)
{
    FILE *file = NULL;
    char fileFormat[3];
    int width, height, colors;
    char *filename = "img1.pbm";
    // char *filename = "img2.pbm";

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

    unsigned short **array = writeToArray(file, fileFormat, width, height);
    printArray(array, width, height);

    countClusters(array, width, height);

    freeArray((void *)array, height);
    fclose(file);
    return 0;
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

void printArray(unsigned short *array[], int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        printf("%3d) ", i + 1);
        for (int j = 0; j < width; j++)
        {
            printf("%3d ", array[i][j]);
        }
        printf("\n");
    }
}

unsigned short **writeToArray(FILE *file, char *fileFormat, int width, int height)
{
    unsigned short **array = (unsigned short **)malloc(sizeof(unsigned short *) * height);
    for (int i = 0; i < height; i++)
    {
        array[i] = (unsigned short *)malloc(sizeof(unsigned short) * width);
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
                        array[i][counter] = (c >> ctr) & 1;
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
                array[i][j] = c;
            }
        }
    }
    return array;
}

void freeArray(void **arrayP, int size)
{
    for (int i = 0; i < size; i++)
    {
        free(arrayP[i]);
    }
    free(arrayP);
    arrayP = NULL;
}