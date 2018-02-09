#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

#include <string>

#include "../utils.h"
#include "../defs.h"


#define s_text Script->readtext()
#define s_int Script->readint()
#define s_float Script->readfloat()
#define s_enum_DIRECTION Script->readdirection()
#define s_active Script->getactiveobject()
#define s_get Script->getobject
#define s_register Script->registerobject
#define s_par Script->setparadigm
#define s_animation static_cast<Animation*>
#define s_image static_cast<Image*>
#define s_mesh static_cast<Mesh*>
#define s_texture static_cast<MapTexture*>
#define s_staticentity static_cast<StaticEntity*>
#define s_objectentity static_cast<ObjectEntity*>
#define s_actorentity static_cast<ActorEntity*>
#define s_eventstring static_cast<MapEventString*>


typedef void (*paradigmfunction)(neo::ScriptObject*);

namespace neo{

extern bool SCRIPT_DEBUG;


class ScriptObject{

public:

    ScriptObject();
    ~ScriptObject();

    void BindParadigm(std::string, paradigmfunction);

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
    std::map<std::string, paradigmfunction> _paradigms;






};//ScriptObject

extern ScriptObject* Script;

void GlobalDescriptor(ScriptObject*);

}//namespace

#endif // SCRIPT_H_INCLUDED
