#ifndef STRINGCOMPARATOR_H
#define STRINGCOMPARATOR_H


class StringComparator
{

    public:
        StringComparator();

        int Length(char* line);

        void Copy(char* line, char* source);

        bool Compare(char* one, char* two);

};

#endif // STRINGCOMPARATOR_H
