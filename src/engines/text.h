#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include "SDL_ttf.h"
#include <string>
#include <map>

#include "../defs.h"
#include "../utils.h"

#include "video.h"
#include "image.h"
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

class FontGlyph{

public:
    FontGlyph();
    ~FontGlyph();

    int _miny, _maxy;
    int _minx, _maxx;
    int _advance;
    Image* _texture;
};

class Font{

public:
    Font(TTF_Font*,std::string);
    ~Font();

    int _height;
    int _ascent,_descent;
    int _line_skip;

    bool IsLoaded() const {return _is_loaded;}

    std::map<Uint16, FontGlyph*> _glyph_cache;

private:
    std::string _name;
    bool _is_loaded;
    bool _LoadGlyphs(TTF_Font*);
};


class TextEngine{

public:

    TextEngine();
    ~TextEngine();

    bool LoadFont(std::string, int, std::string);
    void FreeFont(std::string);

//    void RenderText(std::string, TextStyle*, int aligned, int speed, int);
    void Write(std::string, std::string);
    SDL_Surface* RenderText(std::string, std::string);

private:

    std::map<std::string, Font*> _loaded_fonts;

};//class TextEngine


extern TextEngine* Text;
}//namespace
#endif // TEXT_H_INCLUDED
