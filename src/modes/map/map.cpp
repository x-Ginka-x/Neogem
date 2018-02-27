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
    _event_manager->AddSwitch("switch1");
    aabb = CreateObjectEntity("aabb");
    Mesh* mesh = _physics_manager->CreateMesh();
    mesh->SetSize(16,16,16);
    mesh->SetSolid(false);
    mesh->SetStatic(false);
    aabb->LinkMesh(mesh);

    _dialog_manager = new DialogManager();
    dialogtest = _dialog_manager->AddDialog("Hey, this is a dialog box ! That's awesome !", 2, "Hell yeah !", "Meh...");
    int dialogtest2 = _dialog_manager->AddDialog("And it's a second DialogBox ! I can't get enough of 'em !!!");


    MapEventString* dialog_ev = _event_manager->CreateEventString();
    _event_manager->AddVar("var_choicetest", 0);
    dialog_ev->PushEvent(new event::Choice(dialogtest, "object_block", "var_choicetest"));
    dialog_ev->PushEvent(new event::Dialog(dialogtest2, "object_block"));
    dialog_ev->SetCondition(VARIABLE_EQUALS, "var_choicetest", 0);

    GetObjectEntity("object_block")->AddActiveEvent(dialog_ev);

    _state = STATE_EXPLO;
}

MapMode::MapMode(string script_file){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map mode");
    _current_map = this;
    _script_file = script_file;
    _view_manager = new MapVideoEngine();
    _state = STATE_EXPLO;
}

MapMode::~MapMode(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG: destroy map mode");
    Clear(_static_entities);
    Clear(_object_entities);
    Clear(_actor_entities);
    delete _view_manager;
    delete _physics_manager;
    delete _event_manager;
    delete _dialog_manager;
}

void MapMode::Draw(){


    _blank_bg->SetDisplaySize(640, 360);
    _blank_bg->Draw(0,0,1,IMAGE_DRAW_FROM_TOPLEFT);
    _view_manager->Draw();
    _dialog_manager->Draw();

//    Time->ExitGame();
}

void MapMode::Update(){
//    ERR("lopp");

    coor3f pos = GetActorEntity("ginka")->GetPos();
    GetObjectEntity("aabb")->GetMesh()->SetPos(pos.x-4, pos.y, pos.z-4);
    /*** Controls (to be moved away) ***/

    if(Input->Press(SDLK_ESCAPE))
        Mode->Pop();

    if(_dialog_manager->IsPlaying())
        _state = STATE_DIALOG;
    else
        _state = STATE_EXPLO;

    if(_state == STATE_EXPLO)
        _CheckControls();

    else if(_state == STATE_DIALOG)
        _dialog_manager->CheckControls();


    _dialog_manager->Update();

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


StaticEntity* MapMode::GetStaticEntity(string name){

    if(_static_entities.find(name) != _static_entities.end())
        return _static_entities.at(name);
    else
        return NULL;
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

            if(GetObjectEntity("aabb")->IsColliding(it->second) && Input->Press(SDLK_e) && _state == STATE_EXPLO){
                it->second->PlayActive();
            }
            it->second->PlayPassive();
        }
    }
}


void MapMode::_CheckControls(){

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
}
