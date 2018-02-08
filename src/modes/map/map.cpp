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
    _physics_manager->SetUpdateZone(-1000,-500,2000,1000);

    _view_manager->Follow(coor3f(0,0,0));
    _view_manager->Update();
    _state = NO_CHANGE;
}

MapMode::MapMode(string script_file){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");
    neo_map::_current_map = this;
    _script_file = script_file;
    _view_manager = new MapVideoEngine();
    _state = NO_CHANGE;
}

MapMode::~MapMode(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG: destroy map mode");
    Clear(_static_entities);
    Clear(_object_entities);
    Clear(_actor_entities);
    delete _view_manager;
    delete _physics_manager;
    delete _event_manager;
}

void MapMode::Draw(){

    _view_manager->Draw();

}

void MapMode::Update(){

    if(Input->EscapePress())
        Mode->Pop();

    if(_state == CHANGE)
        Reset();

    if(GetObjectEntity("object_rock") != NULL)
        GetObjectEntity("object_rock")->PlayPassive();


    if(Input->RightState()){

        GetActorEntity("ginka")->Walk(EAST);
    }
    else if(Input->LeftState()){

        GetActorEntity("ginka")->Walk(WEST);
    }
    else if(Input->UpState()){

        GetActorEntity("ginka")->Walk(NORTH);
    }
    else if(Input->DownState()){

        GetActorEntity("ginka")->Walk(SOUTH);
    }

    _physics_manager->Update(Time->GetUpdateTime());
    _physics_manager->ManageCollisions();


    _event_manager->Update();
    _UpdateEntities(_static_entities);
    _UpdateEntities(_object_entities);
    _UpdateEntities(_actor_entities);

    _view_manager->Follow(GetActorEntity("ginka")->GetCenter());

    _view_manager->Update();

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

ActorEntity* MapMode::CreateActorEntity(std::string name){

    if(_actor_entities.find(name) == _actor_entities.end()){

        ActorEntity* act = new ActorEntity(name);
        _actor_entities.insert(make_pair(name, act));
    }

    return _actor_entities.at(name);
}

ObjectEntity* MapMode::GetObjectEntity(string name){

    if(_object_entities.find(name) != _object_entities.end())
        return _object_entities.at(name);
    else
        return NULL;
}

ActorEntity* MapMode::GetActorEntity(string name){

    if(_actor_entities.find(name) != _actor_entities.end())
        return _actor_entities.at(name);
    else
        return NULL;
}

template<class A> void MapMode::_UpdateEntities(A& entities){

    for(auto it = entities.begin(); it != entities.end(); ++it){

        it->second->Update(Time->GetUpdateTime());
        if(it->second->IsVisible() && it->second->GetTexture() != NULL){
            Image* img = it->second->GetTexture()->GetCurrentFrame();
            coor3f pos = it->second->GetMesh()->GetPos();
            coor3f size = it->second->GetMesh()->GetSize();

            _view_manager->RegisterTextureForSorting(img, pos, size);
        }
    }
}
