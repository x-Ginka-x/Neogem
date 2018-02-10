
#include "input.h"

using namespace std;
using namespace neo;


namespace neo{

InputEngine* Input = NULL;
bool INPUT_DEBUG = true;

/*** Constructor ***/

InputEngine::InputEngine() {

	if (INPUT_DEBUG) LOG(".create input");

	if(SDL_Init(SDL_INIT_EVENTS) < 0){
        if(INPUT_DEBUG) ERR(".INPUT_DEBUG : Failed to initialize SDL_EVENTS");
        return;
    }

	_any_key_pressed = false;
	_any_key_released = false;

}//InputEngine()

/*** Destructor ****/

InputEngine::~InputEngine() {
	if (INPUT_DEBUG) LOG(".destroy input");
}//~InputEngine()


/*** Accessors ***/

bool InputEngine::AnyKeyPressed(){return _any_key_pressed;}
bool InputEngine::AnyKeyReleased(){return _any_key_released;}

/*** This function checks for a new event and calls KeyEventHandler providing it the SDL_Event ***/

void InputEngine::Update(){

	_any_key_pressed   = false;
	_any_key_released = false;

	for(auto it = _inputpress.begin(); it != _inputpress.end(); ++it){
	    it->second = false;
	}
	for(auto it = _inputrelease.begin(); it != _inputrelease.end(); ++it){
	    it->second = false;
	}


    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        _event=event;
        switch (event.type)
        {
        case SDL_QUIT:
            _any_key_pressed = true;
            break;

        case SDL_KEYDOWN:
            {
                if(event.key.repeat != 0)
                break;
                else
                _KeyEventHandler(event.key);
            }
        case SDL_KEYUP:
            {
                _KeyEventHandler(event.key);
            }
        } // end switch
    }//PollEvent

}//EventHandler()

/*** This function handles the SDL_Event given by the precedent function ***/

void InputEngine::_KeyEventHandler(SDL_KeyboardEvent& key_event){

    if (key_event.type == SDL_KEYDOWN){

        _any_key_pressed = true;

        if(_inputpress.find(key_event.keysym.sym) == _inputpress.end())
            _inputpress.insert(make_pair(key_event.keysym.sym, true));
        else
            _inputpress[key_event.keysym.sym] = true;

        if(_inputstate.find(key_event.keysym.sym) == _inputstate.end())
            _inputstate.insert(make_pair(key_event.keysym.sym, false));
        else
            _inputstate[key_event.keysym.sym] = true;

    }///if KEYDOWN

    else if (key_event.type == SDL_KEYUP){

        _any_key_released= true;

        if(_inputrelease.find(key_event.keysym.sym) == _inputrelease.end())
            _inputrelease.insert(make_pair(key_event.keysym.sym, true));
        else
            _inputrelease[key_event.keysym.sym] = true;

        if(_inputstate.find(key_event.keysym.sym) == _inputstate.end())
            _inputstate.insert(make_pair(key_event.keysym.sym, false));
        else
            _inputstate[key_event.keysym.sym] = false;
    }///if KEYUP

}//KeyEventHandler


bool InputEngine::Press(SDL_Keycode key){

    if(_inputpress.find(key) == _inputpress.end()){
        return false;
    }
    else
        return _inputpress[key];
}

bool InputEngine::Release(SDL_Keycode key){

    if(_inputrelease.find(key) == _inputrelease.end()){
        return false;
    }
    else
        return _inputrelease[key];
}

bool InputEngine::State(SDL_Keycode key){

    if(_inputstate.find(key) != _inputstate.end()){
        return _inputstate[key];
    }
    else
        return false;
}

}//namespace neo
