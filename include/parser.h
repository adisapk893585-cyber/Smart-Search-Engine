#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class Parser{

private:

    unordered_set<string> stopWords;

public:

    Parser();

    void loadStopWords();

    string toLowerCase(string text);

    string removePunctuation(string text);

    vector<string> tokenize(string text);

    vector<string> parse(string text);

};

#endif