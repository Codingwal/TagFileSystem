#include "fileSystem.h"
#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>

struct Pair
{
    struct Pair *next[2];
    bool isLeaf;
};

static Pair *fs_createLeaf(void)
{
    Pair *pair = malloc(sizeof(Pair));
    pair->isLeaf = true;
    pair->next[0] = NULL;
    pair->next[1] = NULL;
    return pair;
}

void fs_init(FileSystem *fs)
{
    fs->highestId = 0;
    fs->files = fs_createLeaf();
}

typedef void (*LeafFunc)(Pair *);
static void fs_foreachLeafRecursive(Pair *pair, LeafFunc callback)
{
    if (pair == NULL)
        return;

    if (!pair->isLeaf)
    {
        fs_foreachLeafRecursive(pair->next[0], callback);
        fs_foreachLeafRecursive(pair->next[1], callback);
        return;
    }

    callback(pair);
}

File *fs_createFile(FileSystem *fs, Tags tags)
{
    Pair *pair = fs->files;
    while (true)
    {
        bool tagPresent = tags & 1;

        // Create file
        if (pair->isLeaf)
        {
            if (pair->next[tagPresent] != NULL) // File already present
            {
                printf("File with this tag combination already exists!\n");
                return NULL;
            }

            File *file = malloc(sizeof(File));
            pair->next[tagPresent] = (Pair *)file;
            return file;
        }

        tags >>= 1;
        pair = pair->next[tagPresent];
    }
}

void fs_addTagHelper(Pair *parent)
{
    Pair *a = fs_createLeaf();
    Pair *b = fs_createLeaf();

    a->next[0] = parent->next[0];
    b->next[0] = parent->next[1];
    parent->next[0] = a;
    parent->next[1] = b;
    parent->isLeaf = false;
}
Tag fs_addTag(FileSystem *fs)
{
    fs_foreachLeafRecursive(fs->files, fs_addTagHelper);
    return ++fs->highestId;
}

File *fs_getFile(FileSystem *fs, Tags tags)
{
    Pair *pair = fs->files;
    while (true)
    {
        bool tagPresent = tags & 1;

        // Return file
        if (pair->isLeaf)
            return (File *)pair->next[tagPresent];

        tags >>= 1;
        pair = pair->next[tagPresent];
    }
}

Tags fs_tagArr2Tags(Tag *tagArray, size_t tagCount)
{
    Tags tags = 0;
    for (size_t i = 0; i < tagCount; i++)
    {
        tags |= 1 << (unsigned int)tagArray[i];
    }
    return tags;
}
Tags fs_tags2Tags(size_t tagCount, ...)
{
    va_list args;
    va_start(args, tagCount);
    Tags tags = fs_vtags2Tags(tagCount, args);
    va_end(args);
    return tags;
}
Tags fs_vtags2Tags(size_t tagCount, va_list args)
{

    Tags tags = 0;
    for (size_t i = 0; i < tagCount; i++)
    {
        tags |= 1 << va_arg(args, Tags);
    }
    return tags;
}