#include "../include/Parser.h"

#include <fstream>
#include <sstream>
#include <cctype>

Parser::Parser(){

    loadStopWords();
}

void Parser::loadStopWords(){

    ifstream fin("data/stopwords.txt");

    if(!fin.is_open()){
        cout<<"Unable to open stopwords.txt\n";
        return;
    }

    string word;

    while(fin>>word){

        stopWords.insert(word);
    }

    fin.close();
}

string Parser::toLowerCase(string text){

    for(char &ch:text){

        ch=tolower(ch);
    }

    return text;
}

string Parser::removePunctuation(string text){

    for(char &ch:text){

        if(ispunct(ch))
            ch=' ';
    }

    return text;
}

vector<string> Parser::tokenize(string text){

    vector<string> words;

    stringstream ss(text);

    string word;

    while(ss>>word){

        words.push_back(word);
    }

    return words;
}

vector<string> Parser::parse(string text){

    text=toLowerCase(text);

    text=removePunctuation(text);

    vector<string> words=tokenize(text);

    vector<string> result;

    for(auto &word:words){

        if(stopWords.find(word)==stopWords.end()){

            result.push_back(word);
        }
    }

    return result;
}