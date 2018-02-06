
#include <iostream>
#include "time.h"

using namespace std;
using namespace neo;



bool neo::TIME_DEBUG = true;

TimeEngine* neo::Time = NULL;

///------------ Timer class ------------///

Timer::Timer(){
    _state = TIMER_INVALID;
    _duration = 0;
    _number_loops = 0;
    _time_expired = 0;
    _times_completed = 0;
}//Constructor

Timer::Timer(int duration, int loops){
    _state = TIMER_INITIAL;
    _duration = duration;
    _number_loops = loops;
    _time_expired = 0;
    _times_completed = 0;
}//overloaded constructor

Timer::~Timer(){

}//Destructor

void Timer::Initialize(int duration, int number_loops) {
	_state = TIMER_INITIAL;
	_duration = duration;
	_number_loops = number_loops;
	_time_expired = 0;
	_times_completed = 0;
}

void Timer::Update(){
    Update(Time->GetUpdateTime());
}//Update

void Timer::Update(int time){
    if (IsRunning() == false) {
		return;
	}

	_UpdateTimer(time);
}//manual update


float Timer::PercentComplete() const {
	switch (_state) {
		case TIMER_INITIAL:
			return 0.0f;
		case TIMER_RUNNING:
		    return static_cast<float>(_time_expired) / static_cast<float>(_duration);
		case TIMER_PAUSED:
			return static_cast<float>(_time_expired) / static_cast<float>(_duration);
		case TIMER_FINISHED:
			return 1.0f;
		default:
			return 0.0f;
	}
}//PercentComplete

void Timer::SetDuration(int duration){
    if(IsInitial()==false){
        if(TIME_DEBUG) ERR("TIME : Trying to set duration of a non-initialized timer");
        return;
    }
    _duration = duration;
}//SetDuration

void Timer::SetNumberLoops(int loops){
    if(IsInitial()==false){
        if(TIME_DEBUG) ERR("TIME : Trying to set loops number of a non-initialized timer");
        return;
    }
    _number_loops = loops;
}//SetNumberLoops

void Timer::_UpdateTimer(int time){

    _time_expired += time;

	if (_time_expired >= _duration) {
		_times_completed++;

		if (_number_loops < 0) {
			_time_expired -= _duration;
		}
		else if (_times_completed >= _number_loops) {
			_time_expired = 0;
			_state = TIMER_FINISHED;
		}
		else {
			_time_expired -= _duration;
		}
	}

}//_UpdateTimer


///----------------- TimeEngine -----------------///


TimeEngine::TimeEngine(){
    if(TIME_DEBUG) LOG(".create time");

    _not_done = true;
}

TimeEngine::~TimeEngine(){
    if(TIME_DEBUG) LOG(".destroy time");
}



void TimeEngine::InitializeTimers() {
	_last_update = SDL_GetTicks();
	_update_time = 1; // Set to non-zero, otherwise bad things may happen...
	_hours_played = 0;
	_minutes_played = 0;
	_seconds_played = 0;
	_milliseconds_played = 0;
}

void TimeEngine::UpdateTimers() {
	// Update the update game timer
	int tmp = _last_update;
	_last_update = SDL_GetTicks();
	_update_time = _last_update - tmp;

	// Update the game play timer
	_milliseconds_played += _update_time;
	if (_milliseconds_played >= 1000) {
		_seconds_played += _milliseconds_played / 1000;
		_milliseconds_played = _milliseconds_played % 1000;
		if (_seconds_played >= 60) {
			_minutes_played += _seconds_played / 60;
			_seconds_played = _seconds_played % 60;
			if (_minutes_played >= 60) {
				_hours_played += _minutes_played / 60;
				_minutes_played = _minutes_played % 60;
			}
		}
	}
}//UpdateTimers
