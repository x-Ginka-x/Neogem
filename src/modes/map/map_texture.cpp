#include "map_texture.h"

using namespace std;
using namespace neo;



MapTexture::MapTexture(){
    if(MAP_DEBUG) LOG(".MAP_DEBUG : create and initialize MapTexture");
    _is_visible = false;
    _current_animation = NULL;
}

MapTexture::~MapTexture(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : destroy MapTexture");
//    for(auto it = _animations.begin(); it != _animations.end(); ++it){
//        delete (*it).second;
//    }
}


void MapTexture::AddAnimation(std::string name, Animation anim){

    _animations.insert(std::make_pair(name, anim));
    if(_current_animation == NULL) _current_animation = &_animations[name];
}


bool MapTexture::CurrentAnimationIsFinished(){

    if(_current_animation == NULL){
        return true;
    }
    else
        return _current_animation->IsFinished();
}


void MapTexture::Play(std::string animation_name){

    if(_animations.find(animation_name) == _animations.end()) return;

    if(_current_animation != &_animations[animation_name]){

        _current_animation = &_animations[animation_name];
        _current_animation->Reset();

    }

    if(_current_animation->IsFinished())
        _current_animation->Reset();

    _current_animation->Play();

}


bool MapTexture::IsPlaying(string name){

    if(_animations.find(name) == _animations.end())
        return false;
    else{
        if(_current_animation == &_animations[name] && _current_animation->IsPlaying() == true)
            return true;
        else return false;
    }
}

void MapTexture::Update(int update_time){

    if(_current_animation ==  NULL) return;
    else
        _current_animation->Update(update_time);

}


coor2i MapTexture::GetCurrentFrameSize(){

    return _current_animation->GetCurrentFrameTexture()->GetSize();
}

Image* MapTexture::GetCurrentAnimationFrameTexture(){

    return _current_animation->GetCurrentFrameTexture();
}
