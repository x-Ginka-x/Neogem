#ifndef TEXT_FONT_H_INCLUDED
#define TEXT_FONT_H_INCLUDED

#include "SDL_ttf.h"
#include <string>
#include <map>

#include "image.h"

namespace neo{

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

}//namespace
#endif // TEXT_FONT_H_INCLUDED
