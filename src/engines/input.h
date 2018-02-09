#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <SDL.h>
#include <string>

#include "../utils.h"
#include "../defs.h"

namespace neo{

extern InputEngine* Input;

extern bool INPUT_DEBUG;



class InputEngine{

private:

    bool _any_key_pressed;
    bool _any_key_released;

	SDL_Event _event;

	void _KeyEventHandler(SDL_KeyboardEvent& key_event);


	std::map<SDL_Keycode, bool> _inputpress;
	std::map<SDL_Keycode, bool> _inputrelease;
	std::map<SDL_Keycode, bool> _inputstate;

public:

    InputEngine();
    ~InputEngine();

    bool AnyKeyPressed();
    bool AnyKeyReleased();

    void Update();


    bool Press(SDL_Keycode key);
	bool Release(SDL_Keycode key);
	bool State(SDL_Keycode key);


    SDL_Event GetEvent() {return _event;}


};//class InputEngine


}//namespace neo

#endif // INPUT_H_INCLUDED
