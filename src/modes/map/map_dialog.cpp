#include "map_dialog.h"
#include "../../engines/text.h"
#include "map.h"
#include <cstdarg>

using namespace neo;
using namespace std;

DialogBox::DialogBox(std::string text, Image* bubble){

    _text = text;
    int length = Text->CalculateLength(text, Text->GetFont("default_font"));

    _width = length / 3;
    _twidth = 9*_width/10;

    _textbox.SetSize(_twidth, 0);
    _textbox.Load(text, Text->GetFont("default_font"));
    _height = 10*_textbox.GetSize().y/9;
    _bubble = bubble;

}

DialogBox::~DialogBox(){

}

void DialogBox::Update(){

    if(_choice_id < 1 || _choice_id > _choices.size() || _choices.empty())
        _choice_id = 0;

}

void DialogBox::Draw(int x, int y, int z){

    _bubble->SetDisplaySize(_width, _height);
    _bubble->Draw(x,y,z);
    _textbox.Draw(x,y,z+1);


    if(_choices.size() > 0){

        unsigned int i = 0;
        int length = 0;
        std::string text;

        while(i < _choices.size()){

            /** Calculate Length of the word **/
            int i_length = Text->CalculateLength(_choices.at(i) + ">    ", Text->GetFont("default_font"));
            /** if it's greater than previous length, set as new length **/
            length = i_length > length ? i_length : length;

            /** display the arrow if it's the current choice **/
            if(i == _choice_id) text += ">";

            /** display choice text **/
            text += _choices.at(i);


            if(i < _choices.size()-1) text += "\n";
            i++;
        }

        TextBox tb;
        tb.SetSize(length, 0);
        tb.Load(text, Text->GetFont("default_font"));
        _bubble->SetDisplaySize(tb.GetSize().x, tb.GetSize().y);
        int tx = x+(_width/2) + tb.GetSize().x/2;
        int ty = y+(_height/2)-tb.GetSize().y/2;
        _bubble->Draw(tx,ty,z);
        tb.Draw(tx,ty,z+1);
    }
}


unsigned int DialogBox::GetChoice(){

    return _choice_id;
}

void DialogBox::IncrementChoice(){

    if(_choices.empty()){
        return;
    }

    _choice_id ++;
    if(_choice_id >= _choices.size())
        _choice_id = 0;
}

void DialogBox::DecrementChoice(){

    if(_choices.empty()){
        return;
    }
    if(_choice_id == 0)
        _choice_id = _choices.size();
    else
        _choice_id --;
}

DialogManager::DialogManager(){

    _dialog_id_buffer = 0;
    _current_dialog_id = 0;
    _bubble = ImgManager->GetImage("bubble");
}


DialogManager::~DialogManager(){

    Clear(_dialog_index);

}

int DialogManager::AddDialog(std::string text,int choice_nb...){

    _dialog_id_buffer ++;
    DialogBox* db = new DialogBox(text, _bubble);
    _dialog_index.insert(make_pair(_dialog_id_buffer, db));

    va_list args;
    va_start(args, choice_nb);
    for(int i = 0; i < choice_nb; i++){
        db->AddChoice(va_arg(args, const char*));
    }
    va_end(args);

    return _dialog_id_buffer;
}



int DialogManager::AddDialog(std::string text, int choice_nb, va_list args){

    _dialog_id_buffer ++;
    DialogBox* db = new DialogBox(text, _bubble);
    _dialog_index.insert(make_pair(_dialog_id_buffer, db));

    for(int i = 0; i < choice_nb; i++){
        db->AddChoice(va_arg(args, const char*));
    }

    return _dialog_id_buffer;
}


void DialogManager::AddChoice(std::string choice){

    if(_dialog_index.empty())
        return;
    _dialog_index.rbegin()->second->AddChoice(choice);
}

void DialogManager::Play(int dialog_id, std::string target){

    _dialog_queue.push(make_pair(dialog_id, target));
    _dialog_index[dialog_id]->SetChoice(0);
}

void DialogManager::Draw(){

    if(_is_playing){

        coor3f pos = MapMode::_current_map->GetViewManager()->tocamera(MapMode::_current_map->GetObjectEntity(_target)->GetCenter());
        pos.y -= 50.5;
        int height = _dialog_index[_current_dialog_id]->GetHeight();
        pos.y -= height/2;
        _dialog_index[_current_dialog_id]->Draw(pos.x,pos.y,995);
    }
}

void DialogManager::Update(){

    if(_dialog_queue.empty()){
        _is_playing = false;
        return;
    }
    _current_dialog_id = _dialog_queue.front().first;
    _target = _dialog_queue.front().second;

    while(_dialog_index.find(_current_dialog_id) == _dialog_index.end()){

        if(_dialog_queue.empty())
            break;
        _dialog_queue.pop();
        _current_dialog_id = _dialog_queue.front().first;
        _target = _dialog_queue.front().second;
    }

    if(_dialog_queue.empty())
        _is_playing = false;
    else
        //_current_dialog->Update()
        _is_playing = true;

}

void DialogManager::CheckControls(){

    if(Input->Press(SDLK_e)){
        _dialog_queue.pop();
    }
    if(Input->Press(SDLK_DOWN)){
        _dialog_index[_current_dialog_id]->IncrementChoice();
    }
    if(Input->Press(SDLK_UP)){
        _dialog_index[_current_dialog_id]->DecrementChoice();
    }
}

int DialogManager::GetChoiceResult(int choice_id){

    return _dialog_index[choice_id]->GetChoice();
}
