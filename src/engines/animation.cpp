#include "animation.h"

using namespace std;
using namespace neo;

AnimationFrame::AnimationFrame(){
    _texture = ImgManager->GetImage("block");
}

AnimationFrame::AnimationFrame(Image* img, int duration){

    _texture = img;
    _duration = duration;
}

AnimationFrame::~AnimationFrame(){
    _texture = NULL;
}




Animation::Animation(){

    _loop_count = 0;
    _frame_count = 0;
    _frame_total = 0;
    _times_played = 0;
    _times_to_play = 0;
    _is_finished = true;
    _is_playing = false;
    _current_frame = NULL;
    _is_initialized = false;
}


Animation::~Animation(){
    for(vector<AnimationFrame*>::iterator it = _frames.begin(); it != _frames.end(); ++it)
        delete (*it);
}



void Animation::PushFrame(Image* img, int duration){

    AnimationFrame* frame = new AnimationFrame(img, duration);
    _frames.push_back(frame);
    _frame_total++;
    if(_current_frame == NULL) _current_frame = _frames.back();
    _is_initialized = true;
}

void Animation::Update(int update_time){

    if(_frame_count >= (int)_frames.size()) return;
    if(_frames.size() <= 1) return;
    _is_finished = false;
    _current_frame = _frames.at(_frame_count);


    if(_is_playing){
        _loop_count += update_time;

        if(_loop_count > _current_frame->_duration){
            _loop_count = 0;
            _frame_count++;
        }
        if(_frame_count > _frame_total - 1){
            _frame_count = 0;
            _times_played++;
        }
        if(_times_played >= _times_to_play){
            _times_played = 0;
            if(_times_to_play > 0){
                _is_playing = false;
                _is_finished = true;
            }
        }
    }

}//Update()


Image* Animation::GetCurrentFrameTexture(){

    if(_is_initialized == false || _current_frame == NULL) return NULL;
    else return _current_frame->_texture;
}



void neo::AnimationDescriptor(ScriptObject* Script){

    std::string instruction = s_text;

    if(instruction == "new"){
        Animation* anim = new Animation();
        s_register(s_text, anim);
    }
    else if(instruction == "frame"){
        Animation* anim = s_animation(s_active);
        std::string str = s_text;
        anim->PushFrame(ImgManager->GetImage(str), s_int);
    }
    else if(instruction == "time"){
        Animation* anim = s_animation(s_active);
        anim->SetTimesToPlay(s_int);
    }
    else if(instruction == "end"){
        s_par("global");
    }
}
