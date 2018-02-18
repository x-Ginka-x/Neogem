#ifndef MAP_DIALOG_H_INCLUDED
#define MAP_DIALOG_H_INCLUDED

#include <string>

#include "../../engines/text_box.h"

namespace neo{



/** BRAINSTORM **

** Dialog manager :

** Text is loaded in strings and rearranged to fit boxes (with '\n' insertions)
** This operation is made before the beginning of the map exploration

** When asked to play, DM displays the selected dialog and waits for an input
** If SDLK_e, the following occurs :
**** .if text was too big, continue displaying the text
**** .else return an END signal
** If SDLK_tab, the following occurs :
**** .skip the dialog and return an END signal

** We need TextBoxes :
** Turn a raw string into a resized one to fit selected size

** We need DialogBoxes :
** Represent a rearranged text and a size for displaying the box
** At loading, choose a size of TextBox
** If the text is still too big, link to another TextBox with same size containing the remaining text


** In-situation logic :
map.cpp
    if(input->press(sdlk_e)
        dialog->startdialog(127, normal);
        state = dialog_state;

dialog.cpp
    Update()
        if(dialog_started)
            play(dialog, normal);
    Update(current_dialog);

    Draw()
        if(is_playing)
            draw(current_dialog, current_state;)
**/


class DialogBox{

public:

    DialogBox(std::string, Image*);
    ~DialogBox();

    void Draw();
    void Update();

    bool FinishedDrawing();
    int GetHeight(){return _height;}

private:

    int _speed;
    int _width, _height;
    int _twidth, _theight;
    Image* _bubble;
    TextBox _textbox;
    std::string _text;

};//DialogBox

class DialogManager{

public:

    DialogManager();
    ~DialogManager();

    void Update();
    void Draw();

    void Play(int, std::string);
    void Stop();

    void SetDialogDisplayStyle(int);
    bool DialogEnd();
    Image* GetDialogBubble(){return _bubble;}

    int AddDialog(std::string);

private:

    std::map<int, DialogBox*> _dialog_index;
    Image* _bubble;
    int _dialog_id;
    int _current_dialog_id;
    std::string _target;
};

}//namespace

#endif // MAP_DIALOG_H_INCLUDED