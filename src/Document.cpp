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