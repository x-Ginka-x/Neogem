
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

	_any_key_pressed		    = false;
	_any_key_released	    = false;

	_up_state             = false;
	_up_press             = false;
	_up_release           = false;
	_down_state           = false;
	_down_press           = false;
	_down_release         = false;
	_left_state           = false;
	_left_press           = false;
	_left_release         = false;
	_right_state          = false;
	_right_press          = false;
	_right_release        = false;
	_escape_press         = false;
	_escape_release       = false;
	_action_state         = false;
	_action_press         = false;
	_action_release       = false;
	_E_press              = false;
	_E_state              = false;
	_E_release            = false;



	_key.up = SDLK_z;
	_key.down = SDLK_s;
	_key.left = SDLK_q;
	_key.right = SDLK_d;
	_key.action = SDLK_SPACE;
	_key.goback = SDLK_f;
	_key.tab = SDLK_TAB;
	_key.escape = SDLK_ESCAPE;
	_key.e = SDLK_e;


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

	_up_press             = false;
	_up_release           = false;
	_down_press           = false;
	_down_release         = false;
	_left_press           = false;
	_left_release         = false;
	_right_press          = false;
	_right_release        = false;
	_escape_press         = false;
	_escape_release       = false;
	_action_press         = false;
	_action_release       = false;
	_E_press              = false;
    _E_release            = false;
    _tab_press            = false;
    _tab_release          = false;

    SDL_Event event;
    if (SDL_PollEvent(&event))
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

    if (key_event.keysym.sym == _key.up) {
			_up_state = true;
			_up_press = true;
			return;
		}
    else if (key_event.keysym.sym == _key.down) {
			_down_state = true;
			_down_press = true;
			return;
		}
    else if (key_event.keysym.sym == _key.left) {
			_left_state = true;
			_left_press = true;
			return;
		}
    else if (key_event.keysym.sym == _key.right) {
			_right_state = true;
			_right_press = true;
			return;
		}
    else if (key_event.keysym.sym == _key.escape) {
			_escape_press = true;
			_escape_state = true;
			return;
		}
    else if (key_event.keysym.sym == _key.action) {
			_action_press = true;
			_action_state = true;
			return;
		}
    else if (key_event.keysym.sym == _key.e) {
            _E_press = true;
            _E_state = true;
            return;
        }
    else if (key_event.keysym.sym == _key.tab) {
            _tab_press = true;
            _tab_state = true;
            return;
    }
    }///if KEYDOWN
    else if (key_event.type == SDL_KEYUP){
            _any_key_released= true;

    if (key_event.keysym.sym == _key.up) {
			_up_state = false;
			_up_release= true;
			return;
		}
    else if (key_event.keysym.sym == _key.down) {
			_down_state = false;
			_down_release = true;
			return;
		}
    else if (key_event.keysym.sym == _key.left) {
			_left_state = false;
			_left_release = true;
			return;
		}
    else if (key_event.keysym.sym == _key.right) {
			_right_state = false;
			_right_release = true;
			return;
		}
    else if (key_event.keysym.sym == _key.escape) {
			_escape_state = false;
			_escape_release = true;
			return;
		}
    else if (key_event.keysym.sym == _key.action) {
			_action_state = false;
			_action_release = true;
			return;
		}
    else if (key_event.keysym.sym == _key.e) {
            _E_state = false;
            _E_release = true;
            return;
        }
    else if (key_event.keysym.sym == _key.tab) {
            _tab_state = false;
            _tab_release = true;
            return;
    }
    }///if KEYUP
}//KeyEventHandler

}//namespace neo
