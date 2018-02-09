#ifndef MAP_TEXTURE_H_INCLUDED
#define MAP_TEXTURE_H_INCLUDED

#include <SDL.h>

#include <string>
#include <map>
#include <vector>

#include "../../defs.h"

#include "../../engines/animation.h"

namespace neo{



class MapTexture{

public:

    MapTexture();
    ~MapTexture();


    void Play(std::string);
    bool IsPlaying(std::string animation_name);
    bool CurrentAnimationIsFinished();

    coor2i GetCurrentFrameSize();
    Animation* GetCurrentAnimation(){return _current_animation;}
    Image* GetCurrentFrame();

    void AddAnimation(std::string, Animation*);

    void Update(int);

    void SetVisible(bool visible){_is_visible = visible;}
    bool IsVisible(){return _is_visible;}

protected:


private:

    Animation* _current_animation;
    Image* _current_frame;
    std::map<std::string, Animation*> _animations;

    bool _is_visible;


};//class MapTexture

void MapTextureDescriptor(ScriptObject*);

}//namespace

#endif // MAP_TEXTURE_H_INCLUDED
