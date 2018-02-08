#include <cstdlib>
#include <iostream>

#include "engines/time.h"
#include "engines/input.h"
#include "engines/video.h"
#include "engines/image.h"
#include "engines/text.h"
#include "engines/mode.h"
    #include "modes/map/map.h"
    #include "modes/test/test.h"

#include "script.h"
#include "Shader.h"/** TODO : Reorganize Shader.h and place it in an other directory **/

#include "kaguya/kaguya.hpp"
/***TODO LIST :

.Image::Load functions - reorganize and simplify 'loadscript'
.TextManager - add support for texture batching
.Shaders - reorganize and translate remaining french expressions
.Input - simplify and enhance
.Image manager - add a _loaded_surfaces map and a _CollectGarbage() to clean it
.Script - Add security checking everywhere in script.cpp
.Map - _current_map should be a static attribute of MapMode
.Overall - Access to different attributes (in Entity for example) is too easy
.Mesh - Add a Mesh class and rename Mesh MeshData (reduce memory usage -> MeshDatas are now like 3D models and Meshes hold only position and velocity)
.ActorEntity - change ActorEntity::Update()
.Entity - Maybe initialize the MapTexture and the Mesh in the Entity constructor, so that it never points to NULL and some attributes (like _is_static) are already set
.Script - Maybe create different types of ScriptManager (MapScript for example) and place them with their respective gamemodes (modes/map/map_script)
        - Add one ScriptManager per MapMode and a ScriptEngine that acts on _current_script_manager (otherwise there are conflicts in ScriptEngine::_memory
.Event - Possible memory leak ? ~EventManager() calls MapEvent destructor on MapEvent child classes
***/

/**TODO NOW :

    Events and Entities - Link to ScriptEngine
                        - Recreate test map
**/

using namespace std;


void InitializeEngines(){

    neo::Time = new neo::TimeEngine();
    neo::Time->InitializeTimers();
    neo::Input = new neo::InputEngine();
    neo::Video = new neo::VideoEngine();
    neo::Video->Initialize();
    neo::ImgManager = new neo::ImageManager();
    neo::Text = new neo::TextEngine();
    neo::Mode = new neo::ModeManager();
    neo::Script = new neo::ScriptManager();

}//void InitializeEngines()

void QuitNeogem(){

    int h = neo::Time->GetPlayHours();
    int m = neo::Time->GetPlayMinutes();
    int s = neo::Time->GetPlaySeconds();

    delete neo::Script;
    delete neo::Mode;
    delete neo::Text;
    delete neo::ImgManager;
    delete neo::Video;
    delete neo::Input;
    delete neo::Time;



    cout  << "\n \nProcessed in : " << h << "h " << m << "m " << s << "s. \n";

    neo::LOG("Exited clearly");
    neo::LOG("Good Bye ! :)");


}//void QuitNeogem()




int main(int argc, char **argv)
{

    atexit(QuitNeogem);
    InitializeEngines();

    neo::MapMode* mapmode = new neo::MapMode();
    neo::Mode->Push(mapmode);


    // Main loop

    while(neo::Time->NotDone()){


        neo::Video->Clear();

        neo::Mode->Draw();

        neo::Video->Display();


        neo::Input->Update();

        neo::Mode->Update();

        neo::Time->UpdateTimers();

    }
    // On quitte la SDL


    return 0;
}
