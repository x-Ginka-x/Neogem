#include "map_video.h"
#include "../../engines/time.h"

using namespace std;
using namespace neo;



MapVideoEngine::MapVideoEngine(){

    if(MAP_DEBUG) LOG(".MAP_DEBUG : create map video");
    _camera_offset.w = SCREEN_WIDTH;
    _camera_offset.h = SCREEN_HEIGHT;

}//Constructor

MapVideoEngine::~MapVideoEngine(){
    if(MAP_DEBUG) LOG(".MAP_DEBUG : destroy map video");
    Clear(_texture_objects);

}//Destructor

void MapVideoEngine::Update(){

    _camera_offset.x = toscreen(_camera_target).x - 640.0f;
    _camera_offset.y = toscreen(_camera_target).y - 360.0f;
    for(auto it = _texture_objects.begin(); it != _texture_objects.end(); ++it){
        (*it)->Update(Time->GetUpdateTime());
    }

}//Update


void MapVideoEngine::Draw(){

    coor3f pos;
    coor3f coor;
    float z = 1;

    for(multimap<d_int, Image*>::iterator it = _ordered_textures.begin(); it != _ordered_textures.end(); ++it){

        d_int depth = (*it).first;
        coor.x = depth.x + depth.w/2.0f;
        coor.y = depth.y + depth.h/2.0f;
        coor.z = depth.z + depth.d/2.0f;

        pos = tocameraf(coor);
        Video->SetCursorPos(pos.x, pos.y, z);
        (*it).second->Draw();
        z += 1.0;
    }

    _ordered_textures.clear();

}//Draw

coor3f MapVideoEngine::toscreen(coor3f P){

    return toscreen(P.x, P.y, P.z);
}//toscreen

coor3f MapVideoEngine::toscreen(float x, float y, float z){

    return coor3f((x * 2.0f - z * 2.0f), (x +z - y * 2.0f), 0.0);
}//toscreen


coor3f MapVideoEngine::tocamera(float x, float y, float z){

    coor3f point = toscreen(x, y, z);
    point.x -= _camera_offset.x;
    point.y -= _camera_offset.y;

    return point;
}//tocamera

coor3f MapVideoEngine::tocamera(coor3f P){

    return tocamera(P.x, P.y, P.z);
}//tocamera

/**Fast version of tocamera(doesn't call toscreen)**/
coor3f MapVideoEngine::tocameraf(coor3f P){

    return coor3f(((P.x * 2.0f - P.z * 2.0f) - _camera_offset.x), ((P.x + P.z - P.y * 2.0f) - _camera_offset.y), 0.0);
}


/*** TODO

Point* MapVideoEngine::toworld(int x, int y){

return NULL;
}

***/
/*** NEW INTERFACE ***/

void MapVideoEngine::RegisterTextureForSorting(Image* tex, coor3f pos, coor3f siz){
    if(tex == NULL){
        if(MAP_DEBUG) ERR(".MAP_DEBUG : Registered a non initialized AnimationFrame (_texture = NULL)");
        return;
    }

    /**Don't wanna draw out-of-reach textures **/

    coor3f tpos = tocamera(pos);
    coor2i tsiz = tex->GetSize();

    if(tpos.x + tsiz.x < 0 ||
       tpos.x > SCREEN_WIDTH ||
       tpos.y + tsiz.y < 0 ||
       tpos.y > SCREEN_HEIGHT){
        return;
    }

    /**-------------**/
    d_int depth(pos.x,pos.y,pos.z,siz.x,siz.y,siz.z);
    _ordered_textures.insert(make_pair(depth, tex));
}

MapTexture* MapVideoEngine::CreateMapTexture(){

    MapTexture* tex = new MapTexture();
    _texture_objects.push_back(tex);
    return _texture_objects.back();

}
