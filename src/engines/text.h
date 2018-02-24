#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED


#include "../defs.h"
#include "../utils.h"

#include "video.h"
#include "image.h"
#include "text_font.h"
//
namespace neo{

//
///** Question to ask before rendering the text :
//
//        -Aligned ?
//            .centered
//        or : left align
//        or : right align
//
//        -Style ?
//            .font
//            .color
//            .shadow
//
//        -Speed ?
//            .insta-render
//        or : progressive display
//                ->speed of displaying
//
//        -Borders ?
//            .coordinates
//
//        -Cut ?
//            .straight display
//            .cut in parts
//*/
//
////class FontProperties{
////
////    TTF_Font* _font;
////    int _hinting, _kerning, _style, _outline;
////
////};
////
////class TextStyle{
////
////    bool _is_shadowed;
////    FontProperties* _font_properties;
////    int4 _color;
////
////};
////
////
////class TextTexture{
////
////public:
////
////    TextTexture();
////    ~TextTexture();
////
////    void Draw();
////
////private:
////    std::vector<TextPart*> _text_parts;
////
////};
////
////
////class RenderedTextLine{
////
////    TextTexture* _texture;
////};
////
//

class TextEngine{

public:

    TextEngine();
    ~TextEngine();

    Font* LoadFont(std::string, int, std::string);
    void FreeFont(std::string);
    Font* GetFont(std::string);

//    void RenderText(std::string, TextStyle*, int aligned, int speed, int);
    void Write(int, int,int, std::string text, Font* font);
    SDL_Surface* RenderText(std::string, std::string);

    int CalculateLength(std::string, Font*);

private:

    std::map<std::string, Font*> _loaded_fonts;

};//class TextEngine


extern TextEngine* Text;
}//namespace
#endif // TEXT_H_INCLUDED
