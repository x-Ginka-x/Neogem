#include "map_mesh.h"


using namespace std;
using namespace neo;

bool neo::MESH_DEBUG = true;



///*** Mesh functions ***/

float Mesh::_jouletospeed(float value, float mass){

    return sqart(2*value/mass) * 16.0f;
}

float Mesh::_speedtojoule(float value,float mass){

    return (mass/2*value*value/256);
}
//
//
Mesh::Mesh(){
    if(MESH_DEBUG) LOG(".MESH_DEBUG : create and initialize mesh object");

    _is_moving = false;
    _position_changed = false;
    _is_solid = true;
    _on_ground = false;
    _is_static = false;
    _mass = 1.0f;
    _resistance = 500.0f;
    _bounce = 0;

    _blocked_directions.insert(make_pair(UP, false));
    _blocked_directions.insert(make_pair(DOWN, false));
    _blocked_directions.insert(make_pair(EAST, false));
    _blocked_directions.insert(make_pair(WEST, false));
    _blocked_directions.insert(make_pair(NORTH, false));
    _blocked_directions.insert(make_pair(SOUTH, false));

    _kinetic_energy.insert(make_pair(UP, 0.0f));
    _kinetic_energy.insert(make_pair(DOWN, 0.0f));
    _kinetic_energy.insert(make_pair(EAST, 0.0f));
    _kinetic_energy.insert(make_pair(WEST, 0.0f));
    _kinetic_energy.insert(make_pair(NORTH, 0.0f));
    _kinetic_energy.insert(make_pair(SOUTH, 0.0f));

    _is_initialized = true;
    _pos = coor3f(0,0,0);
    _size = coor3f(0,0,0);

}//Constructor
//
//
Mesh::~Mesh(){
    if(MESH_DEBUG) LOG(".MESH_DEBUG : destroy mesh object");


}//Destructor
//
coor3f Mesh::GetPos(){

    return coor3f(_pos.x, _pos.y, _pos.z);
}
//
//
void Mesh::SetPos(float x, float y, float z){

    _pos.x = x;
    _pos.y = y;
    _pos.z = z;
    _is_moving = true;
    _CalculatePosition();
}//SetPos
//
//
void Mesh::SetSize(float x, float y, float z){

    _size.x = x;
    _size.y = y;
    _size.z = z;
    _CalculatePosition();
}//SetSize
//
//
void Mesh::Move(DIRECTION dir, float speed){
    //cout << "ENTITY MOVING" << endl;

    switch(dir){
    case NORTH:
        _pos.z -= speed;
        break;
    case NORTH_EAST:
        _pos.x += speed;
        _pos.z -= speed;
        break;
    case EAST:
        _pos.x += speed;
        break;
    case SOUTH_EAST:
        _pos.x += speed;
        _pos.z += speed;
        break;
    case SOUTH:
        _pos.z += speed;
        break;
    case SOUTH_WEST:
        _pos.x -= speed;
        _pos.z += speed;
        break;
    case WEST:
        _pos.x -= speed;
        break;
    case NORTH_WEST:
        _pos.x -= speed;
        _pos.z -= speed;
        break;
    case UP:
        _pos.y += speed;
        break;
    case DOWN:
        _pos.y -= speed;
        break;
    default:
        break;
    }
    _is_moving = true;
}//Move()
//
//
void Mesh::_CalculatePosition(){

    float x = _pos.x;
    float y = _pos.y;
    float z = _pos.z;
    float w = _size.x;
    float h = _size.y;
    float d = _size.z;

    _left = x;
    _right = x + w;
    _bottom = y;
    _top = y + h;
    _front = z + d;
    _back = z;

    _hot_left.placeat(x, y + h/2, z + d/2);
    _hot_right.placeat(x + w, y + h/2, z + d/2);
    _hot_top.placeat(x + w/2, y + h, z + d/2);
    _hot_bottom.placeat(x + w/2, y, z + d/2);
    _hot_front.placeat(x + w/2, y + h/2, z + d);
    _hot_back.placeat(x + w/2, y + h/2, z);
    _anchor.placeat(x + w, y, z + d);
    _center.placeat(x + w/2, y + h/2, z + d/2);

}//_CalculatePosition


