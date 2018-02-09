#include "map_texture.h"
#include "map.h"
using namespace std;
using namespace neo;



MapTexture::MapTexture(){
    if(MAP_DEBUG) LOG(".MAP_DEBUG : create and initialize MapTexture");
    _is_visible = false;
    _current_animation = NULL;
}

MapTexture::~MapTexture(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : destroy MapTexture");
    Clear(_animations);
}


void MapTexture::AddAnimation(std::string name, Animation* anim){

    _animations.insert(std::make_pair(name, anim));
    if(_current_animation == NULL) _current_animation = _animations[name];
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

    if(_current_animation != _animations[animation_name]){

        _current_animation = _animations[animation_name];
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
        if(_current_animation == _animations[name] && _current_animation->IsPlaying() == true)
            return true;
        else return false;
    }
}

void MapTexture::Update(int update_time){

    if(_current_animation ==  NULL) return;
    else
        _current_animation->Update(update_time);

    _current_frame = _current_animation->GetCurrentFrameTexture();
}


coor2i MapTexture::GetCurrentFrameSize(){

    return _current_animation->GetCurrentFrameTexture()->GetSize();
}

Image* MapTexture::GetCurrentFrame(){

    return _current_frame;
}



void neo::MapTextureDescriptor(ScriptObject* Script){

    std::string instruction = s_text;
    if(instruction == "new"){
        MapTexture* tex = MapMode::_current_map->GetViewManager()->CreateMapTexture();
        s_register(s_text, tex);
    }
    else if(instruction == "end"){
        s_par("global");
    }
    else if(instruction == "animation"){
        MapTexture* tex = s_texture(s_active);
        std::string name = s_text;
        std::string animname = s_text;
        Animation* anim = s_animation(s_get(name));
        tex->AddAnimation(animname, anim);
    }
    else{

    }
}
