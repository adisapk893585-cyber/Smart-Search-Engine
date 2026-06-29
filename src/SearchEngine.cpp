#include "../include/SearchEngine.h"
#include <algorithm>
#include <cctype>

SearchEngine::SearchEngine()

    : cache(10)

{
}

void SearchEngine::addDocument(Document document)
{

    documents.push_back(document);

    index.addDocument(document);

    vector<string> words = parser.parse(document.content);

    for (auto &word : words)
    {
        trie.insert(word);

        if (vocabulary.insert(word).second)
        {
            vocabularyList.push_back(word);
        }
    }
}

vector<SearchResult> SearchEngine::keywordSearch(string word)
{

    analytics.recordSearch(word);

    if (cache.contains(word))
    {

        analytics.recordCacheHit();

        vector<int> ids = cache.get(word);

        unordered_map<int, double> scores;

        for (int id : ids)
        {

            scores[id] = 1.0;
        }

        return ranking.rank(scores);
    }

    analytics.recordCacheMiss();

    vector<int> ids = index.search(word);

    cache.put(word, ids);

    unordered_map<int, double> scores;

    int totalDocuments = documents.size();

    int documentFrequency = index.getDocumentFrequency(word);

    for (int id : ids)
    {

        int frequency = index.getWordFrequency(word, id);

        int totalWords = parser.parse(documents[id].content).size();

        double score = ranking.calculateTFIDF(
            frequency,
            totalWords,
            totalDocuments,
            documentFrequency);

        scores[id] = score;
    }

    return ranking.rank(scores);
}

vector<string> SearchEngine::autoComplete(

    string prefix

)
{

    return trie.autoComplete(prefix);
}

string SearchEngine::spellSuggestion(string word)
{

    return spellCorrector.suggest(word,vocabularyList);
}

vector<Document> SearchEngine::getDocuments()
{

    return documents;
}

void SearchEngine::loadDocuments(string folderPath)
{

    documents = FileManager::loadDocuments(folderPath);

    for (auto &doc : documents)
    {

        index.addDocument(doc);

        vector<string> words = parser.parse(doc.content);

        for (auto &word : words)
        {

            trie.insert(word);

            if (vocabulary.insert(word).second)
            {

                vocabularyList.push_back(word);
            }
        }
    }
}

void SearchEngine::showAnalytics()
{

    analytics.showDashboard();
}

vector<SearchResult> SearchEngine::booleanSearch(string query)
{

    size_t pos;

    unordered_map<int, double> scores;

    if ((pos = query.find(" AND ")) != string::npos)
    {

        string first = query.substr(0, pos);
        string second = query.substr(pos + 5);

        vector<int> ids = index.andSearch(first, second);

        int totalDocuments = documents.size();

        int df1 = index.getDocumentFrequency(first);
        int df2 = index.getDocumentFrequency(second);

        for (int id : ids)
        {
            int totalWords = parser.parse(documents[id].content).size();

            double score = 0;

            int f1 = index.getWordFrequency(first, id);
            score += ranking.calculateTFIDF(
                f1,
                totalWords,
                totalDocuments,
                df1
            );

            int f2 = index.getWordFrequency(second, id);
            score += ranking.calculateTFIDF(
                f2,
                totalWords,
                totalDocuments,
                df2
            );

            scores[id] = score;
        }
    }

    else if ((pos = query.find(" OR ")) != string::npos)
    {

        string first = query.substr(0, pos);
        string second = query.substr(pos + 4);

        vector<int> ids = index.orSearch(first, second);

        int totalDocuments = documents.size();

        int df1 = index.getDocumentFrequency(first);
        int df2 = index.getDocumentFrequency(second);

        for (int id : ids)
        {
            int totalWords = parser.parse(documents[id].content).size();

            double score = 0;

            int f1 = index.getWordFrequency(first, id);
            score += ranking.calculateTFIDF(
                f1,
                totalWords,
                totalDocuments,
                df1
            );

            int f2 = index.getWordFrequency(second, id);
            score += ranking.calculateTFIDF(
                f2,
                totalWords,
                totalDocuments,
                df2
            );

            scores[id] = score;
        }
    }

    else if ((pos = query.find(" NOT ")) != string::npos)
    {

        string first = query.substr(0, pos);
        string second = query.substr(pos + 5);

        vector<int> ids = index.notSearch(first, second);

        int totalDocuments = documents.size();

        int df = index.getDocumentFrequency(first);

        for (int id : ids)
        {
            int totalWords = parser.parse(documents[id].content).size();

            int freq = index.getWordFrequency(first, id);

            scores[id] = ranking.calculateTFIDF(
                freq,
                totalWords,
                totalDocuments,
                df
            );
        }
    }

    return ranking.rank(scores);
}

