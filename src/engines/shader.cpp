#include "shader.h"
#include "../utils.h"

Shader::Shader() : _vertexID(0), _fragmentID(0), _programID(0), _src_vertex(), _src_fragment()
{
}


Shader::Shader(Shader const &srcshader)
{
    _src_vertex = srcshader._src_vertex;
    _src_fragment = srcshader._src_fragment;

    Load();
}


Shader::Shader(std::string vertexSource, std::string _src_fragment) : _vertexID(0), _fragmentID(0), _programID(0),
                                                                       _src_vertex(vertexSource), _src_fragment(_src_fragment)
{
}


Shader::~Shader()
{

    glDeleteShader(_vertexID);
    glDeleteShader(_fragmentID);
    glDeleteProgram(_programID);
}



Shader& Shader::operator=(Shader const &srcshader)
{

    _src_vertex = srcshader._src_vertex;
    _src_fragment = srcshader._src_fragment;

    Load();

    return *this;
}


bool Shader::Load()
{

    if(glIsShader(_vertexID) == GL_TRUE)
        glDeleteShader(_vertexID);

    if(glIsShader(_fragmentID) == GL_TRUE)
        glDeleteShader(_fragmentID);

    if(glIsProgram(_programID) == GL_TRUE)
        glDeleteProgram(_programID);



    if(!Compile(_vertexID, GL_VERTEX_SHADER, _src_vertex))
        return false;

    if(!Compile(_fragmentID, GL_FRAGMENT_SHADER, _src_fragment))
        return false;



    _programID = glCreateProgram();



    glAttachShader(_programID, _vertexID);
    glAttachShader(_programID, _fragmentID);


    glBindAttribLocation(_programID, 0, "in_Vertex");
    glBindAttribLocation(_programID, 1, "in_Color");
    glBindAttribLocation(_programID, 2, "in_TexCoord0");



    glLinkProgram(_programID);


    GLint errorlink(0);
    glGetProgramiv(_programID, GL_LINK_STATUS, &errorlink);


    if(errorlink != GL_TRUE)
    {

        GLint err_size(0);
        glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &err_size);


        char *error = new char[err_size + 1];


        glGetShaderInfoLog(_programID, err_size, &err_size, error);
        error[err_size] = '\0';


        neo::ERR(error);

        delete[] error;
        glDeleteProgram(_programID);

        return false;
    }



    else
        return true;
}


bool Shader::Compile(GLuint &shader, GLenum type, std::string const &srcfile)
{


    shader = glCreateShader(type);


    if(shader == 0)
    {
        neo::ERR(".SHADER.CPP : Wrong type of shader");
        return false;
    }


    std::ifstream file(srcfile.c_str());


    if(!file)
    {
        neo::ERR(".SHADER.CPP : Tried to load shader from no- existing file named : "+ srcfile);
        glDeleteShader(shader);

        return false;
    }



    std::string line;
    std::string srccode;


    while(getline(file, line))
        srccode += line + '\n';


    file.close();


    const GLchar* srccode_str = srccode.c_str();


    glShaderSource(shader, 1, &srccode_str, 0);


    glCompileShader(shader);


    GLint compile_err(0);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_err);



    if(compile_err != GL_TRUE)
    {

        GLint err_size(0);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &err_size);



        char *error = new char[err_size + 1];


        glGetShaderInfoLog(shader, err_size, &err_size, error);
        error[err_size] = '\0';


        std::cout << error << std::endl;

        delete[] error;
        glDeleteShader(shader);

        return false;
    }



    else
        return true;
}



GLuint Shader::getProgramID() const
{
    return _programID;
}
