#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <SDL.h>
#include <vector>

#include "../../defs.h"
#include "../../utils.h"

#include "../../engines/time.h"
#include "../../engines/mode.h"
#include "../../engines/input.h"
#include "../../engines/video.h"
    #include "../../engines/image.h"
    #include "../../engines/text.h"
    #include "../../engines/animation.h"
    #include "../map/map_entity.h"



namespace neo{

extern bool TEST_DEBUG;

class TestMode: public neo::GameMode{

public:

    TestMode();
    ~TestMode();

    void Update();
    void Draw();
    void Reset();

private:



};//class TestMode




}//namespace

#endif // TEST_H_INCLUDED
