#ifndef VIDEO_H_INCLUDED
#define VIDEO_H_INCLUDED

#include <SDL.h>

//#ifdef WIN32
    #include <glew.h>
//#else
//    #define GL3_PROTOTYPES 1
//    #include <GL3/gl3.h>
//#endif // WIN32

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>
#include <map>

#include "../defs.h"
#include "../utils.h"
#include "shader.h"


namespace neo{

extern bool VIDEO_DEBUG;

extern VideoEngine* Video;

class VideoEngine{

private:

    SDL_Window* _window;
    SDL_GLContext _glcontext;

    glm::mat4 _projection;
    glm::mat4 _modelview_save;
    Shader _texture_shader;

    std::vector<glm::mat4> _matrices;

public:

    glm::mat4 _modelview;
    VideoEngine();
    ~VideoEngine();

    bool Initialize();
    void Display();
    void Clear();

    void LoadShader(std::string, std::string);
    void Blit(GLuint,GLuint);

    void SetCursorPos(float,float,float);
    void TranslateCursor(float,float,float);
    void ResizeCursor(float, float);
    void SetCamPos(float,float,float);
    void SaveMatrix();
    void ResetMatrix();
    void PushMatrix();
    void PopMatrix();

    float _ogltexcoords[12];
    int _oglvertices_array_size;
    int _ogltexcoords_array_size;



};//VideoEngine

}//namespace

#endif // VIDEO_H_INCLUDED
