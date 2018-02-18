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
#include "shader.h"


#include "script.h"


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
    coor2i _display_size;

    bool _is_valid;

public:

    /** Loads an image from a png file (must be RGBA) **/
    bool LoadFromFile(std::string);

    /**Loads an image from a chunk of a png file, with specific coords **/
    bool LoadFromFile_Coord(std::string,int,int,int,int);

    /** Loads an image from an already existing SDL_Surface* (used in TextManager) **/
    bool LoadFromSurface(SDL_Surface*);

    /**Draws the image at the current VideoManager's cursor location **/
    /** flags are :
        .IMAGE_DRAW_CENTERED(default) -> draws from the center of the image
        .IMAGE_DRAW_FROM_BOTTOM -> draws from the point at the center of the bottom side of the image
        .IMAGE_DRAW_FROM_TOPLEFT -> 'standard' 2D blitting from top left corner (like SDL_Blit)
    **/

    void Draw(IMAGE_DRAW_FLAG flag = IMAGE_DRAW_CENTERED);

    void SetDisplaySize(float, float);
    coor2i GetSize() const;
    int GetHeight() const;
    int GetWidth() const;
    bool IsValid() const;

};//Image



class ImageManager{

private:

    /** Used in destructor to delete all resources **/
    void _ClearResources();

public:

    ImageManager();
    ~ImageManager();

    /** Image memory **/
    std::map<std::string, Image*> _resource_holder;

    /** Returns an image of the memory or tries to load it from a file **/
    Image* GetImage(std::string);

    /** Used in TextManager **/
    Image* RegisterSurfaceAsImage(SDL_Surface*, std::string);

    /** Cleans the memory but keeps default texture and default font **/
    void ClearResources();

    /** TEST **/

};//ImageManager

void ImageDescriptor(ScriptObject*);

/** Only one ImageManager is used throughout the entire program but it could be good to be able to clean it from time to time **/
extern ImageManager* ImgManager;

}//namespace

#endif // IMAGE_H_INCLUDED
