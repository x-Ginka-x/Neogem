#ifndef MAP_EVENT_H_INCLUDED
#define MAP_EVENT_H_INCLUDED

#include <string>
#include <map>
#include<vector>

#include "../../utils.h"
#include "../../defs.h"


namespace neo{

class EventManager;
class MapEvent;

namespace event{

extern EventManager* current_event_manager;

enum CONDITION_TYPE{

    NO_CONDITION = 0,
    VARIABLE_EQUALS,
    VARIABLE_IS_LESSER_THAN,
    VARIABLE_IS_GREATER_THAN,
    SWITCH_IS_ON
};

class Condition{

public:
    Condition(){_type = NO_CONDITION; _variable = ""; _value = 0;}
    ~Condition(){}

    CONDITION_TYPE _type;
    std::string _variable;
    int _value;

};//event::Condition

enum VARIABLE_MODIFIER{

    EQUALS = 0,
    MINUS,
    PLUS,
    DIV,
    MULT,
    MOD
};



}//namespace event

class MapEventString{

public:
    MapEventString();
    ~MapEventString();

    bool PushEvent(MapEvent* event);
    void PopEvent(){_events.pop_back();}

    void Update();
    void Play();

private:

    unsigned int _event_counter;
    bool _is_playing;

    std::vector<MapEvent*> _events;
    MapEvent* _current_event;

};//MapEventString

class MapEvent{

protected:

    MapEvent();
    std::string _name;


    bool _is_playing;
    bool _is_done;
    std::vector<event::Condition> _conditions;

    EventManager* _event_manager;

    virtual void _Update(){};
    bool _AssertCondition(event::Condition);
    bool _AssertConditions();
    Entity* GetEntity(std::string name);

public:

    virtual ~MapEvent();

    std::string GetName() const {return _name;}
    void AddCondition(event::CONDITION_TYPE, std::string, int);

    void Play() {_is_playing = true; _is_done = false;}
    void Stop() {_is_playing = false;}
    void Reset(){_is_done = false;}
    bool IsPlaying() const {return _is_playing;}
    bool IsDone() const {return _is_done;}

    void Update();

private:

};




class EventManager{

private:

    std::map<std::string, MapEvent*> _events;
    std::map<std::string, MapEventString*> _event_strings;
    std::map<std::string, bool> _switches;
    std::map<std::string, int> _variables;

    MapMode* _current_map;

public:

    EventManager();
    ~EventManager();

    void RegisterEvent(std::string name, MapEvent* event){_events.insert(std::make_pair(name, event));}
    void DeleteEvent(std::string);

    MapEvent* GetEvent(std::string);


///TEST WITH EVENT STRINGS///

    void RegisterEventString(std::string name, MapEventString* event_string){_event_strings.insert(std::make_pair(name, event_string));}
    void DeleteEventString(std::string);

    MapEventString* GetEventString(std::string);

///--------------///


    bool AddSwitch(std::string);
    bool DeleteSwitch(std::string);

    bool GetSwitch(std::string name) const {if (_switches.find(name) != _switches.end()) return _switches.at(name); else return false;}
    void TurnOn(std::string) ;
    void TurnOff(std::string);

    int GetVar(std::string);
    void SetVar(std::string, int);
    void AddVar(std::string, int);

    MapMode* GetCurrentMap() {return _current_map;}

    void Update();

};//EventManager

namespace event{



class Teleport : public MapEvent {

private:

    coor3f _pos;
    DIRECTION _dir;
    std::string _map_name;

    Teleport();

    void _Update();

public:

    Teleport(coor3f, DIRECTION, std::string map_name = "");
    ~Teleport();


};//TeleportEvent


class ModifyVariable : public MapEvent{

private:

    std::string _variable;
    VARIABLE_MODIFIER _modifier;
    int _value;

    void _Update();

public:

    ModifyVariable(std::string, VARIABLE_MODIFIER, int);
    ~ModifyVariable();


};//ModifyVariableEvent


class ActivateSwitch : public MapEvent{

private:

    std::string _name;
    std::string _mode;

    void _Update();

public:

    ActivateSwitch(std::string, std::string);
    ~ActivateSwitch();


};//ActivateSwitch

class HideEntity : public MapEvent{

private:

    std::string _name;

    void _Update();

public:

    HideEntity(std::string name){_name = name;}
    ~HideEntity(){}

};//HideEntity


class RevealEntity : public MapEvent{

private:

    std::string _name;

    void _Update();

public:

    RevealEntity(std::string name){_name = name;}
    ~RevealEntity(){}

};//HideEntity

class PlayAnimation : public MapEvent{

private:

    std::string _name;
    std::string _state;

    void _Update();

public:

    PlayAnimation(std::string name, std::string state){_name = name; _state= state;}
    ~PlayAnimation(){}

};//PlayAnimation


}//namespace event
}//namespace

#endif // MAP_EVENT_H_INCLUDED
