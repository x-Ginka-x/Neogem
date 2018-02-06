#include "text.h"

using namespace neo;
using namespace std;

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
        SDL_Color black = {50,50,50,50};
        SDL_Surface* surf = TTF_RenderGlyph_Blended(font, glyph, black);
        if(!surf){
            ERR("-text.cpp : Problem with TTF_RenderGlyph");
            return false;
        }
        string glyphname = _name + "_font_char" + table[i];
        Image* img = ImgManager->RegisterSurfaceAsImage(surf, glyphname);
//img=ImgManager->GetImage("block");
//        if(img->LoadFromSurface(surf) == false){
//            ERR("-text.cpp : Could not create a glyph : " + table[i]);
//            return false;
//        }
        SDL_FreeSurface(surf);

        FontGlyph* fontglyph = new FontGlyph();
        fontglyph->_miny = miny;
        fontglyph->_minx = minx;
        fontglyph->_maxy = maxy;
        fontglyph->_maxx = maxx;
        fontglyph->_advance = advance;
        fontglyph->_texture = img;

        _glyph_cache.insert(std::pair<Uint16, FontGlyph*>(table[i], fontglyph));
    }
    return true;
}


TextEngine* neo::Text = NULL;


TextEngine::TextEngine(){

    if(TTF_Init()<0 && VIDEO_DEBUG) ERR(".VIDEO_DEBUG : Error while initializing TTF library");
    LoadFont("minecraft", 32, "default_font");
}

TextEngine::~TextEngine(){
    for(auto it = _loaded_fonts.begin(); it != _loaded_fonts.end(); ++it){
        delete it->second;
    }
    TTF_Quit();
}


bool TextEngine::LoadFont(std::string fontpath, int size, std::string name){

    if(_loaded_fonts.find(name) != _loaded_fonts.end())
        return false;

    TTF_Font* font = TTF_OpenFont(("fonts/" + fontpath + ".ttf").c_str(), size);
    if(!font)
        return false;

    Font* ft = new Font(font, fontpath);

    _loaded_fonts.insert(make_pair(name, ft));
    return true;
}//LoadFont


void TextEngine::FreeFont(std::string name){

    if(_loaded_fonts.find(name) == _loaded_fonts.end())
        return;

    delete _loaded_fonts.find(name)->second;
    _loaded_fonts.erase(name);

}//FreeFont


void TextEngine::Write(std::string text, std::string fontname){

    if(_loaded_fonts.find(fontname) == _loaded_fonts.end()){
        ERR("-text.cpp : Trying to write with non-existing font !");
        return;
    }

    else if(_loaded_fonts.at(fontname)->IsLoaded() == false){
        ERR("-text.cpp : Trying to write with existing but unloaded font !");
        return;
    }

    else{
        Font* font = _loaded_fonts[fontname];
        const char* characters = text.c_str();

        for(unsigned int i = 0; i < text.size(); ++i){
            FontGlyph* glyph = font->_glyph_cache.find((Uint16)characters[i])->second;
            glyph->_texture->Draw(IMAGE_DRAW_FROM_TOPLEFT);
            Video->TranslateCursor((float)glyph->_advance,0.0,0.0);

        }
    }
}//Write
