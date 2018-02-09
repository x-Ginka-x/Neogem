#include "map.h"
#include "../test/test.h"

using namespace std;
using namespace neo;

bool neo::MAP_DEBUG = true;
MapMode* MapMode::_current_map = NULL;

MapMode::MapMode(){
    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");

    _current_map = this;
    _script_file = "";

    _view_manager = new MapVideoEngine();
    _physics_manager = new PhysicsEngine();
    _event_manager = new EventManager();

    ScriptObject* script = new ScriptObject();

    script->BindParadigm("image", (paradigmfunction)&neo::ImageDescriptor);
    script->BindParadigm("animation", (paradigmfunction)&neo::AnimationDescriptor);
    script->BindParadigm("mesh", (paradigmfunction)&neo::MeshDescriptor);
    script->BindParadigm("event", (paradigmfunction)&neo::MapEventDescriptor);
    script->BindParadigm("staticentity", (paradigmfunction)&neo::StaticEntityDescriptor);
    script->BindParadigm("objectentity", (paradigmfunction)&neo::ObjectEntityDescriptor);
    script->BindParadigm("actorentity", (paradigmfunction)&neo::ActorEntityDescriptor);
    script->BindParadigm("map_texture", (paradigmfunction)&neo::MapTextureDescriptor);

    script->_executescript("src/modes/map/testmap.nsl");

    delete script;

    _physics_manager->SetUpdateZone(-1000,-500,2000,1000);

    _blank_bg = ImgManager->GetImage("crate2");

    _view_manager->Follow(coor3f(0,0,0));
    _view_manager->Update();
    _state = NO_CHANGE;
}

MapMode::MapMode(string script_file){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");
    _current_map = this;
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

    for(int i = 0; i < 4; i++){
        Video->SetCursorPos((i%2)*640, (i/2)*360, 0.5);
        _blank_bg->Draw(IMAGE_DRAW_FROM_TOPLEFT);
    }
    _view_manager->Draw();
    Video->SetCursorPos(150, 10.5, 999);
    Text->Write("Place the Block on the Mark", "default_font");

}

void MapMode::Update(){

    /*** Controls (to be moved away) ***/

    if(Input->Press(SDLK_ESCAPE))
        Mode->Pop();

    if(Input->State(SDLK_d)){

        GetActorEntity("ginka")->Walk(EAST);
    }
    else if(Input->State(SDLK_q)){

        GetActorEntity("ginka")->Walk(WEST);
    }
    else if(Input->State(SDLK_z)){

        GetActorEntity("ginka")->Walk(NORTH);
    }
    else if(Input->State(SDLK_s)){

        GetActorEntity("ginka")->Walk(SOUTH);
    }
    if(Input->Press(SDLK_i)){
        Mode->Push(new TestMode());
    }

    /** Update the Physics and manage the collisions **/

    _physics_manager->Update(Time->GetUpdateTime());
    _physics_manager->ManageCollisions();

    /** This recalculate collisions for meshes that are between two others meshes, preventing some bugs **/

    _physics_manager->ManageCollisions();

    /** Updates the MapEvent strings **/

    _event_manager->Update();

    /** Updates static, object and actor entities **/

    _UpdateEntities(_static_entities);
    _UpdateEntities(_object_entities);
    _UpdateEntities(_actor_entities);


    /** Still not good but sets the camera to follow the hero **/

    _view_manager->Follow(GetActorEntity("ginka")->GetPos());

    /** Updates the MapTextures **/

    _view_manager->Update();
}

void MapMode::Reset(){

    _current_map = this;
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

            it->second->PlayPassive();
        }
    }
}
