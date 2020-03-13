#ifndef BROWSE_BUTTON_H
#define BROWSE_BUTTON_H
#include<QPushButton>
#include<QEvent>

class browse_button :public QPushButton
{
Q_OBJECT;
public:
    browse_button(QWidget *parent = 0);
    ~browse_button();
public:
    void enterEvent(QEvent *e); //鼠标进入事件
    void leaveEvent(QEvent *e);//鼠标离开事件
};


#endif // BROWSE_BUTTON_H