string SearchEngine::generateSnippet(int documentId,string keyword)
{

    string content = documents[documentId].content;

    string lowerContent = content;

    transform(lowerContent.begin(), lowerContent.end(), lowerContent.begin(), ::tolower);

    transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);

    vector<string> words;

    size_t op;

    if((op = keyword.find(" and ")) != string::npos){

        words.push_back(keyword.substr(0, op));
        words.push_back(keyword.substr(op + 5));
    }
    else if((op = keyword.find(" or ")) != string::npos){

        words.push_back(keyword.substr(0, op));
        words.push_back(keyword.substr(op + 4));
    }
    else if((op = keyword.find(" not ")) != string::npos){

        words.push_back(keyword.substr(0, op));
        words.push_back(keyword.substr(op + 5));
    }
    else{

        words.push_back(keyword);
    }

    size_t pos = string::npos;

    for(auto &word : words){

        size_t current = lowerContent.find(word);

        if(current != string::npos){

            if(pos == string::npos || current < pos){

                pos = current;
            }
        }
    }
    if (pos == string::npos)
    {

        return "";
    }

    int start = max(0, (int)pos - 35);

    int length = min(120,(int)content.size() - start);

    string snippet = content.substr(start, length);

    for (char &ch : snippet)
    {

        if (ch == '\n')
        {

            ch = ' ';
        }
    }

    if (start != 0)
    {

        snippet = "..." + snippet;
    }

    if (start + length < (int)content.size())
    {

        snippet += "...";
    }

    return snippet;
}

vector<SearchResult> SearchEngine::phraseSearch(string phrase)
{

    analytics.recordSearch(phrase);

    unordered_map<int, double> scores;

    transform(phrase.begin(),phrase.end(),phrase.begin(),::tolower);

    for (auto &doc : documents)
    {

        string content = doc.content;

        string lowerContent = content;

        transform(lowerContent.begin(),lowerContent.end(),lowerContent.begin(),::tolower);

        if (lowerContent.find(phrase) != string::npos)
        {

            scores[doc.id] = 1.0;
        }
    }

    return ranking.rank(scores);
}

vector<SearchResult> SearchEngine::fuzzySearch(string query)
{

    string suggestion = spellSuggestion(query);

    if (suggestion.empty())
    {

        return {};
    }

    return keywordSearch(suggestion);
}

vector<string> SearchEngine::getHistory()
{

    return analytics.getHistory();
}

void SearchEngine::showTrending()
{

    analytics.showTrending();
}

void SearchEngine::addHistory(string query)
{

    analytics.addHistory(query);
}

int SearchEngine::getTotalWords()
{
    int total = 0;

    for(auto &doc : documents)
    {
        total += parser.parse(doc.content).size();
    }

    return total;
}

pair<string,int> SearchEngine::getLargestDocument()
{
    string name;
    int mx = -1;

    for(auto &doc : documents)
    {
        int words = parser.parse(doc.content).size();

        if(words > mx)
        {
            mx = words;
            name = doc.title;
        }
    }

    return {name,mx};
}

pair<string,int> SearchEngine::getSmallestDocument()
{
    string name;
    int mn = INT_MAX;

    for(auto &doc : documents)
    {
        int words = parser.parse(doc.content).size();

        if(words < mn)
        {
            mn = words;
            name = doc.title;
        }
    }

    return {name,mn};
}

void SearchEngine::showIndexStatistics()
{
    cout << "\n=========================================\n";
    cout << "         INDEX STATISTICS\n";
    cout << "=========================================\n\n";

    cout << "Documents Indexed : "
         << documents.size()
         << endl;

    cout << "Unique Terms      : "
         << vocabularyList.size()
         << endl;

    int totalWords = getTotalWords();

    cout << "Total Tokens      : "
         << totalWords
         << endl;

    if(!documents.empty())
    {
        cout << "Average Doc Size  : "
             << totalWords / documents.size()
             << " words"
             << endl;

        auto largest = getLargestDocument();
        auto smallest = getSmallestDocument();

        cout << "Largest Document  : "
            << largest.first
            << ".txt ("
            << largest.second
            << " words)\n";

        cout << "Smallest Document : "
            << smallest.first
            << ".txt ("
            << smallest.second
            << " words)\n";     
    }

    cout << "\n=========================================\n";
}

void SearchEngine::setLastSearchResults(vector<SearchResult> results)
{
    lastSearchResults = results;
}

vector<SearchResult> SearchEngine::getLastSearchResults()
{
    return lastSearchResults;
}