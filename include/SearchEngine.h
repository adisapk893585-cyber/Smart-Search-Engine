#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include<iostream>
#include<vector>
#include<unordered_set>


#include "Document.h"
#include "InvertedIndex.h"
#include "Trie.h"
#include "Ranking.h"
#include "LRUCache.h"
#include "SpellCorrector.h"
#include "Parser.h"
#include "FileManager.h"
#include "Analytics.h"
#include "SearchResult.h"

using namespace std;

class SearchEngine{

private:

    vector<Document> documents;

    InvertedIndex index;

    Trie trie;

    Ranking ranking;

    LRUCache cache;

    SpellCorrector spellCorrector;

    Parser parser;

    Analytics analytics;

    unordered_set<string> vocabulary;

    vector<string> vocabularyList;

    int getTotalWords();

    pair<string,int> getLargestDocument();

    pair<string,int> getSmallestDocument();

    vector<SearchResult> lastSearchResults;

public:

    SearchEngine();

    void addDocument(Document document);

    vector<SearchResult> keywordSearch(string word);

    vector<string> autoComplete(string prefix);

    string spellSuggestion(string word);

    vector<Document> getDocuments();

    void loadDocuments(string folderPath);

    void showAnalytics();

    vector<SearchResult> booleanSearch(string query);

    string generateSnippet(
        int documentId,
        string keyword
    );

    vector<SearchResult> phraseSearch(string phrase);

    vector<SearchResult> fuzzySearch(string query);

    void addHistory(string query);

    vector<string> getHistory();

    void showTrending();
    
    void showIndexStatistics();

    void setLastSearchResults(vector<SearchResult> results);

    vector<SearchResult> getLastSearchResults();

};

#endif