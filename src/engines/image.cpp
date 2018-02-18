#include "image.h"
#include "video.h"
#include "time.h"

using namespace glm;
using namespace neo;
using namespace std;

ImageManager* neo::ImgManager = NULL;
bool neo::IMAGE_DEBUG = true;

Image::Image(){

    float oglvertices[18] = {-2, -2, 0,   2, -2, 0,   2, 2, 0,
                        -2, -2, 0,   -2, 2, 0,   2, 2, 0};


    for(int i = 0 ;i < 18; i++) _oglvertices[i] = oglvertices[i];
    _display_size = coor2i(0,0);

}

Image::~Image(){


    glDeleteTextures(1,&_surface_id);
    glDeleteBuffers(1,&_oglvboid);
    glDeleteBuffers(1,&_oglvaoid);
    if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : delete image");
}

bool Image::LoadFromFile(std::string path){

    //These 3 constants are stored in VideoEngine as public members but should be static attributes of Image
    float _ogltexcoords[12];
    for(int i=0;i<12;i++) _ogltexcoords[i] = Video->_ogltexcoords[i];
    int _oglvertices_array_size = Video->_oglvertices_array_size;
    int _ogltexcoords_array_size = Video->_ogltexcoords_array_size;

    ///Load the SDL_Surface

    SDL_Surface* surface = IMG_Load(path.c_str());
    if(surface == NULL)
        return false;
    else if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : surface ok");

    _size = neo::coor2i(surface->w,surface->h);


    ///Bind the surface to a texture

    float oglvertices[18] = {(float)-_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0,
                        (float)-_size.x/2, (float)-_size.y/2, 0,   (float)-_size.x/2, (float)_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0};

    for(int i = 0 ;i < 18; i++) _oglvertices[i] = oglvertices[i];

    glGenTextures(1, &_surface_id);

    glBindTexture(GL_TEXTURE_2D,_surface_id);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D,0);

    SDL_FreeSurface(surface);


    ///Bind it to a VBO

    glGenBuffers(1, &_oglvboid);

    glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
    glBufferData(GL_ARRAY_BUFFER,_oglvertices_array_size+_ogltexcoords_array_size,0,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,_oglvertices_array_size,_oglvertices);
    glBufferSubData(GL_ARRAY_BUFFER,_oglvertices_array_size,_ogltexcoords_array_size,_ogltexcoords);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    ///Bind the VBO to a VAO

    glGenVertexArrays(1,&_oglvaoid);
    glBindVertexArray(_oglvaoid);
        glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(_oglvertices_array_size));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    _is_valid = true;

    return true;
}


bool Image::LoadFromFile_Coord(string path, int x, int y, int w, int h){

    ///OpenGL  helpers
    float _ogltexcoords[12];
    for(int i=0;i<12;i++) _ogltexcoords[i] = Video->_ogltexcoords[i];
    int _oglvertices_array_size = Video->_oglvertices_array_size;
    int _ogltexcoords_array_size = Video->_ogltexcoords_array_size;

    ///Load the SDL_Surface
    SDL_Surface* pngsurface = IMG_Load(path.c_str());
    if(pngsurface == NULL)
        return false;
    else if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : surface ok");

    if((x + w > pngsurface->w)||(y + h > pngsurface->h)){
        if(IMAGE_DEBUG) ERR(".IMAGE_DEBUG : Trying to load a too big chunk of image in Image::LoadFromFile() with name : " + path);
        return false;
    }

    ///Prepare a surface with the right dimensions
    _size = coor2i(w, h);
    SDL_Surface* new_surface = SDL_CreateRGBSurface(0, w, h, pngsurface->format->BitsPerPixel, pngsurface->format->Rmask, pngsurface->format->Gmask, pngsurface->format->Bmask, pngsurface->format->Amask);


    ///Select a chunk in the PNG

    SDL_Rect rect = {x,y,w,h};
    SDL_BlitSurface(pngsurface,&rect,new_surface,NULL);


    float oglvertices[18] = {(float)-_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0,
                        (float)-_size.x/2, (float)-_size.y/2, 0,   (float)-_size.x/2, (float)_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0};

    for(int i = 0 ;i < 18; i++) _oglvertices[i] = oglvertices[i];

    glGenTextures(1, &_surface_id);

    glBindTexture(GL_TEXTURE_2D,_surface_id);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,new_surface->w,new_surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,new_surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D,0);

    SDL_FreeSurface(new_surface);
    SDL_FreeSurface(pngsurface);


    ///Bind it to a VBO

    glGenBuffers(1, &_oglvboid);

    glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
    glBufferData(GL_ARRAY_BUFFER,_oglvertices_array_size+_ogltexcoords_array_size,0,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,_oglvertices_array_size,_oglvertices);
    glBufferSubData(GL_ARRAY_BUFFER,_oglvertices_array_size,_ogltexcoords_array_size,_ogltexcoords);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    ///Bind the VBO to a VAO

    glGenVertexArrays(1,&_oglvaoid);
    glBindVertexArray(_oglvaoid);
        glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(_oglvertices_array_size));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    _is_valid = true;

    return true;
}

