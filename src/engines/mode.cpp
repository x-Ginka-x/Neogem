#include "mode.h"
#include "time.h"

using namespace std;
using namespace neo;


bool neo::MODE_DEBUG = true;
ModeManager* neo::Mode = NULL;

///*** GameMode functions ***///


GameMode::GameMode(){

    _mode_type = GAMEMODE_INVALID;
}//constructor


GameMode::~GameMode(){

}//destructor

GAMEMODE_TYPE GameMode::GetModeType(){

    return _mode_type;
}

///*** ModeManager functions ***///

ModeManager::ModeManager(){
    if(MODE_DEBUG) LOG(".create mode");
    _state = MODEMANAGER_NOCHANGE;
    _next = NULL;
}

ModeManager::~ModeManager(){
    if(MODE_DEBUG) LOG(".destroy mode");

    while (_mode_stack.size() != 0) {
		delete _mode_stack.back();
		_mode_stack.pop_back();
	}
}


void ModeManager::Draw(){
    if(_mode_stack.empty()==false)
        _mode_stack.back()->Draw();
}


void ModeManager::Update(){

    if(_state == MODEMANAGER_NOCHANGE){
    }
    else{

    if(_state == MODEMANAGER_NEXT) {
        _mode_stack.push_back(_next);}

    else if(_state == MODEMANAGER_PREVIOUS) {
        if(_mode_stack.size()<=0) if(MODE_DEBUG) ERR(".MODE_DEBUG : Tried to pop Mode stack while no mode was inside" );
        delete _mode_stack.back();
        _mode_stack.pop_back();
    }

    else if(_state == MODEMANAGER_RESET) {
        int i = (_mode_stack.size())-1;
        while(i>0){
            delete _mode_stack.back();
            _mode_stack.pop_back();
            i--;
        }
    }

    if(_mode_stack.empty()){
        if(MODE_DEBUG) ERR(".MODE_DEBUG : Mode stack empty, Exiting game ..." );
        Time->ExitGame();
        return;
    }

    _mode_stack.back()->Reset();

    _state = MODEMANAGER_NOCHANGE;
    }//if(_state!=NOCHANGE)

    _mode_stack.back()->Update();
}//Update



GAMEMODE_TYPE ModeManager::GetTopModeType(){
    if(_mode_stack.empty())
        return GAMEMODE_INVALID;
    else
        return _mode_stack.back()->GetModeType();
}//gettype


GameMode* ModeManager::GetTopMode(){
    if(_mode_stack.empty())
        return NULL;
    else
        return _mode_stack.back();
}//gettopmode


