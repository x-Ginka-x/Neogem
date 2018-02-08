#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include <string>

#include "kaguya/kaguya.hpp"

#include "utils.h"
#include "defs.h"

typedef void (*voidfunction)();

namespace neo{

extern bool SCRIPT_DEBUG;


class ScriptManager{

public:

    ScriptManager();
    ~ScriptManager();

    std::map<std::string, void*> _memory;
    void* _active_object;
    void* getactiveobject(){return _active_object;}

    template<class T>void registerobject(std::string name, T* object){

        _active_object = static_cast<void*>(object);
        _memory.insert(std::make_pair(name, _active_object));
    }

    void* getobject(std::string name){

        if(_memory.find(name) != _memory.end())
        return _memory[name];
        else return NULL;
    }



    size_t _cursor;
    bool _reached_end_of_file;
    void setendvar(bool var){_reached_end_of_file = var;}

    std::string _file;
    void readfile(std::string);
    std::string readtext();
    int readint();
    float readfloat();
    void _jumpline();
//    bool readspecialchar(const char*);
    DIRECTION readdirection();

    void _executeparagraph();
    void _executescript(std::string);
    void _executeline();
    std::string _paradigm;
    void setparadigm(std::string paradigm){_paradigm = paradigm;}
    std::map<std::string, voidfunction> _paradigms;


    /**Paradigms definitions**/




};//ScriptManager

extern ScriptManager* Script;
namespace paradigm{

void image();
void global();
void animation();
void map_texture();
void map_mesh();
void map_entity_static();
void map_entity_object();
void map_event();

}//paradigm
}//namespace

#endif // SCRIPT_H_INCLUDED
