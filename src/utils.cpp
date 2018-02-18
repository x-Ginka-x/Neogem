
#include "utils.h"

namespace neo{

bool UTILS_DEBUG = true;

void ERR(std::string message){

    std::cout << "[ERROR] > " << message << "\n";
}

void LOG(std::string message){

    std::cout << message << "\n";
}

float sqart(float A, float root) {
    const double e = 2.71828182846;
    return pow(e,(std::pow(10.0,9.0)/root)*(1.0-(std::pow(A,-std::pow(10.0,-9.0)))));
}

bool d_int::operator <(d_int const& other) const{
    bool isabove = false;
    bool centerisbehind = false;
    bool wholeobjectisbehind = false;
    bool baseisabovewholeobject = false;

    if(x+z+w+d <= other.x + other.z)
        wholeobjectisbehind = true;
    if(x+z+(w+d)/2.0f <= other.x+other.z+(other.w+other.d)/2.0f)
        centerisbehind = true;
    if(y >= other.y + other.h)
        baseisabovewholeobject = true;
    if(y + h > other.y+1)
        isabove = true;
    bool finalb=true;
    if(centerisbehind && !baseisabovewholeobject)
        finalb = true;

    else if(centerisbehind && baseisabovewholeobject && !wholeobjectisbehind)
        finalb = false;

    else if(!centerisbehind && isabove)
        finalb = false;

    else if(!centerisbehind && !isabove)
        finalb = true;
    return finalb;
}



Uint32 SDL_get_pixel32( SDL_Surface *surface, int x, int y )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Get the requested pixel
    return pixels[ ( y * surface->w ) + x ];
}

void SDL_put_pixel32( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

SDL_Surface* SDL_flip_surface( SDL_Surface *surface, SDL_FLIP_FLAG flag)
{
    //Pointer to the soon to be flipped surface
    SDL_Surface *flipped = NULL;

    //If the image is color keyed
//    if( surface->flags & SDL_SRCCOLORKEY )
//    {
        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, 0 );
//    }
    //Otherwise
//    else
//    {
//        flipped = SDL_CreateRGBSurface( SDL_SWSURFACE, surface->w, surface->h, surface->format->BitsPerPixel, surface->format->Rmask, surface->format->Gmask, surface->format->Bmask, surface->format->Amask );
//    }
    //If the surface must be locked
    if( SDL_MUSTLOCK( surface ) )
    {
        //Lock the surface
        SDL_LockSurface( surface );
    }
    //Go through columns
    for( int x = 0, rx = flipped->w - 1; x < flipped->w; x++, rx-- )
    {
        //Go through rows
        for( int y = 0, ry = flipped->h - 1; y < flipped->h; y++, ry-- )
        {
            //Get pixel
            Uint32 pixel = SDL_get_pixel32( surface, x, y );

            //Copy pixel
            if( flag == SDL_FLIP_TOTAL )
            {
                SDL_put_pixel32( flipped, rx, ry, pixel );
            }
            else if( flag == SDL_FLIP_HORIZONTAL )
            {
                SDL_put_pixel32( flipped, rx, y, pixel );
            }
            else if( flag == SDL_FLIP_VERTICAL )
            {
                SDL_put_pixel32( flipped, x, ry, pixel );
            }
        }
    }
    //Unlock surface
    if( SDL_MUSTLOCK( surface ) )
    {
        SDL_UnlockSurface( surface );
    }

//        SDL_SetColorKey( flipped, SDL_TRUE, surface->format-> );


    //Return flipped surface
    return flipped;
}

}//namespace neo
