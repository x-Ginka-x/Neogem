#ifndef DEF_SHADER
#define DEF_SHADER



//#ifdef WIN32
    #include <glew.h>


//// Include Mac
//
//#elif __APPLE__
//    #define GL3_PROTOTYPES 1
//    #include <OpenGL/gl3.h>


// Include UNIX/Linux

//#else
//    #define GL3_PROTOTYPES 1
//    #include <GL3/gl3.h>

//#endif


#include <iostream>
#include <string>
#include <fstream>



class Shader
{
    public:

    Shader();
    Shader(Shader const&);
    Shader(std::string, std::string);
    ~Shader();

    Shader& operator=(Shader const&);

    bool Load();
    bool Compile(GLuint&, GLenum, std::string const&);
    GLuint getProgramID() const;


    private:

    GLuint _vertexID;
    GLuint _fragmentID;
    GLuint _programID;

    std::string _src_vertex;
    std::string _src_fragment;
};

#endif
