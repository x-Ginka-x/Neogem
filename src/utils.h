#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <map>
#include <vector>

#include <SDL.h>

namespace neo {

enum SDL_FLIP_FLAG{

    SDL_FLIP_VERTICAL,
    SDL_FLIP_HORIZONTAL,
    SDL_FLIP_TOTAL
};

extern bool UTILS_DEBUG;


void ERR(std::string);
void LOG(std::string);


class int2{

public:
    int2(){a = 0; b = 0;}
    int2(int one,int two){a = one; b = two;}
    int2(SDL_Rect rect){a = rect.x; b = rect.y;}
    ~int2(){}
    int a;
    int b;

    int2 operator+(int2 &other){int2 result = int2(a+other.a, b+other.b); return result;}
};

class int3{

public:
    int3(){a = 0; b = 0; c = 0;}
    int3(int one, int two, int three){a = one; b = two; c = three;}
    ~int3(){}
    int a;
    int b;
    int c;
};

class int4{

public:
    int4(){a = 0; b = 0; c = 0; d = 0;}
    int4(int one, int two, int three,int four){a = one; b = two; c = three; d = four;}
    ~int4(){}
    int a;
    int b;
    int c;
    int d;
};


class coor2i{


public:
    coor2i(){x = 0; y = 0;}
    coor2i(int one,int two){x = one; y = two;}
    coor2i(int2 pos){x = pos.a; y = pos.b;}
    coor2i(SDL_Rect rect){x = rect.x; y = rect.y;}
    ~coor2i(){}
    int x;
    int y;

    coor2i operator+(coor2i &other){coor2i result = coor2i(x+other.x, y+other.y); return result;}
};

class coor3f{

public:
    coor3f(){x = 0; y = 0; z = 0;}
    coor3f(float one, float two, float three){x = one; y = two; z = three;}
    ~coor3f(){}
    float x;
    float y;
    float z;
    void placeat(float a, float b, float c){x = a; y = b; z = c;}
};

class coor4f{

public:
    coor4f(){x = 0; y = 0; w = 0; h = 0;}
    coor4f(float one, float two, float three,float four){x = one; y = two; w = three; h = four;}
    ~coor4f(){}
    float x;
    float y;
    float w;
    float h;
};

class coor4i{

public:
    coor4i(){x = 0; y = 0; w = 0; h = 0;}
    coor4i(int one, int two, int three,int four){x = one; y = two; w = three; h = four;}
    ~coor4i(){}
    int x;
    int y;
    int w;
    int h;
};

class d_int{

public:
    d_int(){}
    d_int(float X, float Y, float Z, float W, float H, float D){x=X;y=Y;z=Z;w=W;h=H;d=D;}
    ~d_int(){}

    float x, y, z;
    float w, h, d;

    bool operator <(d_int const& other) const;
};

template <typename T> std::string to_string(const T& n){

    std::ostringstream stm ;
    stm << n ;
    return stm.str() ;
}

template <typename B,class A> void Clear(std::map<B, A*> _map){

    for(auto it = _map.begin(); it != _map.end(); ++it){
        delete it->second;
    }
}

template <class A> void Clear(std::vector<A*> ptr_vec){

    for(auto it = ptr_vec.begin(); it != ptr_vec.end(); ++it){
        delete *it;
    }
}


float sqart(float, float root = 2.0);
Uint32 SDL_get_pixel32( SDL_Surface *surface, int x, int y );
void SDL_put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel );
SDL_Surface* SDL_flip_surface( SDL_Surface *surface, SDL_FLIP_FLAG flag);

}//namespace neo
#endif // UTILS_H_INCLUDED
