#include "map.h"

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

    Script->_executescript("texte.txt");


    ObjectEntity* obj = CreateObjectEntity("object");
    MapTexture* tex = _view_manager->CreateMapTexture();
    anim = new Animation();
    anim->PushFrame(ImgManager->GetImage("block"), 1000);
    tex->AddAnimation("DEFAULT", anim);
    obj->LinkMapTexture(tex);
    Mesh* mesh = _physics_manager->CreateMesh();
    mesh->SetPos(32,120,0);
    mesh->SetSize(16,32,16);
    mesh->SetStatic(true);
    obj->LinkMesh(mesh);
    _object_entities.insert(make_pair("object", obj));


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

    _view_manager->Update();
//    _physics_manager->Update(Time->GetUpdateTime());
//    _physics_manager->ManageCollisions();
//    _event_manager->Update();

    for(auto it = _static_entities.begin(); it != _static_entities.end(); ++it){

        it->second->Update(Time->GetUpdateTime());
        if(it->second->IsVisible()){
            Image* img = it->second->GetTexture()->GetCurrentFrame();
            coor3f pos = it->second->GetMesh()->GetPos();
            coor3f size = it->second->GetMesh()->GetSize();

            _view_manager->RegisterTextureForSorting(img, pos, size);
        }
    }

    for(auto it = _object_entities.begin(); it != _object_entities.end(); ++it){

        it->second->Update(Time->GetUpdateTime());
        if(it->second->IsVisible()){
            Image* img = it->second->GetTexture()->GetCurrentFrame();
            coor3f pos = it->second->GetMesh()->GetPos();
            coor3f size = it->second->GetMesh()->GetSize();

            _view_manager->RegisterTextureForSorting(img, pos, size);
        }
    }

}

void MapMode::Reset(){


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
