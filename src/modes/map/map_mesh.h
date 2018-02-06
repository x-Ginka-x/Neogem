#ifndef MAP_MESH_H_INCLUDED
#define MAP_MESH_H_INCLUDED


#include<SDL.h>

#include <string>
#include <vector>
#include <map>

#include "../../defs.h"
#include "../../utils.h"


namespace neo{

extern bool MESH_DEBUG;


class Mesh{

protected:

    static float _jouletospeed(float, float);
    static float _speedtojoule(float, float);


    coor3f _pos;
    coor3f _size;


    float _left,
        _right,
        _top,
        _bottom,
        _front,
        _back;

    coor3f _hot_left,
           _hot_right,
           _hot_top,
           _hot_bottom,
           _hot_front,
           _hot_back,
           _anchor,
           _center;

    bool _is_moving;
    bool _is_solid;
    bool _is_initialized;
    bool _position_changed;
    bool _on_ground;


    std::vector<Mesh*> _colliding_meshes;

    std::map<DIRECTION, bool> _blocked_directions;
    std::map<DIRECTION, float> _kinetic_energy;

    bool _is_static;


    float _mass;
    float _resistance;
    int _bounce;


    ///*** Private function used to calculate 'key positions'
    ///*** like screen position of the entity
    void _CalculatePosition();

public:
    /** WHAT ??? ---> **/

    Mesh();
    ~Mesh();


    void SetSize(float, float, float);
    void SetPos(float,float, float);

    coor3f GetSize() const {return _size;}
    coor3f GetPos(); //const {return _pos;}

    coor3f GetAnchor() {return _anchor;}
    coor3f GetCenter() {return _center;}

    Mesh* SetSolid(bool solid){_is_solid = solid; return this;}
    bool IsSolid() const {return _is_solid;}

    void Move(DIRECTION, float);



    float GetMass() const {return _mass;}
    Mesh* SetMass(float mass){_mass = mass; return this;}

    float GetResistance() const {return _resistance;}
    void SetResistance(float res) {_resistance = res;}

    int GetBounce() const {return _bounce;}
    void SetBounce(int bounce) {_bounce = bounce;}

    bool IsInitialized() const {return _is_initialized;}

    void Update(int update_time);
    void Initialize() {_CalculatePosition();}

    /*** NEW PHYSIC INTERFACE TEST ***/

    void ApplyForce(DIRECTION, ENERGY_TYPE, float, bool is_capped = false);
    void ResetForces();

    bool IsBlocked(DIRECTION dir){return _blocked_directions.at(dir);}
    void SetBlocked(DIRECTION dir, bool value){_blocked_directions.at(dir) = value;}

    bool IsStatic() const {return _is_static;}
    void SetStatic(bool value){_is_static = value;}

    /**----------------------------***/


    bool IsColliding(Mesh*);
    bool IsColliding(coor3f&);
    void ResolveCollision(Mesh*);
    bool IsMoving() const  {return _is_moving;}
    bool PositionChanged() const {return _position_changed;}
    bool IsOnGround() const {return _on_ground;}

    std::vector<Mesh*> GetCollidingMesh(){return _colliding_meshes;}


private:


};//class Mesh



}//namespace

#endif // MAP_MESH_H_INCLUDED
