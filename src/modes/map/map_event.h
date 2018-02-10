#ifndef MAP_EVENT_H_INCLUDED
#define MAP_EVENT_H_INCLUDED

#include <string>
#include <map>
#include<vector>

#include "../../utils.h"
#include "../../defs.h"


#define NO_CONDITION 0
#define VARIABLE_EQUALS 1
#define VARIABLE_IS_LESSER_THAN 2
#define VARIABLE_IS_GREATER_THAN 3
#define SWITCH_IS_ON 4

namespace neo{

class EventManager;
class MapEvent;

namespace event{

extern EventManager* current_event_manager;



class Condition{

public:
    Condition(){_type = NO_CONDITION; _variable = ""; _value = 0;}
    ~Condition(){}

    int _type;
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
    void SetCondition(int, std::string, int);

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
    void AddCondition(int, std::string, int);

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

    std::vector<MapEvent*> _events;
    std::vector<MapEventString*> _event_strings;
    std::map<std::string, bool> _switches;
    std::map<std::string, int> _variables;

    MapMode* _current_map;

public:

    EventManager();
    ~EventManager();

    void RegisterEvent(MapEvent*);
    MapEventString* CreateEventString();

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

/** All possible MapEvents are defined here **/

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

#define EVENT_HIDE 1
#define EVENT_REVEAL 2
#define EVENT_TOGGLE_HIDE 3
#define EVENT_PHANTOM 4
#define EVENT_UNPHANTOM 5
#define EVENT_TOGGLE_PHANTOM 6

class HideRevealEntity : public MapEvent{

private:

    std::string _name;
    int _mode;

    void _Update();

public:

    HideRevealEntity(std::string name, int mode){_name = name; _mode = mode;}
    ~HideRevealEntity(){}

};//HideRevealEntity

class PlayAnimation : public MapEvent{

private:

    std::string _name;
    std::string _state;

    void _Update();

public:

    PlayAnimation(std::string name, std::string state){_name = name; _state= state;}
    ~PlayAnimation(){}

};//PlayAnimation

class ListenPosition : public MapEvent{

private:

    std::string _target;
    std::string xvar, yvar, zvar;
    void _Update();

public:
    ListenPosition(std::string target, std::string x, std::string y, std::string z){_target = target; xvar = x; yvar = y; zvar = z;}
    ~ListenPosition(){}
};


class SetStatic : public MapEvent{

private:

    std::string _target;
    bool _value;
    void _Update();

public:

    SetStatic(std::string target, bool value){_target = target; _value = value;}
    ~SetStatic(){}
};

//class ReflectMovement : public MapEvent{
//
//};

}//namespace event


void MapEventDescriptor(ScriptObject*);
}//namespace

#endif // MAP_EVENT_H_INCLUDED
