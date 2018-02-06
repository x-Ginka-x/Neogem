#include "map_entity.h"

using namespace std;
using namespace neo;


Entity::Entity(){
    _name = "";
    _visible = true;
    _aabb_visible = false;
    _mesh = NULL;
    _texture = NULL;
}

Entity::~Entity(){

}


void Entity::AddPassiveEvent(MapEventString* event_string){

    _passive_events.push_back(event_string);
}

void Entity::AddActiveEvent(MapEventString* event_string){

    _active_events.push_back(event_string);
}

void Entity::LinkMesh(Mesh* mesh){

    _mesh = mesh;
}


void Entity::PlayActive(){

    for(auto it = _active_events.begin(); it != _active_events.end(); it++){
        (*it)->Play();
    }
}

void Entity::PlayPassive(){

    for(auto it = _passive_events.begin(); it != _passive_events.end(); it++){
        (*it)->Play();
    }
}

bool Entity::IsColliding(Entity* E){

    vector<Mesh*> colliding_meshes = _mesh->GetCollidingMesh();
    for(vector<Mesh*>::iterator it = colliding_meshes.begin(); it != colliding_meshes.end(); ++it){
        if(*it == E->_mesh){
            return true;
        }
    }
    return false;
}


StaticEntity::StaticEntity(){
    if(MAP_DEBUG) LOG(".create and initialize [static object]");

}

StaticEntity::~StaticEntity(){
    if(MAP_DEBUG) LOG((".destroy [static object]\n  -> name = " + _name));
}


void StaticEntity::Update(int update_time){

    _texture->Play("DEFAULT");
}




ObjectEntity::ObjectEntity(string name){
    if(MAP_DEBUG) LOG((".create and initialize [map object]\n  -> name = " + name));

    _name = name;
    _state = "DEFAULT";

}

ObjectEntity::~ObjectEntity(){
    LOG((".destroy [map object]\n  -> name = " + _name));

}


void ObjectEntity::Update(int update_time){

    if(_texture->CurrentAnimationIsFinished())
        _texture->Play("DEFAULT");
}


ActorEntity::ActorEntity(string name){
    _name = name;
    _state = "STANDING_EAST";
    _is_falling = false;
    _is_jumping = false;
    _is_walking = false;
    _direction = EAST;

}

ActorEntity::~ActorEntity(){

}


void ActorEntity::Update(int update_time){

    _is_falling = _mesh->IsOnGround() == true ? false : true;

    if(_is_jumping == true && _is_falling  == false){
        LOG("jumping");
        _mesh->ApplyForce(UP, ENERGY_TYPE_METERSECOND, 5.0f);
        _is_falling = true;
    }

    if(_is_falling == true)
        _is_walking = false;


    if(_is_walking == false){

        if(_is_falling ==  false)
            _mesh->ResetForces();

        switch (_direction)
        {
        case NORTH:
            _state = "STANDING_NORTH";
            break;
        case NORTH_EAST:
            _state = "STANDING_NORTH_EAST";
            break;
        case EAST:
            _state = "STANDING_EAST";
            break;
        case SOUTH_EAST:
            _state = "STANDING_SOUTH_EAST";
            break;
        case SOUTH:
            _state = "STANDING_SOUTH";
            break;
        case SOUTH_WEST:
            _state = "STANDING_SOUTH_WEST";
            break;
        case WEST:
            _state = "STANDING_WEST";
            break;
        case NORTH_WEST:
            _state = "STANDING_NORTH_WEST";
            break;
        default:
            _state = "DEFAULT";
            break;
        }
    }
    else if(_is_walking == true){

        _mesh->ResetForces();
        _mesh->ApplyForce(_direction, ENERGY_TYPE_METERSECOND, 10.0f, true);
    }

    _texture->Play(_state);

    _is_walking = false;
    _is_jumping = false;

}//Update


void ActorEntity::Initialize(){


}

void ActorEntity::_SpecialUpdate(){

}


void ActorEntity::Walk(DIRECTION dir){

    if(_mesh->IsOnGround())
        _is_walking = true;
    else
        _is_walking = false;

    _direction = dir;


    switch (_direction)
    {
    case NORTH:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_NORTH";
        break;

    case NORTH_EAST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_NORTH_EAST";
        break;

    case EAST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_EAST";
        break;

    case SOUTH_EAST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_SOUTH_EAST";
        break;

    case SOUTH:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
            _state = "WALKING_SOUTH";
        break;

    case SOUTH_WEST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_SOUTH_WEST";
        break;

    case WEST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_WEST";
        break;

    case NORTH_WEST:
        if(_is_falling == true)
            _state = "DEFAULT";
        else
        _state = "WALKING_NORTH_WEST";
        break;

    default:
        _state = "DEFAULT";
        break;
    }


}


void ActorEntity::Jump(){
    LOG("jump");
    _is_jumping = true;
}
