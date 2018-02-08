#include "script.h"

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
    }
    else if(instruction == "size"){
        Mesh* mesh = s_mesh(s_active);
        float x = s_float;
        float y = s_float;
        float z = s_float;
        mesh->SetSize(x, y, z);
    }
    else if(instruction == "mass"){
        Mesh* mesh = s_mesh(s_active);
        int i = s_int;
        mesh->SetMass(i);
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
        StaticEntity* obj = neo_map::_current_map->CreateStaticEntity(name);
        s_register(name, obj);
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
    }
    else{

    }
}


void paradigm::map_entity_object(){

    std::string instruction = s_text;

    if(instruction == "new"){
        std::string name = s_text;
        ObjectEntity* obj = neo_map::_current_map->CreateObjectEntity(name);
        s_register(name, obj);
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "texture"){
        MapTexture* tex = s_texture(s_get(s_text));
        ObjectEntity* obj = s_objectentity(s_active);
        obj->LinkMapTexture(tex);
    }

    else if(instruction == "mesh"){
        Mesh* mesh = (Mesh*)s_get(s_text);
        ObjectEntity* obj = s_objectentity(s_active);
        obj->LinkMesh(mesh);
        mesh->SetStatic(false);
    }
    else if(instruction == "eventstring"){
        MapEventString* ev = s_eventstring(s_get(s_text));
        ObjectEntity* obj = s_objectentity(s_active);
        obj->AddPassiveEvent(ev);
    }
    else{

    }
}

void paradigm::map_entity_actor(){

    std::string instruction = s_text;

    if(instruction == "new"){
        std::string name = s_text;
        ActorEntity* act = neo_map::_current_map->CreateActorEntity(name);
        s_register(name, act);
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "texture"){
        MapTexture* tex = s_texture(s_get(s_text));
        ActorEntity* act = s_actorentity(s_active);
        act->LinkMapTexture(tex);
    }

    else if(instruction == "mesh"){
        Mesh* mesh = (Mesh*)s_get(s_text);
        ActorEntity* act = s_actorentity(s_active);
        act->LinkMesh(mesh);
        mesh->SetStatic(false);
    }
    else{

    }
}

void paradigm::map_event(){

    std::string instruction = s_text;

    if(instruction == "setvar"){
        std::string name = s_text;
        int value = s_int;
        event::current_event_manager->SetVar(name, value);
    }

    if(instruction == "new"){
        std::string name = s_text;
        MapEventString* ev_str = event::current_event_manager->CreateEventString();
        s_register(name, ev_str);
    }
    else if(instruction == "push"){
        std::string mapevent_type = s_text;
        if(mapevent_type == "hidereveal"){
            std::string name = s_text;
            int mode = s_int;
            MapEventString* ev_str = s_eventstring(s_active);
            ev_str->PushEvent(new event::HideRevealEntity(name, mode));
        }
        else if(mapevent_type == "teleport"){
            float x = s_float;
            float y = s_float;
            float z = s_float;
            DIRECTION dir = s_enum_DIRECTION;
            std::string map_name = s_text;
            MapEventString* ev_str = s_eventstring(s_active);
            ev_str->PushEvent(new event::Teleport(coor3f(x,y,z), dir, map_name));
        }
        else if(instruction == "playanimation"){

        }
        else if(instruction == "activateswitch"){

        }
        else if(instruction == "modifyvariable"){

        }
    }
    else if(instruction == "condition"){
        int type = s_int;
        std::string str = s_text;
        int value = s_int;
        MapEventString* ev_str = s_eventstring(s_active);
        ev_str->SetCondition(type, str, value);
    }
    else if(instruction == "end"){
        s_par("global");
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
    _paradigms.insert(std::make_pair<std::string,voidfunction>("staticentity",(voidfunction)&paradigm::map_entity_static));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("objectentity",(voidfunction)&paradigm::map_entity_object));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("event",(voidfunction)&paradigm::map_event));
    _paradigms.insert(std::make_pair<std::string,voidfunction>("actorentity",(voidfunction)&paradigm::map_entity_actor));

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

DIRECTION ScriptManager::readdirection(){

    std::string direction;
    DIRECTION dir = SOUTH;

    if(direction == "NORTH") dir = NORTH;
    else if(direction == "NORTH_EAST") dir = NORTH_EAST;
    else if(direction == "EAST") dir = EAST;
    else if(direction == "SOUTH_EAST") dir = SOUTH_EAST;
    else if(direction == "SOUTH") dir = SOUTH;
    else if(direction == "SOUTH_WEST") dir = SOUTH_WEST;
    else if(direction == "WEST") dir = WEST;
    else if(direction == "NORTH_WEST") dir = NORTH_WEST;
    return dir;
}

//bool ScriptManager::readspecialchar(const char* value){
//
//    const char* str = (_file.substr(_cursor, 2)).c_str();
//    std::cout << str << std::endl;
//    if(str == value)
//        return true;
//    else
//        return false;
//}

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

    if(_paradigms.find(_paradigm) != _paradigms.end())
        _paradigms[_paradigm]();
}
