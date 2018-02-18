#ifndef MAP_VIDEO_H_INCLUDED
#define MAP_VIDEO_H_INCLUDED

#include <map>

#include "../../defs.h"
#include "../../utils.h"

#include "../../engines/video.h"
    #include "../../engines/image.h"
#include "map_texture.h"

namespace neo{

class MapVideoEngine{

public:

    MapVideoEngine();
    ~MapVideoEngine();

    /*** Camera Functions ***/

    void Follow(coor3f P) {_camera_target = P;}

    /*** ----------------------- ***/
    void Update();
    void Draw();

    coor3f toscreen(coor3f);
    coor3f toscreen(float, float, float);
    coor3f tocamera(float, float ,float);
    coor3f tocamera(coor3f);
    coor3f tocameraf(coor3f);

    coor4f GetOffset() {return _camera_offset;}

    void RegisterTextureForSorting(Image*, coor3f, coor3f);

    MapTexture* CreateMapTexture();

protected:

private:

    coor3f _camera_target;
    coor4f _camera_offset;

    std::multimap<d_int, Image*> _ordered_textures;
    std::vector<MapTexture*> _texture_objects;

};//class MapVideoEngine

}//namespace

#endif // MAP_VIDEO_H_INCLUDED
