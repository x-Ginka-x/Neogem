#include "text.h"

using namespace neo;
using namespace std;



TextEngine* neo::Text = NULL;


TextEngine::TextEngine(){

    if(TTF_Init()<0 && VIDEO_DEBUG) ERR(".VIDEO_DEBUG : Error while initializing TTF library");
    LoadFont("default_font", 16, "default_font");
}

TextEngine::~TextEngine(){
    for(auto it = _loaded_fonts.begin(); it != _loaded_fonts.end(); ++it){
        delete it->second;
    }
    TTF_Quit();
}


Font* TextEngine::LoadFont(std::string fontpath, int fontsize, std::string name){

    if(_loaded_fonts.find(name) != _loaded_fonts.end())
        return _loaded_fonts[name];

    TTF_Font* font = TTF_OpenFont(("fonts/" + fontpath + ".ttf").c_str(), fontsize);
    if(!font)
        return NULL;

    Font* ft = new Font(font, name);

    _loaded_fonts.insert(make_pair(name, ft));
    return ft;
}//LoadFont


void TextEngine::FreeFont(std::string name){

    if(_loaded_fonts.find(name) == _loaded_fonts.end())
        return;

    delete _loaded_fonts.find(name)->second;
    _loaded_fonts.erase(name);

}//FreeFont


Font* TextEngine::GetFont(std::string font){

    if(_loaded_fonts.find(font) != _loaded_fonts.end())
        return _loaded_fonts[font];

    return _loaded_fonts["default_font"];
}


void TextEngine::Write(int x, int y, int z, std::string text, Font* font){

//    if(_loaded_fonts.find(fontname) == _loaded_fonts.end()){
//        ERR("-text.cpp : Trying to write with non-existing font !");
//        return;
//    }
//
//    else if(_loaded_fonts.at(fontname)->IsLoaded() == false){
//        ERR("-text.cpp : Trying to write with existing but unloaded font !");
//        return;
//    }
//
//    else{
//        Font* font = _loaded_fonts[fontname];
        const char* characters = text.c_str();

        for(unsigned int i = 0; i < text.size(); ++i){

            FontGlyph* glyph = font->_glyph_cache[characters[i]];
            glyph->_texture->Draw(x, y, z, IMAGE_DRAW_FROM_TOPLEFT);
            x += glyph->_advance+1;

        }
//    }
}//Write


int TextEngine::CalculateLength(string text, Font* font){

    int length = 0;
    const char* characters = text.c_str();

    for(unsigned int i = 0; i < text.size(); i++){
        length += font->_glyph_cache[characters[i]]->_advance+1;
    }
    return length;
}
