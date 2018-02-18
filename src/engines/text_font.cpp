#include "text_font.h"

using namespace std;
using namespace neo;


FontGlyph::FontGlyph(){
    _minx = _miny = _maxx = _maxy = _advance = 0;
}

FontGlyph::~FontGlyph(){

}

Font::Font(TTF_Font* font, string name){

    _is_loaded = false;
    _name = name;
    _height = TTF_FontHeight(font);
    _ascent = TTF_FontAscent(font);
    _descent = TTF_FontDescent(font);
    _line_skip = TTF_FontLineSkip(font);
    if(!_LoadGlyphs(font))
        ERR("-text.cpp : Glyphs not loaded correctly !");
    else
        _is_loaded = true;
}//Constructor

Font::~Font(){

    for(map<Uint16, FontGlyph*>::iterator it = _glyph_cache.begin(); it != _glyph_cache.end(); ++it){
        delete (*it).second;
    }
    _glyph_cache.clear();
}//Destructor

bool Font::_LoadGlyphs(TTF_Font* font){

    char table[93] = {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ !\"#$%&'()*+,-./0123456789:;<=>?@[]_{|}~"};
    for(int i = 0; i < 92; ++i){

        Uint16 glyph = table[i];

        int miny,maxy,minx,maxx,advance;
        if(TTF_GlyphMetrics(font, glyph, &minx, &maxx, &miny, &maxy, &advance) < 0){
            ERR("-text.cpp : Problem with TTF_GlyphMetrics");
            return false;
        }

        /** We need to add color choice **/
        SDL_Color black = {255,255,255,100};

        SDL_Surface* surf = TTF_RenderGlyph_Blended(font, glyph, black);
        if(!surf){
            ERR("-text.cpp : Problem with TTF_RenderGlyph");
            return false;
        }
        string glyphname = _name + "_font_char" + table[i];
        Image* img = ImgManager->RegisterSurfaceAsImage(surf, glyphname);

        SDL_FreeSurface(surf);

        FontGlyph* fontglyph = new FontGlyph();
        fontglyph->_miny = miny;
        fontglyph->_minx = minx;
        fontglyph->_maxy = maxy;
        fontglyph->_maxx = maxx;
        fontglyph->_advance = advance;
        fontglyph->_texture = img;

        _glyph_cache.insert(make_pair(glyph, fontglyph));
    }
    return true;
}
