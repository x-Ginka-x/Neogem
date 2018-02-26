#ifndef MAP_DIALOG_H_INCLUDED
#define MAP_DIALOG_H_INCLUDED

#include <string>
#include <queue>

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

    void Draw(int,int,int);
    void Update();

    bool FinishedDrawing();
    int GetHeight(){return _height;}

    void AddChoice(std::string choice){_choices.push_back(choice);}
    unsigned int GetChoice();
    void SetChoice(unsigned int val){_choice_id = val;}
    void IncrementChoice();
    void DecrementChoice();

private:

    int _speed;
    int _width, _height;
    int _twidth, _theight;
    Image* _bubble;
    TextBox _textbox;
    std::string _text;
    unsigned int _choice_id;
    std::vector<std::string> _choices;

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

    bool IsPlaying(){return _is_playing;}

    int AddDialog(std::string,int choice_nb = 0, ...);
    int AddDialog(std::string, int choice_nb, va_list);
    void AddChoice(std::string);
    int GetChoiceResult(int);

    void CheckControls();
private:

    std::map<int, DialogBox*> _dialog_index;
    Image* _bubble;
    int _dialog_id_buffer;
    int _current_dialog_id;
    std::string _target;
    std::queue<std::pair<int,std::string> > _dialog_queue;

    bool _is_playing;
};

}//namespace

#endif // MAP_DIALOG_H_INCLUDED
