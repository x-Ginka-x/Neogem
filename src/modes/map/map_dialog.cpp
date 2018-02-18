#include "map_dialog.h"
#include "../../engines/text.h"
#include "map.h"

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

}

void DialogBox::Draw(){

    _bubble->SetDisplaySize(_width, _height);
    _bubble->Draw();
    Video->TranslateCursor(0.0, 0.0, 1.0);
    _textbox.Draw();
}



DialogManager::DialogManager(){

    _dialog_id = 0;
    _current_dialog_id = 0;
    _dialog_index.insert(make_pair(_dialog_id, nullptr));
    _bubble = ImgManager->GetImage("bubble");
}


DialogManager::~DialogManager(){

    Clear(_dialog_index);

}

int DialogManager::AddDialog(std::string text){

    _dialog_id ++;
    DialogBox* db = new DialogBox(text, _bubble);
    _dialog_index.insert(make_pair(_dialog_id, db));

    return _dialog_id;
}

void DialogManager::Play(int dialog_id, std::string target){

    _current_dialog_id = dialog_id;
    _target = target;
}

void DialogManager::Draw(){

    if(_current_dialog_id == 0)
        return;
    else{
        coor3f pos = MapMode::_current_map->GetViewManager()->tocamera(MapMode::_current_map->GetObjectEntity(_target)->GetCenter());
        pos.y -= 50.5;
        int height = _dialog_index[_current_dialog_id]->GetHeight();
        pos.y -= height/2;
        Video->SetCursorPos(pos.x,pos.y,995.0f);

        _dialog_index[_current_dialog_id]->Draw();
    }
}
