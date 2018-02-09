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
#include "Shader.h"

/***TODO LIST :

.Image::Load functions - reorganize and simplify 'loadscript'
.TextManager - add support for texture batching
.Shaders - reorganize and translate remaining french expressions
.Input - simplify and enhance
.Image manager - add a _loaded_surfaces map and a _CollectGarbage() to clean it
.Map - _current_map should be a static attribute of MapMode
.Overall - Access to different attributes (in Entity for example) is too easy
.Mesh - Speed = bugs. Add support for when fast objects go through other meshes
.ActorEntity - change ActorEntity::Update()
.Script - Maybe create different types of ScriptManager (MapScript for example) and place them with their respective gamemodes (modes/map/map_script)
        - Add one ScriptManager per MapMode and a ScriptEngine that acts on _current_script_manager (otherwise there are conflicts in ScriptEngine::_memory
***/

/**TODO NOW :

    Fri 0210 - Empty TODO LIST
    Sat 0211 -> Sun 0212 - Add content to test map
    Mon 0213 -> Sun 0219 - Dialog Manager
    Sun 0219 - 0.11-alpha.0 release


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
