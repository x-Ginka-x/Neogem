#include "map_event.h"

#include "map.h"


using namespace std;
using namespace neo;

EventManager* neo::event::current_event_manager = NULL;

MapEventString::MapEventString(){

    _current_event = NULL;
    _event_counter = 0;
    _is_playing = false;
}

MapEventString::~MapEventString(){


}

bool MapEventString::PushEvent(MapEvent* event){

    if(event == NULL)
        return false;

    else{
        _events.push_back(event);
        event::current_event_manager->RegisterEvent(event);
    }

    return true;
}

void MapEventString::Play(){
    _is_playing = true;
}

void MapEventString::Update(){

    if(_events.empty() == true) return;

    _current_event = _events.at(_event_counter);

    if(_is_playing == true){

        if(_current_event == NULL){
            ERR("-map_event.cpp : Trying to play an empty event in MapEventString::Update()");
            return;
        }

        if(_current_event->IsPlaying() == false){
            _current_event->Play();
        }

        if(_current_event->IsDone()){
            _current_event->Stop();
            _event_counter++;

            if(_event_counter >= _events.size()){
                _event_counter = 0;
                _is_playing = false;
            }

        }
    }
    if(_current_event != NULL){
        _current_event->Update();
    }
}

void MapEventString::SetCondition(int type, std::string var, int value){

    _events.back()->AddCondition(type, var, value);
}

MapEvent::MapEvent(){

    _event_manager = event::current_event_manager;
    _is_playing = false;
    _is_done = false;
}

MapEvent::~MapEvent(){

}

Entity* MapEvent::GetEntity(string name){


//    else if(_event_manager->GetCurrentMap()->_actors.find(name) != neo_map::_current_map->_actors.end())
//
//        return neo_map::_current_map->_actors.at(name);

    return neo_map::_current_map->GetObjectEntity(name);

}



void MapEvent::AddCondition(int type, string variable, int value){

    event::Condition condition = event::Condition();
    condition._type = type;
    condition._variable = variable;
    condition._value = value;
    _conditions.push_back(condition);
}

void MapEvent::Update(){

    if(_is_playing){

        _is_done = false;

        if(_AssertConditions() == false){
            _is_done = true;
            return;
        }
        if(!_is_done)
            _Update();
        else return;
    }
}


bool MapEvent::_AssertCondition(event::Condition condition){

    int type = condition._type;
    string var = condition._variable;
    int value = condition._value;

    if(type == NO_CONDITION)
        return true;
    else if(type == VARIABLE_EQUALS)
        return (_event_manager->GetVar(var) == value);
    else if(type == VARIABLE_IS_LESSER_THAN)
        return (_event_manager->GetVar(var) <= value);
    else if(type == VARIABLE_IS_GREATER_THAN)
        return (_event_manager->GetVar(var) >= value);
    else if(type == SWITCH_IS_ON)
        return (_event_manager->GetSwitch(var) == (value == 1 ? true : false));
    else{
        ERR("-map_event.cpp : Called AssertCondition() with unknown type !");
        ERR("  >_variable = " + var);
        ERR("  >_value = " + to_string(value));
        return false;
    }
}

bool MapEvent::_AssertConditions(){

    bool assertion = false;

    for(auto it = _conditions.begin(); it != _conditions.end(); it++){
        assertion = _AssertCondition(*it);
        if(assertion == false)
            return false;
    }
    return true;
}

EventManager::EventManager(){

    _current_map = neo_map::_current_map;
    neo::event::current_event_manager = this;
}

EventManager::~EventManager(){

    Clear(_events);
    Clear(_event_strings);

}


bool EventManager::AddSwitch(string name){

    if(_switches.find(name) != _switches.end())
        return false;
    else{
        _switches.insert(make_pair(name, false));
        return true;
    }
}


bool EventManager::DeleteSwitch(string name){

    if(_switches.find(name) == _switches.end())
        return false;
    else{
        _switches.erase(name);
        return true;
    }
}



void EventManager::TurnOn(string name){

    if(_switches.find(name) != _switches.end() ){
        _switches.at(name) = true;
    }
}



void EventManager::TurnOff(string name){

    if(_switches.find(name) != _switches.end() ){
        _switches.at(name) = false;
    }
}




void EventManager::AddVar(string name, int value){

    if(_variables.find(name) != _variables.end())
        return;
    else
        _variables.insert(make_pair(name, value));
}

