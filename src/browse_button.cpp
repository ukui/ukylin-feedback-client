#include"browse_button.h"

browse_button::browse_button(QWidget* parent) :QPushButton(parent)
{

}
browse_button::~browse_button()
{

}
void browse_button::enterEvent(QEvent *e)
{
    setStyleSheet("font: 14pt 'Sans Serif';background-color:rgb(107,142,235);color: rgb(68, 68, 68)");
}
void browse_button::leaveEvent(QEvent *e)
{
    setStyleSheet("font: 14pt 'Sans Serif';background-color:rgb(233,233,233);color: rgb(68, 68, 68)");
}
