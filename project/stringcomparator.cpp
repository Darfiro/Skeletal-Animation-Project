#include "stringcomparator.h"

int StringComparator::Length(char *line)
{
    int size = 0;
    while(line[size])
    {
        size++;
    }

    return size;
}

#include <stdio.h>
void StringComparator::Copy(char *line, char *source)
{
    int size = Length(source);
    for (int i = 0; i < size; i++)
    {
        line[i] = source[i];
    }
}

StringComparator::StringComparator()
{

}

bool StringComparator::Compare(char *one, char *two)
{
    bool exact = true;
    int num = Length(one);
    for (int i = 0; i < num && one[i] && two[i]; i++)
    {
        if (one[i] != two[i])
            exact = false;
    }

    return exact;
}

