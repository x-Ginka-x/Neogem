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

#include "engines/script.h"


/*** TODO List :

.TextManager - Add color choice when loading or writing glyphs
.TextManager - add support for texture batching
.Image manager - add a _loaded_surfaces map
.Image - add a resizing function
.Overall - Access to different attributes (in Entity for example) is too easy
.ActorEntity - change ActorEntity::Update()
.MapMode - add a GetNearestObject function to decide what active event should be played

.MapMode - Separate controls (dialog + explo)
.DialogManager - Add Stop, improve link to events

***/

/*** TODO in a future major rework

.Mesh - Rework how collisions are resolved (maybe bring back class KineticForceObject)
.MapVideoEngine - Replace multimap by a custom container for Texture sorting


**TODO NOW :

    Mon 0213 -> Sun 0219 - Dialog Manager
    Sun 0219 - 0.2-alpha.0 release

    .Text->write(Textbox)

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
    neo::Script = new neo::ScriptObject();

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
