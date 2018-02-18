#include "text_box.h"
#include "text.h"

using namespace std;
using namespace neo;

TextBox::TextBox(){


}

TextBox::TextBox(int x, int y){

    _size.x = x;
    _size.y = y;
}

TextBox::~TextBox(){

}

void TextBox::SetSize(int x, int y){

    _size.x = x;
    _size.y = y;
}

void TextBox::Load(string text, Font* font){

    string word;
    string new_string;
    size_t position, old_position;
    int total_length = 0;
    int length = 0;
    int lineskip = font->_line_skip;
    int height = lineskip;
    old_position = 0;

    while(position != text.npos){
        position = text.find(" ", old_position);
        word = text.substr(old_position, position - old_position) + " ";
        LOG(word);
        length = Text->CalculateLength(word, font);
        old_position = position + 1;
        if(total_length + length <= _size.x){
            new_string += word;
            total_length += length;
        }
        else{
            new_string += "\n";
            new_string += word;
            total_length = length;
            height += lineskip;
        }

    }
    _formated_text = new_string;
    _font = font;
    _size.y = height;
    ERR("good");
    ERR(new_string);
}



void TextBox::Draw(){

    size_t position, oldposition;
    oldposition = 0;
    std::string str;
    int length;
    int lineskip = _font->_line_skip;
    Video->TranslateCursor(-_size.x/2, -_size.y/2,0);
    while (position != _formated_text.npos){
        position = _formated_text.find("\n", oldposition);
        str = _formated_text.substr(oldposition, position - oldposition);
        oldposition = position + 1;
        Text->Write(str, _font);
        length = Text->CalculateLength(str, _font);
        Video->TranslateCursor(-length, lineskip, 0);
    }
}