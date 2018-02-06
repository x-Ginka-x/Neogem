#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <vector>
#include <string>

#include "image.h"

namespace neo{


class AnimationFrame{

    friend class Animation;

private:
    Image* _texture;
    int _duration;
    AnimationFrame();

public:
    AnimationFrame(Image*, int);
    ~AnimationFrame();

};



class Animation{
//    friend class MapTexture;

private:

    std::string _file;
    std::vector<std::pair<int4, int> > _frames_data;



    int _loop_count;
    int _times_played;
    int _times_to_play;
    int _frame_count;
    int _frame_total;
    bool _is_finished;

    bool _is_playing;
    bool _is_initialized;

    std::vector<AnimationFrame*> _frames;
    AnimationFrame* _current_frame;

public:
    Animation();
    ~Animation();

    void PushFrame(Image*, int);
    void Pop(){_frames.pop_back();}

    void Play(){_is_playing = true;}
    void Stop(){_is_playing = false;}
    void Reset(){_loop_count = 0; _is_finished = false;}

    Image* GetCurrentFrameTexture();
    bool IsPlaying() const {return _is_playing;}
    bool IsFinished() const {return _is_finished;}
    void SetTimesToPlay(int n){_times_to_play = n;}

    void Update(int);

};

}//namespace
#endif // ANIMATION_H_INCLUDED