bool Image::LoadFromSurface(SDL_Surface* surface){

    float _ogltexcoords[12];
    for(int i=0;i<12;i++) _ogltexcoords[i] = Video->_ogltexcoords[i];
    int _oglvertices_array_size = Video->_oglvertices_array_size;
    int _ogltexcoords_array_size = Video->_ogltexcoords_array_size;

    _size = coor2i(surface->w, surface->h);

    ///Bind the surface to a texture

    float oglvertices[18] = {(float)-_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)-_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0,
                        (float)-_size.x/2, (float)-_size.y/2, 0,   (float)-_size.x/2, (float)_size.y/2, 0,   (float)_size.x/2, (float)_size.y/2, 0};

    for(int i = 0 ;i < 18; i++) _oglvertices[i] = oglvertices[i];

    glGenTextures(1, &_surface_id);

    glBindTexture(GL_TEXTURE_2D,_surface_id);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,surface->w,surface->h,0,GL_RGBA,GL_UNSIGNED_BYTE,surface->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D,0);

    ///Bind it to a VBO

    glGenBuffers(1, &_oglvboid);

    glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
    glBufferData(GL_ARRAY_BUFFER,_oglvertices_array_size+_ogltexcoords_array_size,0,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER,0,_oglvertices_array_size,_oglvertices);
    glBufferSubData(GL_ARRAY_BUFFER,_oglvertices_array_size,_ogltexcoords_array_size,_ogltexcoords);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    ///Bind the VBO to a VAO

    glGenVertexArrays(1,&_oglvaoid);
    glBindVertexArray(_oglvaoid);
        glBindBuffer(GL_ARRAY_BUFFER,_oglvboid);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(0));
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(_oglvertices_array_size));
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);


    _is_valid = true;

    return true;
}


void Image::Draw(IMAGE_DRAW_FLAG flag){

//    Video->PushMatrix();
glm::mat4 matrix = Video->_modelview;
    if(_display_size.x == 0 || _display_size.y == 0)
        _display_size = _size;

    if(_display_size.x != _size.x || _display_size.y != _size.y)
        Video->ResizeCursor((float)_display_size.x / (float)_size.x, (float)_display_size.y / (float)_size.y);

    if(flag == IMAGE_DRAW_CENTERED){
        Video->Blit(_oglvaoid, _surface_id);
    }
    else if(flag == IMAGE_DRAW_FROM_TOPLEFT){
        Video->TranslateCursor(_display_size.x/2, _display_size.y/2, 0.0);
        Video->Blit(_oglvaoid, _surface_id);
    }
//    Video->PopMatrix();
    Video->_modelview = matrix;
    _display_size = _size;
}

void Image::SetDisplaySize(float x, float y){

    _display_size = coor2i(x, y);
}

coor2i Image::GetSize() const{

    return _size;
}

int Image::GetHeight() const{

    return _size.y;
}

int Image::GetWidth() const{

    return _size.x;
}

