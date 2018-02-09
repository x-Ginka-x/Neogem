#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "../../engines/mode.h"
#include "../../engines/input.h"
#include "../../engines/time.h"
#include "../../engines/text.h"
#include "map_physics.h"
#include "map_entity.h"


#include "../../engines/script.h"

#include "map_video.h"

namespace neo{

/**< this bool tells if MapMode should write logs in the console */
extern bool MAP_DEBUG;

enum MAP_STATE{

    NO_CHANGE,
    CHANGE
};

/** \class MapMode
 *
 *  This class manages all the processing of the main mode of the game.
 *  It can be in several states, like exploration, dialogue, etc...
 *  It updates in every loop, then draws all the scene we need to see.
 *  In MapMode, the player will be able to walk (perhaps jump), fight, speak to NPCs...
 *
 *  Map.h and the others map_* headers contain all the necessary to create a 2.5D world,
 *  with a bit of physics, with collisions, events, and rendering helpers like textures etc...
 *
 */


class MapMode: public GameMode{

public:

    static MapMode* _current_map;

    MapMode();
    MapMode(std::string);
    ~MapMode();

    /** \brief This function updates the entire visible area of the map */

    void Update();

    /** \brief This function renders all that can be seen on the screen */

    void Draw();

    /** \brief Don't know why, just in case */

    void Reset();


    /** \brief This function loads the data of the map, which will eventually be found in Lua files.
    *           for now, it's just a conventional loading.
    *
    *   \return true if successfully loaded the map, or else false
    */

    bool LoadMap();

    void ChangeMap(std::string);

    /** Accessors (used to refer to the current map's camera and events **/

    MapVideoEngine* GetViewManager() {return _view_manager;}
    PhysicsEngine* GetPhysicsManager() {return _physics_manager;}
    EventManager* GetEventManager() {return _event_manager;}

    StaticEntity* CreateStaticEntity(std::string);
    ObjectEntity* CreateObjectEntity(std::string);
    ActorEntity* CreateActorEntity(std::string);

    ObjectEntity* GetObjectEntity(std::string);
    ActorEntity* GetActorEntity(std::string);


private:

    std::string _script_file;
    std::map<std::string, StaticEntity*> _static_entities;
    std::map<std::string, ObjectEntity*> _object_entities;
    std::map<std::string, ActorEntity*> _actor_entities;

    MAP_STATE _state;

    /**the class responsible of the communication between '3D world' and VideoEngine */
    MapVideoEngine* _view_manager;
    PhysicsEngine* _physics_manager;
    EventManager* _event_manager;


    template<class A> void _UpdateEntities(A&);

    /** Temporary **/
    Image* _blank_bg;


};//class MapMode



}//namespace

#endif // MAP_H_INCLUDED
