#ifndef DEFS_H_INCLUDED
#define DEFS_H_INCLUDED

typedef unsigned short ush;

namespace neo{


    extern bool INPUT_DEBUG;
    class InputEngine;


    extern bool VIDEO_DEBUG;
    class VideoEngine;
        const unsigned short SCREEN_WIDTH = 1280;
        const unsigned short SCREEN_HEIGHT = 720;
    class Image;


    extern bool TIME_DEBUG;
    class TimeEngine;
    class Timer;

    class ScriptObject;

    extern bool MODE_DEBUG;
    class ModeManager;
    class GameMode;
        class MapMode;
        extern bool MAP_DEBUG;
//        extern MapMode* _current_map;
            class AnimationFrame;
            class Animation;
            class MapTexture;
            class Point;
            class Mesh;


                enum DIRECTION{

                    NORTH,
                    NORTH_EAST,
                    EAST,
                    SOUTH_EAST,
                    SOUTH,
                    SOUTH_WEST,
                    WEST,
                    NORTH_WEST,
                    UP,
                    DOWN
                };

                enum OBJECT_TYPE{

                    OBJECT_TYPE_INVALID,
                    OBJECT_TYPE_STATIC,
                    OBJECT_TYPE_MOVABLE,
                    OBJECT_TYPE_ACTOR
                };


                enum ENERGY_TYPE{

                    ENERGY_TYPE_JOULE,
                    ENERGY_TYPE_METERSECOND
                };

            class MapVideoEngine;
            class Entity;
                class StaticEntity;
                class ObjectEntity;
                class ActorEntity;

            class DialogManager;
            class PhysicsEngine;

            namespace event{

                class Condition;
            }

        class TestMode;
}

#endif // DEFS_H_INCLUDED