void Mesh::Update(int update_time){

    /** Start with 0 collision **/

    _colliding_meshes.clear();

    /** Hold the time passed in seconds **/

    float seconds_passed = 0.001f * update_time;

    /** Update only if object is initialized **/

    if(_is_initialized == false) return;
    _position_changed = false;

    /** If mesh is static, no way to move **/

    if(_is_static == false){

        SetBlocked(NORTH, false);
        SetBlocked(SOUTH, false);
        SetBlocked(EAST, false);
        SetBlocked(WEST, false);
        SetBlocked(UP, false);
        SetBlocked(DOWN, false);
    }

    /** else all ways are possible **/

    else{
        SetBlocked(NORTH, true);
        SetBlocked(SOUTH, true);
        SetBlocked(EAST, true);
        SetBlocked(WEST, true);
        SetBlocked(UP, true);
        SetBlocked(DOWN, true);
    }

    _on_ground = false;


    /** Equilibrate the energies so that there's only one way for each direction **/

    if(_kinetic_energy.at(EAST) > _kinetic_energy.at(WEST) ){
        _kinetic_energy.at(EAST) -= _kinetic_energy.at(WEST);
        _kinetic_energy.at(WEST) = 0.0f;
    }
    else{
        _kinetic_energy.at(WEST) -= _kinetic_energy.at(EAST);
        _kinetic_energy.at(EAST) = 0.0f;
    }
    if(_kinetic_energy.at(NORTH) > _kinetic_energy.at(SOUTH) ){
        _kinetic_energy.at(NORTH) -= _kinetic_energy.at(SOUTH);
        _kinetic_energy.at(SOUTH) = 0.0f;
    }
    else{
        _kinetic_energy.at(SOUTH) -= _kinetic_energy.at(NORTH);
        _kinetic_energy.at(NORTH) = 0.0f;
    }
    if(_kinetic_energy.at(UP) > _kinetic_energy.at(DOWN) ){
        _kinetic_energy.at(UP) -= _kinetic_energy.at(DOWN);
        _kinetic_energy.at(DOWN) = 0.0f;
    }
    else{
        _kinetic_energy.at(DOWN) -= _kinetic_energy.at(UP);
        _kinetic_energy.at(UP) = 0.0f;
    }

    /** If energies are very small no need to care **/

    if(_kinetic_energy.at(DOWN) < 0.01f)
        _kinetic_energy.at(DOWN) = 0.0f;
    if(_kinetic_energy.at(UP) < 0.01f)
        _kinetic_energy.at(UP) = 0.0f;
    if(_kinetic_energy.at(EAST) < 0.01f)
        _kinetic_energy.at(EAST) = 0.0f;
    if(_kinetic_energy.at(WEST) < 0.01f)
        _kinetic_energy.at(WEST) = 0.0f;
    if(_kinetic_energy.at(NORTH) < 0.01f)
        _kinetic_energy.at(NORTH) = 0.0f;
    if(_kinetic_energy.at(SOUTH) < 0.01f)
        _kinetic_energy.at(SOUTH) = 0.0f;


    /** If there's no energy, mesh is not moving... **/


    if((_kinetic_energy.at(UP) +
       _kinetic_energy.at(DOWN) +
       _kinetic_energy.at(EAST) +
       _kinetic_energy.at(WEST) +
       _kinetic_energy.at(NORTH) +
       _kinetic_energy.at(SOUTH)) == 0.0f){

        _is_moving = false;
        _on_ground = true;
    }

    /** ...else move the mesh **/

    else{
        _is_moving = true;
    }



    /** This, below, is a very bad but important fix that cancels infinitesimal bouncing **/
//
//    float mv = seconds_passed * _jouletospeed(_kinetic_energy.at(UP), _mass);
//    if (mv < 1.0f) _kinetic_energy.at(UP) = 0; //Temp fix for bouncing issue

    /** ---------------- **/


    if(_is_moving){

        float v = _jouletospeed(_kinetic_energy.at(WEST), _mass);
        float y = v - (_resistance * seconds_passed);
        if (y < 0) y = 0;
        float dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(WEST, dist);
        _kinetic_energy.at(WEST) = _speedtojoule(y, _mass);


        v = _jouletospeed(_kinetic_energy.at(EAST), _mass);
        y = v - (_resistance * seconds_passed);
        if (y < 0) y = 0;
        dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(EAST, dist);
        _kinetic_energy.at(EAST) = _speedtojoule(y, _mass);



        v = _jouletospeed(_kinetic_energy.at(NORTH), _mass);
        y = v - (_resistance * seconds_passed);
        if (y < 0) y = 0;
        dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(NORTH, dist);
        _kinetic_energy.at(NORTH) = _speedtojoule(y, _mass);



        v = _jouletospeed(_kinetic_energy.at(SOUTH), _mass);
        y = v - (_resistance * seconds_passed);
        if (y < 0) y = 0;
        dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(SOUTH, dist);
        _kinetic_energy.at(SOUTH) = _speedtojoule(y, _mass);



        v = _jouletospeed(_kinetic_energy.at(DOWN), _mass);
        y = v - (_resistance * seconds_passed);
        if (y < 0) y = 0;
        dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(DOWN, dist);
        _kinetic_energy.at(DOWN) = _speedtojoule(y, _mass);


        v = _jouletospeed(_kinetic_energy.at(UP), _mass);
        y = v - (300.0f * seconds_passed);
        if (y < 0) y = 0;
        dist = (seconds_passed * y) + ((v - y) * seconds_passed / 2);
        Move(UP, dist);
        _kinetic_energy.at(UP) = _speedtojoule(y, _mass);
//
//        /** Convert energy to speed in pixels then multiply by time passed **/
//
//        Move(NORTH, seconds_passed * _jouletospeed(_kinetic_energy.at(NORTH), _mass));
//        Move(SOUTH, seconds_passed * _jouletospeed(_kinetic_energy.at(SOUTH), _mass));
//        Move(EAST, seconds_passed * _jouletospeed(_kinetic_energy.at(EAST), _mass));
//
//        Move(UP, seconds_passed * _jouletospeed(_kinetic_energy.at(UP), _mass));
//        Move(DOWN, seconds_passed * _jouletospeed(_kinetic_energy.at(DOWN), _mass));


        /** Energies need to be updated by a ratio calculated with time passed and resistance of mesh **/
        //STILL EXPERIMENTAL



//
//        if(IsBlocked(DOWN) == false){
//            float k = _kinetic_energy.at(DOWN);
//            _kinetic_energy.at(DOWN) -= k*0.5f;
//        }
//        else{
//            _kinetic_energy.at(DOWN) = 0.0f;
//        }
//        if(IsBlocked(UP) == false){
////            k = _kinetic_energy.at(UP)*0.1f;
////            _kinetic_energy.at(UP) -= k;
//        }
//        else{
//            _kinetic_energy.at(UP) = 0.0f;
//        }


        /** Update all the variables attached to _pos **/

        _CalculatePosition();

    }


}//Update



