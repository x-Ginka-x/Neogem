#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL.h>
#include <string>

#include "../utils.h"
#include "../defs.h"

namespace neo{

extern InputEngine* Input;

extern bool INPUT_DEBUG;

namespace detail{

class KeyState{

public :

    SDL_Keycode up;
    SDL_Keycode down;
    SDL_Keycode left;
    SDL_Keycode right;
    SDL_Keycode action;
    SDL_Keycode goback;
    SDL_Keycode tab;
    SDL_Keycode pause;
    SDL_Keycode escape;
    SDL_Keycode e;

    /**************** TODO!!!!!!!!!! ****************/
    /****** Add support for the remaining keys ******/

};//class KeyState

}//namespace neo

class InputEngine{

private:

    detail::KeyState _key;

    bool _any_key_pressed;
    bool _any_key_released;

    bool _up_state;
	bool _down_state;
	bool _left_state;
	bool _right_state;
	bool _escape_state;
    bool _action_state;
    bool _E_state;
    bool _tab_state;

    bool _up_press;
	bool _down_press;
	bool _left_press;
	bool _right_press;
	bool _escape_press;
	bool _action_press;
	bool _E_press;
    bool _tab_press;

    bool _up_release;
	bool _down_release;
	bool _left_release;
	bool _right_release;
	bool _escape_release;
	bool _action_release;
    bool _E_release;
    bool _tab_release;

	SDL_Event _event;

	void _KeyEventHandler(SDL_KeyboardEvent& key_event);

	/***LATER : void _SetNewKey(SDL_Keycode & old_key, SDL_Keycode new_key);***/


public:

    InputEngine();
    ~InputEngine();

    bool AnyKeyPressed();
    bool AnyKeyReleased();

    void Update();

    /** State members access functions***/

    bool UpState() const
		{ return _up_state; }

	bool DownState() const
		{ return _down_state; }

	bool LeftState() const
		{ return _left_state; }

	bool RightState() const
		{ return _right_state; }

    bool EscapeState() const
        { return _escape_state; }

    bool ActionState() const
        { return _action_state; }

    bool EState() const {
        return _E_state;
    }

    bool TabState() const {
        return _tab_state;
    }

	/*** Press members access functions ***/

    bool UpPress() const
		{ return _up_press; }

	bool DownPress() const
		{ return _down_press; }

	bool LeftPress() const
		{ return _left_press; }

	bool RightPress() const
		{ return _right_press; }

    bool EscapePress() const
        { return _escape_press; }

    bool ActionPress() const
        { return _action_press; }

    bool EPress() const {
        return _E_press;
    }

    bool TabPress() const {
        return _tab_press;
    }

    /*** Release members access functions ***/

    bool UpRelease() const
		{ return _up_release; }

	bool DownRelease() const
		{ return _down_release; }

	bool LeftRelease() const
		{ return _left_release; }

	bool RightRelease() const
		{ return _right_release; }

    bool EscapeRelease() const
        { return _escape_release; }

    bool ActionRelease() const
        { return _action_release; }

    bool ERelease() const {
        return _E_release;
    }

    bool TabRelease() const {
        return _tab_release;
    }

    SDL_Event GetEvent() {return _event;}

    /*** Key names access functions ***/

    std::string GetUpKeyName() const
		{ return SDL_GetKeyName(_key.up); }

	std::string GetDownKeyName() const
		{ return SDL_GetKeyName(_key.down); }

	std::string GetLeftKeyName() const
		{ return SDL_GetKeyName(_key.left); }

	std::string GetRightKeyName() const
		{ return SDL_GetKeyName(_key.right); }

    std::string GetActionKeyName() const
    { return SDL_GetKeyName(_key.action); }


    /*** Key setting functions ***/

    /*void SetUpKey(const SDL_Keycode& key)
		{ _SetNewKey(_key.up, key); }

	void SetDownKey(const SDL_Keycode& key)
		{ _SetNewKey(_key.down, key); }

	void SetLeftKey(const SDL_Keycode& key)
		{ _SetNewKey(_key.left, key); }

	void SetRightKey(const SDL_Keycode& key)
		{ _SetNewKey(_key.right, key); }*/

    /*** SDL_Key _key members access functions ***/

/*  int32 GetUpKey() const
		{ return _key.up; }

	int32 GetDownKey() const
		{ return _key.down; }

	int32 GetLeftKey() const
		{ return _key.left; }

	int32 GetRightKey() const
		{ return _key.right; }*/
    /***TODO***/

};//class InputEngine


}//namespace neo

#endif // INPUT_H_INCLUDED
