#ifndef TEXT_BOX_H_INCLUDED
#define TEXT_BOX_H_INCLUDED

#include "text_font.h"

namespace neo{


class TextBox{

public:

    TextBox();
    TextBox(int, int);
    ~TextBox();

    void Load(std::string, Font* font);
    void Draw(int,int,int);

    void SetFont(Font* font){_font = font;}
    void SetSize(int, int);
    coor2i GetSize(){return _size;}

private:

    coor2i _size;
    std::string _formated_text;
    Font* _font;

};//TextBox


}//namespace

#endif // TEXT_BOX_H_INCLUDED
