#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include<iostream>
#include<vector>

#include "SearchResult.h"
#include "Document.h"

using namespace std;

class FileManager{

public:
    static vector<Document> loadDocuments(string folderPath);

    static void exportResultsToCSV(vector<SearchResult> results,vector<Document> documents);

};

#endif