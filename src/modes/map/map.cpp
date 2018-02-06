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

    Script->_executescript("src/modes/map/texte.txt");
    obj = CreateObjectEntity("object");
    Animation anim = Animation();
    MapTexture* tex = _view_manager->CreateMapTexture();
    anim.PushFrame(ImgManager->GetImage("crate<f<0,0,37,37"), 100);
    tex->AddAnimation("DEFAULT", anim);
    obj->LinkMapTexture(tex);
    Mesh* mesh = _physics_manager->CreateMesh();
    mesh->SetPos(32.0,300.0,0.0);
    mesh->SetSize(16.0,32.0,16.0);
    mesh->SetMass(10);

    obj->LinkMesh(mesh);

    _event_manager->AddVar("switch", 0);

    event::HideEntity* ev = new event::HideEntity("object");
    _event_manager->RegisterEvent("ev", ev);

    ev->AddCondition(event::VARIABLE_EQUALS, "switch", 1);

    MapEventString* ev_str = new MapEventString();
    ev_str->PushEvent(ev);
    _event_manager->RegisterEventString("ev_str", ev_str);

    obj->AddPassiveEvent(ev_str);
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
    delete _view_manager;
    delete _physics_manager;
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
    if(Input->DownPress()){
        _event_manager->SetVar("switch", 1);
    }
    if(Input->RightPress()){
        obj->GetMesh()->SetPos(32,300,0);
    }

    _view_manager->Update();
    _physics_manager->Update(Time->GetUpdateTime());
    _physics_manager->ManageCollisions();
    _event_manager->Update();
    obj->PlayPassive();

    for(auto it = _static_entities.begin(); it != _static_entities.end(); ++it){

        it->second->Update(Time->GetUpdateTime());
        if(it->second->IsVisible()){
            Image* img = it->second->GetTexture()->GetCurrentAnimationFrameTexture();
            coor3f pos = it->second->GetMesh()->GetPos();
            coor3f size = it->second->GetMesh()->GetSize();

            _view_manager->RegisterTextureForSorting(img, pos, size);
        }
    }
    if(obj->IsVisible()){
        _view_manager->RegisterTextureForSorting(obj->GetTexture()->GetCurrentAnimationFrameTexture(), obj->GetPos(), obj->GetSize());
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

    if(_objects.find(name) == _objects.end()){

        ObjectEntity* obj = new ObjectEntity(name);
        _objects.insert(make_pair(name, obj));
    }

    return _objects.at(name);
}


ObjectEntity* MapMode::GetObjectEntity(string name){

    if(_objects.find(name) != _objects.end())
        return _objects.at(name);
    else
        return NULL;
}
