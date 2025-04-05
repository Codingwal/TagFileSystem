#pragma once

#include <stdint.h>

typedef unsigned int Tag;
typedef uint64_t Tags;

typedef struct Pair Pair;
typedef struct FileSystem
{
    Tag highestId;
    Pair *files;
} FileSystem;

typedef struct File
{
    char content[100];
} File;

void fs_init(FileSystem *fs);

File *fs_createFile(FileSystem *fs, Tags tags);
Tag fs_addTag(FileSystem *fs);

File *fs_getFile(FileSystem *fs, Tags tags);

Tags fs_tagArr2Tags(Tag *tagArray, size_t tagCount);
Tags fs_tags2Tags(size_t tagCount, ...);
Tags fs_vtags2Tags(size_t tagCount, va_list args);