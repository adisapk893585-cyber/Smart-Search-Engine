#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include<iostream>
#include<vector>

#include "Document.h"

using namespace std;

class FileManager{

public:

    static vector<Document> loadDocuments(
        string folderPath
    );

};

#endif