bool Mesh::IsColliding(Mesh* E){

    if(_left > E->_right
    || _right < E->_left
    || _bottom > E->_top
    || _top < E->_bottom
    || _back > E->_front
    || _front < E->_back){
    return false;
    }

    else{
        _colliding_meshes.push_back(E);
        return true;
    }
}
//
bool Mesh::IsColliding(coor3f &P){
    int x = P.x;
    int y = P.y;
    int z = P.z;
    if(x > _right
    || x < _left
    || y > _top
    || y < _bottom
    || z > _front
    || z < _back)
        return false;

    else return true;

}

///*** New version ***/
///** TODO : This **/

void Mesh::ResolveCollision(Mesh* mesh){

    if(mesh->IsColliding(_hot_bottom)){
        _on_ground = true;
        _pos.y = mesh->_pos.y+mesh->_size.y;
        if(mesh->IsBlocked(DOWN)){
            SetBlocked(DOWN,true);
        }

        float k = _kinetic_energy.at(DOWN);
        mesh->ApplyForce(DOWN, ENERGY_TYPE_JOULE, k/2);
        ApplyForce(UP, ENERGY_TYPE_JOULE, _bounce*k/2);
        _kinetic_energy.at(DOWN) -= k;
    }

    else if(mesh->IsColliding(_hot_left)){
        Move(EAST, mesh->_right - _left);

        if(mesh->IsBlocked(WEST))
            SetBlocked(WEST, true);

        float k = _kinetic_energy.at(WEST);
        mesh->ApplyForce(WEST, ENERGY_TYPE_JOULE, k/2, true);
        ApplyForce(EAST, ENERGY_TYPE_JOULE, k/2, true);
        _kinetic_energy.at(WEST) -= k;
    }

    else if(mesh->IsColliding(_hot_right)){
        Move(WEST, _right - mesh->_left);

        if(mesh->IsBlocked(EAST))
            SetBlocked(EAST, true);

        float k = _kinetic_energy.at(EAST);
        mesh->ApplyForce(EAST, ENERGY_TYPE_JOULE, k/2, true);
        ApplyForce(WEST, ENERGY_TYPE_JOULE, k/2, true);
        _kinetic_energy.at(EAST) -= k;
    }


    else if(mesh->IsColliding(_hot_top)){
        Move(DOWN, _top - mesh->_bottom);

        if(mesh->IsBlocked(UP))
            SetBlocked(UP, true);

        float k = _kinetic_energy.at(UP);
        mesh->ApplyForce(UP, ENERGY_TYPE_JOULE, k/2);
        ApplyForce(DOWN, ENERGY_TYPE_JOULE, k/2);
        _kinetic_energy.at(UP) -= k;
    }


    else if(mesh->IsColliding(_hot_front)){
        Move(NORTH, _front - mesh->_back);

        if(mesh->IsBlocked(SOUTH))
            SetBlocked(SOUTH, true);

        float k = _kinetic_energy.at(SOUTH);
        mesh->ApplyForce(SOUTH, ENERGY_TYPE_JOULE, k/2, true);
        ApplyForce(NORTH, ENERGY_TYPE_JOULE, k/2, true);
        _kinetic_energy.at(SOUTH) -= k;
    }


    else if(mesh->IsColliding(_hot_back)){
        Move(SOUTH, mesh->_front - _back);

        if(mesh->IsBlocked(NORTH))
            SetBlocked(NORTH, true);

        float k = _kinetic_energy.at(NORTH);
        mesh->ApplyForce(NORTH, ENERGY_TYPE_JOULE, k/2, true);
        ApplyForce(SOUTH, ENERGY_TYPE_JOULE, k/2, true);
        _kinetic_energy.at(NORTH) -= k;
    }


//    else cout << "Collision doesn't need a correction\n";

    _CalculatePosition();

}

///*** NEW KINETIC INTERFACE ***/
//
//
void Mesh::ApplyForce(DIRECTION dir, ENERGY_TYPE type, float value, bool is_capped){

    if(IsBlocked(dir))
        return;

    float temp_k = 0;
    float cap = 0;

    if(type == ENERGY_TYPE_METERSECOND){
        temp_k = (_mass * value * value / 2);
        cap = temp_k;
    }
    else if(type == ENERGY_TYPE_JOULE){
        cap = value;
        temp_k = value;
    }

    if(is_capped == false){

        _kinetic_energy.at(dir) += temp_k;
        return;
    }

    else if(is_capped == true){
        if(_kinetic_energy.at(dir) >= cap)
            return;
        else if (_kinetic_energy.at(dir) + temp_k > cap)
            _kinetic_energy.at(dir) = cap;
        else
            _kinetic_energy.at(dir) += temp_k;
    }

}

void Mesh::ResetForces(){

    _kinetic_energy.at(EAST) =
    _kinetic_energy.at(WEST) =
    _kinetic_energy.at(NORTH) =
    _kinetic_energy.at(SOUTH) = 0.0f;

}