bool Image::IsValid() const{

    return _is_valid;
}




ImageManager::ImageManager(){

    if(IMAGE_DEBUG) LOG(".create imagemanager");
    GetImage("defaulttexture");
}

ImageManager::~ImageManager(){

    _ClearResources();
    if(IMAGE_DEBUG) LOG(".destroy imagemanager");
}

/** This function searches the _resource_holder to find the image named by loadscript
*** If it's not found it loads it
*** It will automatically look for "img/[loadscript].png
*** The format must be RGBA
*** If you put the mark <f< and a set of 4 coordinates, it will search for the designated chunk in the png file
*** example : GetImage("character<f<20,20,64,64") will load img/character.png and cut a chunk from x20, y20, with width and height of 64px **/

Image* ImageManager::GetImage(string loadscript){

    /*If we don't find the image in the memory*/

    if(_resource_holder.find(loadscript) == _resource_holder.end()){

        if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : Loading " + loadscript);

        /*Find if we need to cut a chunk*/

        size_t script_pos = loadscript.find("<f<");

        /*If yes */

        if(script_pos != loadscript.npos){

            string path = loadscript.substr(0,script_pos);//Holds the image path

            string str = loadscript.substr(script_pos + 3);
            int x,y,w,h;

            script_pos = str.find(",");//find coma
            x = atoi((str.substr(0,script_pos)).c_str());//extract int from the string
            str = str.substr(script_pos+1);//string is truncated from after coma

            script_pos = str.find(",");//find coma
            y = atoi((str.substr(0,script_pos)).c_str());
            str = str.substr(script_pos+1);

            script_pos = str.find(",");
            w = atoi((str.substr(0,script_pos)).c_str());
            str = str.substr(script_pos+1);

            h = atoi(str.c_str());

            /*Now that all coords are set, we load the image*/

            Image* img = new Image();
            img->LoadFromFile_Coord(("img/" + path + ".png"),x,y,w,h);

            if(img->IsValid()){
                _resource_holder.insert(make_pair(loadscript, img));
            }
            else{
                if(IMAGE_DEBUG) ERR(".IMAGE_DEBUG : Unable to load image at this location : " + ("img/" + path + ".png"));
                return _resource_holder.at("defaulttexture");
            }

        }
        else{

            Image* img = new Image();

            /*if the image is valid, we return it, else we return a default texture*/

            if(img->LoadFromFile("img/" + loadscript + ".png") == true)
                _resource_holder.insert(make_pair(loadscript, img));

            else{
                if(IMAGE_DEBUG) ERR(".IMAGE_DEBUG : Unable to load image at this location : " + loadscript);
                return _resource_holder.at("defaulttexture");
            }
        }
    }
    if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : Retrieving image : " + loadscript + " " + to_string(_resource_holder.at(loadscript)->_size.x));
    return _resource_holder.at(loadscript);
}

void ImageManager::_ClearResources(){

    for(auto it = _resource_holder.begin(); it != _resource_holder.end(); ++it){

        delete it->second;
        LOG(it->first);
    }
}

void ImageManager::ClearResources(){

    for(auto it = _resource_holder.begin(); it != _resource_holder.end(); ++it){

        std::string str = it->first;
        if(str == "defaulttexture"){

        }
        else if(str.substr(0, 12) == "default_font"){

        }
        else{
            delete it->second;
            LOG(str);
        }
    }
}



Image* ImageManager::RegisterSurfaceAsImage(SDL_Surface* surface, string name){

    Image* img = new Image();
    img->LoadFromSurface(surface);
    if(img == NULL)
        return NULL;
    else
        LOG(name);

    _resource_holder.insert(make_pair(name, img));
    return _resource_holder.at(name);
}




/** TEST **/

void neo::ImageDescriptor(ScriptObject* Script){

    std::string instruction = s_text;

    if(instruction == "new"){
        instruction = s_text;

        Image* img = ImgManager->GetImage(instruction);
        s_register(instruction, img);
    }
    else if(instruction == "end"){
        s_par("global");
    }
}
