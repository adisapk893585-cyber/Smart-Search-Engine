#ifndef INVERTEDINDEX_H
#define INVERTEDINDEX_H

#include<iostream>
#include<unordered_map>
#include<vector>
#include<string>

#include "Document.h"
#include "Parser.h"

using namespace std;

class InvertedIndex{

private:

    // word -> (document id -> frequency)
    unordered_map<
        string,
        unordered_map<int,int>
    > index;

    Parser parser;

public:

    InvertedIndex();

    void addDocument(Document document);

    vector<int> search(string word);

    unordered_map<int,int> getPostingList(string word);

    bool contains(string word);

    int vocabularySize();

    int getDocumentFrequency(string word);

    int getWordFrequency(
        string word,
        int documentId
    );

    vector<int> andSearch(
    string first,
    string second
    );

    vector<int> orSearch(
        string first,
        string second
    );

    vector<int> notSearch(
        string first,
        string second
    );

};

#endif