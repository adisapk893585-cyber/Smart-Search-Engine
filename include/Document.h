#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>

using namespace std;

class Document{

public:

    int id;
    string title;
    string content;

    Document();

    Document(
        int id,
        string title,
        string content
    );
};

#endif