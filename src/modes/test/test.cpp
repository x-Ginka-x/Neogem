#include "test.h"
#include "../../script.h"

using namespace std;
using namespace neo;

namespace neo{

bool TEST_DEBUG = true;


TestMode::TestMode(){


}//constructor

TestMode::~TestMode(){
    if(TEST_DEBUG) LOG(".TEST_MODE_DEBUG : destroy test mode");


}//destructor



void TestMode::Draw(){




}//Draw()



void TestMode::Update(){

    if(Input->EscapePress()) Mode->Pop();


}//Update()

void TestMode::Reset(){


    if(TEST_DEBUG) LOG(".TEST_MODE_DEBUG : TestMode Initialization finished clearly");

}//Reset()




}//namespace neo
