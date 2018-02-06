#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "../defs.h"
#include "../utils.h"

namespace neo{

extern bool TIME_DEBUG;

extern TimeEngine* Time;


///*** Some flags and enums that will be useful in the time machine ***///

const int SYSTEM_INFINITE_TIME = 0xFFFFFFFF;
const int SYSTEM_TIMER_NO_LOOPS = 0;
const int SYSTEM_TIMER_INFINITE_LOOP = -1;

enum TIMER_STATE {
	TIMER_INVALID  = -1,
	TIMER_INITIAL  =  0,
	TIMER_RUNNING  =  1,
	TIMER_PAUSED   =  2,
	TIMER_FINISHED =  3,
	TIMER_TOTAL    =  4
};


///=================================================================///


/** Here is the standard timer class using throughout the game and **/
        ///*** the class responsible of timers management ***///


///=================================================================///



class Timer {
	friend class TimeEngine;

public:

	Timer();

    Timer(int duration, int loops = 0);

	virtual ~Timer();

	void Initialize(int duration, int loops = 0);



	virtual void Update();

	virtual void Update(int time);

	void Reset()
		{ if (_state != TIMER_INVALID) { _state = TIMER_INITIAL; _time_expired = 0; _times_completed = 0; } }

	void Run()
		{ if (IsInitial() || IsPaused()) _state = TIMER_RUNNING; }

	void Pause()
		{ if (IsRunning()) _state = TIMER_PAUSED; }

	void Finish()
		{ _state = TIMER_FINISHED; }



	bool IsInitial() const
		{ return (_state == TIMER_INITIAL); }

	bool IsRunning() const
		{ return (_state == TIMER_RUNNING); }

	bool IsPaused() const
		{ return (_state == TIMER_PAUSED); }

	bool IsFinished() const
		{ return (_state == TIMER_FINISHED); }



	int CurrentLoop() const
		{ return (_times_completed + 1); }

	int TimeLeft() const
		{ return (_duration - _time_expired); }


	float PercentComplete() const;

	void SetDuration(int duration);

	void SetNumberLoops(int loops);




	TIMER_STATE GetState() const
		{ return _state; }

	int GetDuration() const
		{ return _duration; }

	int GetNumberLoops() const
		{ return _number_loops; }

	int GetTimeExpired() const
		{ return _time_expired; }

	int GetTimesCompleted() const
		{ return _times_completed; }


protected:

	TIMER_STATE _state;


	int _duration;

	int _number_loops;


	int _time_expired;

	int _times_completed; ///each time the timer reaches TIME_FINISHED state



	void _UpdateTimer(int);
}; // class Timer





class TimeEngine{

public:

	TimeEngine();
	~TimeEngine();


	void InitializeTimers();

	void InitializeUpdateTimer()
		{ _last_update = SDL_GetTicks(); _update_time = 1; }


	void UpdateTimers();

	void ExamineTimers();


	int GetUpdateTime() const
		{ return _update_time; }

	void SetPlayTime(const int h, const int m, const int s)
		{ _hours_played = h; _minutes_played = m; _seconds_played = s; _milliseconds_played = 0; }

	int GetPlayHours() const
		{ return _hours_played; }

	int GetPlayMinutes() const
		{ return _minutes_played; }

	int GetPlaySeconds() const
		{ return _seconds_played; }


	///===========================================///

	bool NotDone() const
		{ return _not_done; }

	void ExitGame()
		{ _not_done = false; }


private:

	int _last_update;
    int _update_time;

	int _hours_played;
	int _minutes_played;
	int _seconds_played;
	int _milliseconds_played; //!< \note Milliseconds are not retained when saving or loading a saved game file.

    bool _not_done;

}; // class TimeEngine


}///namespace neo_system

#endif // TIME_H_INCLUDED
