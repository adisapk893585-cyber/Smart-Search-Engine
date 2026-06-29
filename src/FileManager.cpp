#include "../include/FileManager.h"

#include<fstream>
#include<filesystem>
#include <fstream>
#include <iomanip>
#include <ctime>

namespace fs = std::filesystem;

vector<Document> FileManager::loadDocuments(string folderPath){

    vector<Document> documents;

    int id=0;

    if(!fs::exists(folderPath)){
        cout << "Folder not found: " << folderPath << endl;
        return {};
    }

    for(auto &entry : fs::directory_iterator(folderPath)){
        if(entry.path().extension()==".txt"){

            ifstream fin(entry.path());

            if(!fin.is_open()) continue;

            string content;

            string line;

            while(getline(fin,line)){

                content+=line;
                content+="\n";
            }

            fin.close();

            string title=entry.path().stem().string();

            documents.push_back(

                Document(

                    id++,

                    title,

                    content
                )
            );
        }
    }

    return documents;
}

void FileManager::exportResultsToCSV(vector<SearchResult> results,vector<Document> documents)
{
    ofstream fout("exports/search_results.csv");

    if(!fout.is_open())
    {
        cout << "\nUnable to create CSV file.\n";
        return;
    }

    time_t now = time(nullptr);
    tm *local = localtime(&now);

    char buffer[25];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", local);

    fout << "Rank,Document,Score,Export Time\n";

    int rank = 1;

    for(auto &res : results)
    {
        fout << rank++ << ","
             << documents[res.documentId].title << ".txt,"
             << fixed << setprecision(4)
             << res.score << ","
             << buffer
             << "\n";
    }

    fout.close();

    cout << "\nSearch Results Exported Successfully!\n";
    cout << "Saved To : exports/search_results.csv\n";
}