#ifndef MAP_PHYSICS_H_INCLUDED
#define MAP_PHYSICS_H_INCLUDED

#include <set>
#include <vector>

#include "../../utils.h"

#include "map_mesh.h"

namespace neo{

extern bool PHYSICS_DEBUG;

class PhysicsEngine{

private:

    int4 _update_zone;
    float _gravity;

    std::vector<Mesh*> _meshes;

    std::set<Mesh*> _updatable_meshes;
    std::set<Mesh*> _non_updatable_meshes;

    std::vector<Mesh*> _moving_meshes;

    std::vector<Mesh*> _mesh_collision_resolving_helper;

    bool _IsMeshInUZone(Mesh*);
    bool _CheckCollisions(Mesh*);


public:

    PhysicsEngine();
    ~PhysicsEngine();


    void SetUpdateZone(int,int,int,int);
    void SetUpdateZone(int4);

    bool RegisterMesh(Mesh*);
    bool UnregisterMesh(Mesh*);
    Mesh* CreateMesh();

    void Update(int);
    void ManageCollisions();

};

}//namespace

#endif // MAP_PHYSICS_H_INCLUDED
