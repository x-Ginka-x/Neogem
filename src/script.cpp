#include "script.h"
#define LUACLASS kaguya::UserdataMetatable

#include <iostream>
#include <fstream>

#include "engines/image.h"
#include "engines/animation.h"
#include "modes/map/map.h"
#include "modes/map/map_mesh.h"
#include "modes/map/map_texture.h"
#include "modes/map/map_entity.h"

#define s_text Script->readtext()
#define s_int Script->readint()
#define s_float Script->readfloat()
#define s_active Script->getactiveobject()
#define s_get Script->getobject
#define s_register Script->registerobject
#define s_par Script->setparadigm
#define s_animation static_cast<Animation*>
#define s_image static_cast<Image*>
#define s_mesh static_cast<Mesh*>
#define s_texture static_cast<MapTexture*>
#define s_staticentity static_cast<StaticEntity*>


using namespace neo;

bool neo::SCRIPT_DEBUG = true;
ScriptManager* neo::Script = NULL;


void paradigm::image(){

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


void paradigm::global(){

    std::string instruction = s_text;
    ERR(instruction);
    if(instruction == "global"){
        Script->setendvar(false);
    }
    else if(instruction == "end"){
        Script->setendvar(true);
    }
    else if(instruction == ""){

    }
    else{
        s_par(instruction);
    }
}



void paradigm::animation(){

    std::string instruction = s_text;

    if(instruction == "new"){
        Animation* anim = new Animation();
        s_register(s_text, anim);
    }
    else if(instruction == "frame"){
        Animation* anim = s_animation(s_active);
        std::string str = s_text;
        anim->PushFrame(ImgManager->GetImage(str), s_int);
    }
    else if(instruction == "time"){
        Animation* anim = s_animation(s_active);
        anim->SetTimesToPlay(s_int);
    }
    else if(instruction == "end"){
        s_par("global");
    }
}


void paradigm::map_mesh(){

    std::string instruction = s_text;
    if(instruction == "new"){
        Mesh* mesh = neo_map::_current_map->GetPhysicsManager()->CreateMesh();
        s_register(s_text, mesh);
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "pos"){
        Mesh* mesh = s_mesh(s_active);
        float x = s_float;
        float y = s_float;
        float z = s_float;
        mesh->SetPos(x, y, z);
        LOG(to_string(mesh->GetPos().x));
        Mesh* meshptr = s_mesh(s_active);
        LOG(to_string(meshptr->GetPos().x));

    }
    else if(instruction == "size"){
        Mesh* mesh = s_mesh(s_active);
        float x = s_float;
        float y = s_float;
        float z = s_float;
        mesh->SetSize(x, y, z);
    }
    else{

    }
}


void paradigm::map_texture(){

    std::string instruction = s_text;
    if(instruction == "new"){
        MapTexture* tex = neo_map::_current_map->GetViewManager()->CreateMapTexture();
        s_register(s_text, tex);
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "animation"){
        MapTexture* tex = s_texture(s_active);
        std::string name = s_text;
        std::string animname = s_text;
        Animation* anim = s_animation(s_get(name));
        tex->AddAnimation(animname, anim);
    }
    else{

    }
}


void paradigm::map_entity_static(){

    std::string instruction = s_text;

    if(instruction == "new"){
        std::string name = s_text;
        if(name == "staticentity"){
            name = s_text;
            StaticEntity* obj = neo_map::_current_map->CreateStaticEntity(name);
            s_register(name, obj);
        }
        else{

        }
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "texture"){
        MapTexture* tex = s_texture(s_get(s_text));
        StaticEntity* obj = s_staticentity(s_active);
        obj->LinkMapTexture(tex);
    }

    else if(instruction == "mesh"){
        Mesh* mesh = (Mesh*)s_get(s_text);
        StaticEntity* obj = s_staticentity(s_active);
        obj->LinkMesh(mesh);
        mesh->SetStatic(true);
        std::cout<<mesh->GetPos().x<<std::endl;
    }
    else{

    }
}

ScriptManager::ScriptManager(){
    _cursor = 0;
    _paradigms.insert(std::make_pair<std::string,voidfunction>("image",(voidfunction)&paradigm::image));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("global",(voidfunction)&paradigm::global));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("animation",(voidfunction)&paradigm::animation));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("mesh",(voidfunction)&paradigm::map_mesh));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("map_texture",(voidfunction)&paradigm::map_texture));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("entity",(voidfunction)&paradigm::map_entity_static));

}

ScriptManager::~ScriptManager(){

}

void ScriptManager::readfile(std::string name){

    std::ifstream t(name,std::ios::in);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    _file = buffer;
}

std::string ScriptManager::readtext(){

    std::string str;
    size_t newcursor = _file.find_first_of(" \n", _cursor);

    if(_cursor != _file.npos){

        str = _file.substr(_cursor, newcursor-_cursor);
        _cursor = newcursor + 1;
        if(SCRIPT_DEBUG) LOG(".SCRIPT_DEBUG : reading " + str);
        return str;
    }
    return "";
}

int ScriptManager::readint(){


    int i;
    std::string str = readtext();

    i = atoi(str.c_str());
    return i;
}

float ScriptManager::readfloat(){

    float f;
    std::string str = readtext();

    f = atof(str.c_str());
    return f;
}

void ScriptManager::_jumpline(){

    size_t newcursor = _file.find("\n", _cursor);
    _cursor = newcursor + 1;
}

void ScriptManager::_executescript(std::string name){

    readfile(name);
    _paradigm = "global";
    _reached_end_of_file = false;
    ERR(name);
    ERR(_file);
    _cursor = 0;

    while(_reached_end_of_file == false){

        _executeline();
    }
    LOG("End");
}

void ScriptManager::_executeline(){

    _paradigms[_paradigm]();
}
