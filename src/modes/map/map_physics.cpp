#include "map_physics.h"

using namespace std;
using namespace neo;

bool neo::PHYSICS_DEBUG = true;

PhysicsEngine::PhysicsEngine(){

    _gravity = 5.1f;
    _update_zone = int4(0, 0, 1280, 640);
}

PhysicsEngine::~PhysicsEngine(){

    Clear(_meshes);
}


void PhysicsEngine::SetUpdateZone(int x, int y,int x2, int y2){

    _update_zone = int4(x, y, x2, y2);
}

void PhysicsEngine::SetUpdateZone(int4 zone){

    _update_zone = zone;
}

bool PhysicsEngine::RegisterMesh(Mesh* mesh){

    if(mesh == NULL){
        if(PHYSICS_DEBUG) ERR(".PHYSICS_DEBUG : Tried to register non-existing mesh !");
        return false;
    }

    _non_updatable_meshes.insert(mesh);
    return true;
}

bool PhysicsEngine::UnregisterMesh(Mesh* mesh){

    if(_updatable_meshes.find(mesh) != _updatable_meshes.end()){
        _updatable_meshes.erase(mesh);
    }
    else if(_non_updatable_meshes.find(mesh) != _non_updatable_meshes.end()){
        _non_updatable_meshes.erase(mesh);
    }
    else{
        if(PHYSICS_DEBUG) ERR(".PHYSICS_DEBUG : Tried to unregister non registered mesh !");
        return false;
    }
    return true;
}

Mesh* PhysicsEngine::CreateMesh(){

    Mesh* mesh = new Mesh();
    _meshes.push_back(mesh);
    RegisterMesh(mesh);
    return _meshes.back();
}


bool PhysicsEngine::_IsMeshInUZone(Mesh* mesh){

    int4 meshzone(mesh->GetPos().x, mesh->GetPos().z, mesh->GetPos().x + mesh->GetSize().x, mesh->GetPos().z + mesh->GetSize().z);

    if(meshzone.c < _update_zone.a ||
       meshzone.a > _update_zone.c ||
       meshzone.d < _update_zone.b ||
       meshzone.b > _update_zone.d)
        return false;

    return true;
}

bool PhysicsEngine::_CheckCollisions(Mesh* mesh){

    bool _return = false;

    for(set<Mesh*>::iterator it = _updatable_meshes.begin(); it != _updatable_meshes.end(); ++it){
        if(mesh != *it && mesh->IsColliding(*it))
            _return = true;
    }

    return _return;
}


void PhysicsEngine::Update(int update_time){

    vector<Mesh*> temp_garbage;
    vector<Mesh*> temp_garbage2;

    /** Check if mesh is in update zone, if yes update it **/

    for(set<Mesh*>::iterator it = _updatable_meshes.begin();it!= _updatable_meshes.end(); ++it){

        if(!_IsMeshInUZone(*it)){
            _non_updatable_meshes.insert(*it);
            temp_garbage.push_back(*it);
        }

        else{
            if(_gravity > 0 && (*it)->IsStatic() == false)
                if((*it)->IsOnGround() == false)
                    (*it)->ApplyForce(DOWN, ENERGY_TYPE_METERSECOND, _gravity);

            (*it)->Update(update_time);
        }
    }

    for(set<Mesh*>::iterator it = _non_updatable_meshes.begin();it != _non_updatable_meshes.end(); ++it){

        if(_IsMeshInUZone(*it)){

            if(_gravity > 0 && (*it)->IsStatic() == false)
                if((*it)->IsOnGround() == false)
                    (*it)->ApplyForce(DOWN, ENERGY_TYPE_METERSECOND, _gravity);

            (*it)->Update(update_time);
            _updatable_meshes.insert(*it);
            temp_garbage2.push_back(*it);
        }
    }

    /** Clean the remnants of previous operations **/

    while(!temp_garbage.empty()){
        _updatable_meshes.erase(temp_garbage.back());
        temp_garbage.pop_back();
    }

    while(!temp_garbage2.empty()){
        _non_updatable_meshes.erase(temp_garbage2.back());
        temp_garbage2.pop_back();
    }


}//Update


void PhysicsEngine::ManageCollisions(){

    for(set<Mesh*>::iterator it = _updatable_meshes.begin();it!= _updatable_meshes.end(); ++it){
        if((*it)->IsMoving())
            _moving_meshes.push_back(*it);
    }

    Mesh* mesh = NULL;//holds the mesh that will be tested below

    while(_moving_meshes.empty() == false){

        mesh = _moving_meshes.back();

        if(_CheckCollisions(mesh) == true){

            _mesh_collision_resolving_helper = mesh->GetCollidingMesh();

            while(_mesh_collision_resolving_helper.empty() == false){

                if(_mesh_collision_resolving_helper.back()->IsSolid() && mesh->IsSolid()){
                    mesh->ResolveCollision(_mesh_collision_resolving_helper.back());
                }
                _mesh_collision_resolving_helper.pop_back();

            }
        }

        _moving_meshes.pop_back();
    }

}//ManageCollisions
