#ifndef CHOICEBOX_H_INCLUDED
#define CHOICEBOX_H_INCLUDED

namespace neo{

class ChoiceBox{

public:
    ChoiceBox();
    ~ChoiceBox();

    void Draw();

    void AddElement(std::string name, int value);

    void IncrementChoice();
    void DecrementChoice();
    void SetChoice(int);
    int GetChoice();

    void SetDrawCursor(bool);
    bool GetDrawCursor();
    void SetPitch(int);
    int GetPitch();

private:

    bool _horizontal; //If yes we write the choices horizontally
    int _pitch_size; //How many pixels separate each choice
    bool _draw_cursor;
    std::map<int, std::string> _choices;
    Font* _font;

};//ChoiceBox

}//namespace
#endif // CHOICEBOX_H_INCLUDED
