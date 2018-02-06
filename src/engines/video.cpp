
#include "video.h"

bool neo::VIDEO_DEBUG = true;
neo::VideoEngine* neo::Video = NULL;

neo::VideoEngine::VideoEngine(){

    //Initialize some constants used by the Image class .
    //Should be static Image attributes but I could not initialize the _ogltexcoords array properly as a static attribute.

    float ogltexcoords[12] = {0, 0,   1, 0,   1, 1,   0, 0,   0, 1,   1, 1};

    for(int i = 0;i<12;i++) _ogltexcoords[i] = ogltexcoords[i];

    _ogltexcoords_array_size = 12 * sizeof(float);
    _oglvertices_array_size = 18 * sizeof(float);

}

neo::VideoEngine::~VideoEngine(){

    SDL_GL_DeleteContext(_glcontext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

bool neo::VideoEngine::Initialize(){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        if(neo::VIDEO_DEBUG) neo::ERR(".VIDEO_DEBUG : Problem while initializing SDL in Video::Initialize()\n -> " + neo::to_string(SDL_GetError()));
        SDL_Quit();
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

    _window =SDL_CreateWindow("Neogem", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, neo::SCREEN_WIDTH, neo::SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if(_window == 0){
        if(neo::VIDEO_DEBUG) ERR(".VIDEO_DEBUG : Unable to create window in Video::Initialize()");
        SDL_Quit();
        return false;
    }

    _glcontext = SDL_GL_CreateContext(_window);

    if(_glcontext == 0){
        if(neo::VIDEO_DEBUG) neo::ERR(".VIDEO_DEBUG : Unable to create OpenGL context in Video::Initialize()\n -> " + neo::to_string(SDL_GetError()));
        SDL_DestroyWindow(_window);
        SDL_Quit();
        return false;
    }

    GLenum glewinitvar(glewInit());


    if(glewinitvar != GLEW_OK){

        if(neo::VIDEO_DEBUG) neo::ERR(".VIDEO_DEBUG : Error while initializing Glew \n -> " + neo::to_string(glewGetErrorString(glewinitvar)));

        SDL_GL_DeleteContext(_glcontext);
        SDL_DestroyWindow(_window);
        SDL_Quit();

        return false;
    }

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    SDL_GL_SetSwapInterval(1);

    _projection = glm::ortho(0.0f, (float)neo::SCREEN_WIDTH,(float)neo::SCREEN_HEIGHT,0.0f, 0.0f, 1000.0f);
    _modelview = glm::lookAt(glm::vec3(0.0,0.0,1000.0),glm::vec3(0.0,0.0,0.0),glm::vec3(0,1,0));

    _texture_shader = Shader("src/Shaders/texture.vert", "src/Shaders/texture.frag");
    _texture_shader.Load();

    return true;
}//Initialize()

void neo::VideoEngine::Display(){

    SDL_GL_SwapWindow(_window);
}

void neo::VideoEngine::Clear(){

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    SetCamPos(0,0,1000.0);
    SaveMatrix();
}

void neo::VideoEngine::Blit(GLuint vaoid, GLuint textureid){

    glUseProgram(_texture_shader.getProgramID());

            glBindVertexArray(vaoid);


            glUniformMatrix4fv(glGetUniformLocation(_texture_shader.getProgramID(),"projection"),1,GL_FALSE,glm::value_ptr(_projection));
            glUniformMatrix4fv(glGetUniformLocation(_texture_shader.getProgramID(),"modelview"),1,GL_FALSE,glm::value_ptr(_modelview));

            glBindTexture(GL_TEXTURE_2D,textureid);

            glDrawArrays(GL_TRIANGLES,0,6);

            glBindTexture(GL_TEXTURE_2D,0);

            glBindVertexArray(0);

        glUseProgram(0);
}

void neo::VideoEngine::SetCursorPos(float x, float y, float z){


    ResetMatrix();
    _modelview = glm::translate(_modelview,glm::vec3(x,y,z));
}

void neo::VideoEngine::TranslateCursor(float x, float y, float z){

    _modelview = glm::translate(_modelview,glm::vec3(x,y,z));
}

void neo::VideoEngine::SetCamPos(float x, float y, float z){

    _modelview = glm::lookAt(glm::vec3(x,y,z),glm::vec3(0.0,0.0,0.0),glm::vec3(0,1,0));
}

void neo::VideoEngine::SaveMatrix(){

    _modelview_save = _modelview;
}

void neo::VideoEngine::ResetMatrix(){

    _modelview = _modelview_save;
}
