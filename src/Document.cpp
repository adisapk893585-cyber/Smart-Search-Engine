#include "../include/Document.h"

Document::Document(){

    id = 0;
    title = "";
    content = "";
}

Document::Document(
    int id,
    string title,
    string content
){

    this->id = id;
    this->title = title;
    this->content = content;
}

string Document::getSnippet(string keyword){

    size_t pos = content.find(keyword);

    if(pos == string::npos){

        return "";
    }

    int start = max(0, (int)pos - 30);

    int len = min(

        100,

        (int)content.size() - start

    );

    return content.substr(

        start,

        len
    );
}