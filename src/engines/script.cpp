#include "script.h"

#include <iostream>
#include <fstream>


using namespace neo;

bool neo::SCRIPT_DEBUG = true;
ScriptObject* neo::Script = NULL;


void neo::GlobalDescriptor(ScriptObject* Script){

    std::string instruction = s_text;
    ERR(instruction);
    if(instruction == "global"){
        Script->setendvar(false);
    }
    else if(instruction == "end"){
        Script->setendvar(true);
    }
    else if(instruction == ""){

    }
    else{
        s_par(instruction);
    }
}


ScriptObject::ScriptObject(){
    _cursor = 0;
    BindParadigm("global", (paradigmfunction)&neo::GlobalDescriptor);
}

ScriptObject::~ScriptObject(){

}

void ScriptObject::readfile(std::string name){

    std::ifstream t(name,std::ios::in);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    _file = buffer;
}

std::string ScriptObject::readtext(){

    std::string str;
    size_t newcursor = _file.find_first_of(" \n", _cursor);

    if(_cursor != _file.npos){

        str = _file.substr(_cursor, newcursor-_cursor);
        _cursor = newcursor + 1;
        if(SCRIPT_DEBUG) LOG(".SCRIPT_DEBUG : reading " + str);
        return str;
    }
    return "";
}

int ScriptObject::readint(){


    int i;
    std::string str = readtext();

    i = atoi(str.c_str());
    return i;
}

float ScriptObject::readfloat(){

    float f;
    std::string str = readtext();

    f = atof(str.c_str());
    return f;
}

DIRECTION ScriptObject::readdirection(){

    std::string direction;
    DIRECTION dir = SOUTH;

    if(direction == "NORTH") dir = NORTH;
    else if(direction == "NORTH_EAST") dir = NORTH_EAST;
    else if(direction == "EAST") dir = EAST;
    else if(direction == "SOUTH_EAST") dir = SOUTH_EAST;
    else if(direction == "SOUTH") dir = SOUTH;
    else if(direction == "SOUTH_WEST") dir = SOUTH_WEST;
    else if(direction == "WEST") dir = WEST;
    else if(direction == "NORTH_WEST") dir = NORTH_WEST;
    return dir;
}

//bool ScriptObject::readspecialchar(const char* value){
//
//    const char* str = (_file.substr(_cursor, 2)).c_str();
//    std::cout << str << std::endl;
//    if(str == value)
//        return true;
//    else
//        return false;
//}

void ScriptObject::_jumpline(){

    size_t newcursor = _file.find("\n", _cursor);
    _cursor = newcursor + 1;
}

void ScriptObject::_executescript(std::string name){

    readfile(name);
    _paradigm = "global";
    _reached_end_of_file = false;
    ERR(name);
    ERR(_file);
    _cursor = 0;

    while(_reached_end_of_file == false){

        _executeline();
    }
    LOG("End of file");
}

void ScriptObject::_executeline(){

    if(_paradigms.find(_paradigm) != _paradigms.end())
        _paradigms[_paradigm](this);
}


void ScriptObject::BindParadigm(std::string name, paradigmfunction paradigm){

      _paradigms.insert(std::make_pair(name, paradigm));

}
