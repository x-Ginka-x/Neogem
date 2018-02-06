#ifndef MODE_H_INCLUDED
#define MODE_H_INCLUDED

#include <vector>

#include "../defs.h"
#include "../utils.h"

namespace neo{

enum GAMEMODE_TYPE{

    GAMEMODE_INVALID,
    GAMEMODE_TEST,
    GAMEMODE_MAIN
};
enum MODEMANAGER_STATE{

    MODEMANAGER_RESET,
    MODEMANAGER_PREVIOUS,
    MODEMANAGER_NEXT,
    MODEMANAGER_NOCHANGE
};

extern bool MODE_DEBUG;

class GameMode{

protected:
    GAMEMODE_TYPE _mode_type;

public:
    GameMode();
    virtual ~GameMode();

    virtual void Update(){}
    virtual void Draw(){}
    virtual void Reset(){}

    GAMEMODE_TYPE GetModeType();

};//class GameMode


class ModeManager{

public:

    ModeManager();
    ~ModeManager();

    void Update();
    void Draw();
    void Push(GameMode* gm){ _next = gm; _state = MODEMANAGER_NEXT;}
    void Pop() {_state = MODEMANAGER_PREVIOUS;}
    void PopAll(){_state = MODEMANAGER_RESET;}

    GAMEMODE_TYPE GetTopModeType();
    GameMode* GetTopMode();

private:


    std::vector<GameMode*> _mode_stack;
    GameMode* _next;
    MODEMANAGER_STATE _state;

};//class ModeManager

extern ModeManager* Mode;


}//namespace neo

#endif // MODE_H_INCLUDED
