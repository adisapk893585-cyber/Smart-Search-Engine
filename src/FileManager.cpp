#include "../include/FileManager.h"

#include<fstream>
#include<filesystem>

namespace fs = std::filesystem;

vector<Document> FileManager::loadDocuments(

    string folderPath

){

    vector<Document> documents;

    int id=0;

    if(!fs::exists(folderPath)){

        cout << "Folder not found: " << folderPath << endl;

        return {};
    }

    for(auto &entry : fs::directory_iterator(folderPath)){

        if(entry.path().extension()==".txt"){

            ifstream fin(entry.path());

            if(!fin.is_open())
                continue;

            string content;

            string line;

            while(getline(fin,line)){

                content+=line;
                content+="\n";
            }

            fin.close();

            string title=

                entry.path().stem().string();

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