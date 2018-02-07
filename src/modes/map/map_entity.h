#ifndef MAP_ENTITY_H_INCLUDED
#define MAP_ENTITY_H_INCLUDED

#include <string>

#include "../../defs.h"

#include "map_mesh.h"
#include "map_texture.h"
#include "map_event.h"
//#include "map.h"


namespace neo{

/** This class' purpose is to act as a bridge between MapMode and components like **/
  /** Mesh, MapTexture or Event, grouping multiple instances into one Entity **/
/** An Entity represents an object in the map **/

class Entity{

public:

    std::string GetName() {return _name;}
    void SetName(std::string name){_name = name;}


    virtual void Update(int){}

    /*** Mesh management ***/

    void LinkMesh(Mesh*);
    Mesh* GetMesh() const {return _mesh;}

    coor3f GetPos() {return _mesh->GetPos();}
    coor3f GetSize(){return _mesh->GetSize();}
    coor3f GetAnchor() {return _mesh->GetAnchor(); }
    coor3f GetCenter() {return _mesh->GetCenter();}
    bool IsColliding(Entity* E);

    bool HasMoved() const {return _mesh->PositionChanged();}


    /*** Texture management ***/

    MapTexture* GetTexture() {return _texture;}
    void LinkMapTexture(MapTexture* tex) {_texture = tex;_texture->SetVisible(true);}
    void SetVisible(bool visible){_texture->SetVisible(visible);}
    bool IsVisible(){return _texture->IsVisible();}


    void PlayPassive();
    void PlayActive();

    void AddActiveEvent(MapEventString*);
    void AddPassiveEvent(MapEventString*);

    bool _aabb_visible;


protected:

    Entity();
    virtual ~Entity();

    std::string _name;

    Mesh* _mesh;
    MapTexture* _texture;

    std::vector<MapEventString*> _passive_events;
    std::vector<MapEventString*> _active_events;

    bool _visible;


private:


};//class Entity


class StaticEntity : public Entity{

public:
    StaticEntity();
    ~StaticEntity();

    void Update(int);
    void Initialize(){_mesh->Initialize();}

private:

};//class StaticEntity



class ObjectEntity : public Entity{

public:

    ObjectEntity(std::string);
    ~ObjectEntity();

    void Update(int);

private:

    std::string _state;

};//class ObjectEntity


class ActorEntity : public Entity{

public:

    ActorEntity(std::string);
    ~ActorEntity();

    void Update(int);

    virtual void Initialize();

    void Walk(DIRECTION dir);
    void Face(DIRECTION dir) {_direction = dir;}
    void Run(DIRECTION dir){}
    void Jump();

    bool IsWalking() const {return _is_walking;}
    bool IsJumping() const {return _is_jumping;}
    bool IsFalling() const {return _is_falling;}



private:

    std::string _state;
    bool _is_walking;
    bool _is_jumping;
    bool _is_falling;
    DIRECTION _direction;

    virtual void _SpecialUpdate();

};//class ActorEntity

}//namespace

#endif // MAP_ENTITY_H_INCLUDED