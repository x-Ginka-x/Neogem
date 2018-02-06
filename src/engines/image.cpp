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

//    float ogltexcoords[12] = {0, 0,   1, 0,   1, 1,
//                        0, 0,   0, 1,   1, 1};

    for(int i = 0 ;i < 18; i++) _oglvertices[i] = oglvertices[i];

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

    if(flag == IMAGE_DRAW_CENTERED){
        Video->Blit(_oglvaoid, _surface_id);
        cout<<_size.x<<endl;
    }
    else if(flag == IMAGE_DRAW_FROM_TOPLEFT){
        Video->TranslateCursor(_size.x/2, _size.y/2, 0.0);
        Video->Blit(_oglvaoid, _surface_id);
        Video->TranslateCursor(-_size.x/2, -_size.y/2,0.0);
    }
}

coor2i Image::GetSize() const{

    return _size;
}

int Image::GetHeight() const{

    return _size.x;
}

int Image::GetWidth() const{

    return _size.y;
}

bool Image::IsValid() const{

    return _is_valid;
}




ImageManager::ImageManager(){

    if(IMAGE_DEBUG) LOG(".create imagemanager");
    GetImage("defaulttexture");
}

ImageManager::~ImageManager(){

    Clear();
    if(IMAGE_DEBUG) LOG(".destroy imagemanager");
}

Image* ImageManager::GetImage(string loadscript){ /**TODO : Simplify the process and add multiple chunks loading **/

    if(_resource_holder.find(loadscript) == _resource_holder.end()){

        if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : Loading " + loadscript);

        size_t script_pos = loadscript.find("<f<");

        if(script_pos != loadscript.npos){

            string path = loadscript.substr(0,script_pos);

            string str = loadscript.substr(script_pos + 3);
            int x,y,w,h;
            string strx,stry,strw,strh;

            script_pos = str.find(",");//find coma
            strx = str.substr(0,script_pos);//x = what's between beginning and coma
            str = str.substr(script_pos+1);//string is truncated from after coma

            script_pos = str.find(",");//find coma
            stry = str.substr(0,script_pos);
            str = str.substr(script_pos+1);

            script_pos = str.find(",");
            strw = str.substr(0,script_pos);
            strh = str.substr(script_pos+1);
            x = atoi(strx.c_str());
            y = atoi(stry.c_str());
            w = atoi(strw.c_str());
            h = atoi(strh.c_str());

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
            img->LoadFromFile("img/" + loadscript + ".png");

            if(img->IsValid()){
                _resource_holder.insert(make_pair(loadscript, img));
            }
            else{
                if(IMAGE_DEBUG) ERR(".IMAGE_DEBUG : Unable to load image at this location : " + loadscript);
                return _resource_holder.at("defaulttexture");
            }
        }
    }
    if(IMAGE_DEBUG) LOG(".IMAGE_DEBUG : Retrieving image : " + loadscript + " " + to_string(_resource_holder.at(loadscript)->_size.x));
    return _resource_holder.at(loadscript);
}

Image* ImageManager::RegisterSurfaceAsImage(SDL_Surface* surface, string name){

    Image* img = new Image();
    img->LoadFromSurface(surface);

    _resource_holder.insert(make_pair(name, img));
    return _resource_holder.at(name);
}

void ImageManager::Clear(){

    for(auto it = _resource_holder.begin();it!=_resource_holder.end();++it){
        delete it->second;
        LOG(it->first);
    }
}
