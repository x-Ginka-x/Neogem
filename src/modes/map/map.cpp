#include "map.h"
#include "../test/test.h"

using namespace std;
using namespace neo;

bool neo::MAP_DEBUG = true;
MapMode* neo::neo_map::_current_map = NULL;

MapMode::MapMode(){
    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");

    neo_map::_current_map = this;
    _script_file = "";
    _view_manager = new MapVideoEngine();
    _physics_manager = new PhysicsEngine();
    _event_manager = new EventManager();

    Script->_executescript("src/modes/map/testmap.nsl");


//    MapEventString* ev_str = _event_manager->CreateEventString();
//    ev_str->PushEvent(new event::HideRevealEntity("object2", EVENT_TOGGLE_PHANTOM));
//    GetObjectEntity("object2")->AddPassiveEvent(ev_str);

    _state = NO_CHANGE;

}

MapMode::MapMode(string script_file){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");
    neo_map::_current_map = this;
    _script_file = script_file;
    _view_manager = new MapVideoEngine();
    _view_manager->Follow(coor3f(0,0,0));
    _state = NO_CHANGE;
}

MapMode::~MapMode(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG: destroy map mode");
    Clear(_static_entities);
    Clear(_object_entities);
    delete _view_manager;
    delete _physics_manager;
    delete _event_manager;
}

void MapMode::Draw(){

    _view_manager->Follow(coor3f(0,0,0));
    _view_manager->Draw();

}

void MapMode::Update(){

    if(Input->EscapePress())
        Mode->Pop();

    if(_state == CHANGE)
        Reset();

    if(Input->ActionPress()){
        if(GetObjectEntity("object2") != NULL)
            GetObjectEntity("object2")->PlayPassive();
    }
    if(Input->DownPress()){
        Mode->Push(new TestMode());
    }
    if(Input->UpPress()){
        _event_manager->SetVar("var", 1);
    }

    _view_manager->Update();
    _physics_manager->Update(Time->GetUpdateTime());
    _physics_manager->ManageCollisions();
    _event_manager->Update();


    _UpdateEntities(_static_entities);
    _UpdateEntities(_object_entities);

}

void MapMode::Reset(){

    neo_map::_current_map = this;
}


void MapMode::ChangeMap(string path){

    _script_file = path;
    _state = CHANGE;
}


StaticEntity* MapMode::CreateStaticEntity(std::string name){

    if(_static_entities.find(name) == _static_entities.end()){

        StaticEntity* obj = new StaticEntity();
        obj->SetName(name);
        _static_entities.insert(make_pair(name, obj));
    }

    return _static_entities.at(name);
}

ObjectEntity* MapMode::CreateObjectEntity(std::string name){

    if(_object_entities.find(name) == _object_entities.end()){

        ObjectEntity* obj = new ObjectEntity(name);
        _object_entities.insert(make_pair(name, obj));
    }

    return _object_entities.at(name);
}


ObjectEntity* MapMode::GetObjectEntity(string name){

    if(_object_entities.find(name) != _object_entities.end())
        return _object_entities.at(name);
    else
        return NULL;
}

template<class A> void MapMode::_UpdateEntities(A& entities){

    for(auto it = entities.begin(); it != entities.end(); ++it){

        it->second->Update(Time->GetUpdateTime());
        if(it->second->IsVisible()){
            Image* img = it->second->GetTexture()->GetCurrentFrame();
            coor3f pos = it->second->GetMesh()->GetPos();
            coor3f size = it->second->GetMesh()->GetSize();

            _view_manager->RegisterTextureForSorting(img, pos, size);
        }
    }
}
