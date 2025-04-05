#include <stdio.h>
#include "fileSystem.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

static FileSystem fs;

static void createFile(size_t tagCount, char *content, ...)
{
    va_list args;
    va_start(args, content);
    Tags tags = fs_vtags2Tags(tagCount, args);
    va_end(args);

    File *f = fs_createFile(&fs, tags);
    strcpy(f->content, content);
}

static void printFile(size_t tagCount, ...)
{
    va_list args;
    va_start(args, tagCount);
    Tags tags = fs_vtags2Tags(tagCount, args);
    va_end(args);

    File *f = fs_getFile(&fs, tags);
    if (f == NULL)
    {
        printf("File (Tags = 0x%X) does not exist!\n", (uint32_t)tags);
        return;
    }
    printf("File (Tags = 0x%X): \"%s\"\n", (uint32_t)tags, f->content);
}

int main()
{
    fs_init(&fs);

    Tag txt = fs_addTag(&fs);
    Tag cool = fs_addTag(&fs);

    createFile(1, "Cool", cool);
    createFile(2, "Cool.txt", cool, txt);
    printFile(2, txt, cool);

    Tag school = fs_addTag(&fs);

    createFile(1, "School", school);
    createFile(2, "School.txt", school, txt);

    printFile(1, school);
    printFile(1, cool);
    printFile(2, school, txt);
    printFile(2, school, cool);
}