int EventManager::GetVar(string name){

    if(_variables.find(name) != _variables.end())
        return _variables.at(name);
    else{
        ERR("-map_event.cpp : Attempted to call GetVar() on non-existing variable named " + name);
        return 0;
    }
}

void EventManager::SetVar(string name, int value){

    if(_variables.find(name) != _variables.end())
        _variables.at(name) = value;
    else{
        ERR("-map_events.cpp : Attempted to call SetVar() on non-existing variable named " + name);
        ERR("  > variable created");
        AddVar(name, value);
    }
}

void EventManager::RegisterEvent(MapEvent* ev){

    _events.push_back(ev);
}

MapEventString* EventManager::CreateEventString(){

    MapEventString* ev = new MapEventString();
    _event_strings.push_back(ev);
    return ev;
}

void EventManager::Update(){

    for(auto it = _event_strings.begin(); it != _event_strings.end(); ++it){
        (*it)->Update();
    }

}

using namespace event;

Teleport::Teleport(){

}

Teleport::Teleport(coor3f pos, DIRECTION dir, std::string map_name){

    _pos = pos;
    _dir = dir;
    _map_name = map_name;
    _is_playing = false;
}

Teleport::~Teleport(){

}

void Teleport::_Update(){

    GetEntity("Ginka")->GetMesh()->SetPos(_pos.x, _pos.y, _pos.z);
    _is_done = true;
}

ModifyVariable::ModifyVariable(string var, VARIABLE_MODIFIER modifier, int value){

    _variable = var;
    _modifier = modifier;
    _value = value;
}

ModifyVariable::~ModifyVariable(){

}

void ModifyVariable::_Update(){

    int value = _event_manager->GetVar(_variable);

    if(_modifier == EQUALS)
        value = _value;
    else if(_modifier == MINUS)
        value -= _value;
    else if(_modifier == PLUS)
        value += _value;
    else if(_modifier == DIV)
        value /= _value;
    else if(_modifier == MULT)
        value *= _value;
    else if(_modifier == MOD)
        value %= _value;
    else{
        ERR("-map_event.cpp : Unknown VARIABLE_MODIFIER in event::ModifyVariable::_Update()");
    }


    _event_manager->SetVar(_variable, value);
    _is_done = true;
}

ActivateSwitch::ActivateSwitch(string name, string mode){

    _name = name;
    _mode = mode;
}

ActivateSwitch::~ActivateSwitch(){

}

/*** TODO : If toggle mode not needed, replace string by bool ***/
void ActivateSwitch::_Update(){

    if(_mode == "on")
        _event_manager->TurnOn(_name);
    else if(_mode == "off")
        _event_manager->TurnOff(_name);
    else if(_mode == "toggle"){
        if(_event_manager->GetSwitch(_name) == true)
            _event_manager->TurnOff(_name);
        else
            _event_manager->TurnOn(_name);
    }
    else{
        _is_playing = false;
        ERR("-map_event.cpp : Wrong parameter for event::ActivateSwitch");
        ERR("  >parameter is set to " + _name);
        return;
    }
    _is_done = true;
}


/**---------------------------------------**/

void HideRevealEntity::_Update(){

    Entity* _entity = GetEntity(_name);
    switch(_mode){
    case EVENT_HIDE:
        _entity->SetVisible(false);
        break;
    case EVENT_REVEAL:
        _entity->SetVisible(true);
        break;
    case EVENT_TOGGLE_HIDE:{
        if(_entity->IsVisible())
            _entity->SetVisible(false);
        else
            _entity->SetVisible(true);
        break;
    }
    case EVENT_PHANTOM:{
        _entity->SetVisible(false);
        _entity->GetMesh()->SetSolid(false);
        break;
    }
    case EVENT_UNPHANTOM:{
        _entity->SetVisible(true);
        _entity->GetMesh()->SetSolid(true);
        break;
    }
    case EVENT_TOGGLE_PHANTOM:{
        if(_entity->IsVisible())
            _entity->SetVisible(false);
        else
            _entity->SetVisible(true);
        if(_entity->GetMesh()->IsSolid())
            _entity->GetMesh()->SetSolid(false);
        else
            _entity->GetMesh()->SetSolid(true);
        break;
    }

    }
    _is_done = true;
}


void PlayAnimation::_Update(){

    Entity* _entity = GetEntity(_name);

    if(_entity == NULL){
        ERR("-map_event.cpp : Unvalid pointer to Entity in PlayAnimation::_Update()");
        ERR("   Entity called was named : " + _name + " with state : " + _state);
        return;
    }
    _entity->GetTexture()->Play(_state);
    _is_done = true;
}
