#include "clusters.h"

int main(int argc, char **argv)
{
    char filename[50];
    if (argc == 2)
    {

        strcpy(filename, argv[1]);
    }
    else if (argc > 2)
    {
        printf("Too many arguments supplied, example: ./main img1.pbm\n");
        exit(1);
    }
    else
    {
        printf("One argument expected, example: ./main img1.pbm\n");
        exit(1);
    }

    FILE *file = NULL;
    char fileFormat[3];
    int width, height, colors;

    file = fopen(filename, "rb");
    if (!file)
    {
        printf("Couldn't open %s file\n", filename);
        exit(1);
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

    Color **array = writeToArray(file, fileFormat, width, height);
    countColors(array, width, height, filename);
    freeArray((void *)array, height);
    fclose(file);
    return 0;
}
