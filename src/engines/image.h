#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
//#ifdef WIN32
#include <glew.h>

//#else
//    #define GL3_PROTOTYPES 1
//    #include <GL3/gl3.h>

//#endif

#ifndef BUFFER_OFFSET

    #define BUFFER_OFFSET(offset)((char*)NULL+(offset))

#endif // BUFFER_OFFSET

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdlib>
#include <string>
#include <map>

#include "../defs.h"
#include "../utils.h"
#include "../Shader.h"


namespace neo{

enum IMAGE_DRAW_FLAG{

    IMAGE_DRAW_FROM_BOTTOM,
    IMAGE_DRAW_CENTERED,
    IMAGE_DRAW_FROM_TOPLEFT
};

extern bool IMAGE_DEBUG;

class Image{
friend class ImageManager;

private:

    Image();
    ~Image();
    //OpenGL attributes
    float _oglvertices[18];
    GLuint _oglvboid;
    GLuint _oglvaoid;
    GLuint _surface_id;

    coor2i _size;

    bool _is_valid;

public:


    bool LoadFromFile(std::string);
    bool LoadFromFile_Coord(std::string,int,int,int,int);
    bool LoadFromSurface(SDL_Surface*);
    void Draw(IMAGE_DRAW_FLAG flag = IMAGE_DRAW_CENTERED);

    coor2i GetSize() const;
    int GetHeight() const;
    int GetWidth() const;
    bool IsValid() const;

};//Image



class ImageManager{

private:


public:
std::map<std::string, Image*> _resource_holder;
void err(){ERR("coucou");}
    ImageManager();
    ~ImageManager();

    Image* GetImage(std::string);
    Image* RegisterSurfaceAsImage(SDL_Surface*, std::string);
    void Clear();

};//ImageManager

extern ImageManager* ImgManager;

}//namespace

#endif // IMAGE_H_INCLUDED
