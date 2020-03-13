#ifndef MOUSE_BUTTON_H
#define systeminfo_button_H
#include<QPushButton>
#include<QEvent>

class feedback;

class systeminfo_button :public QPushButton
{
Q_OBJECT;
public:
    systeminfo_button(QWidget *parent = 0);
    ~systeminfo_button();
public:
    void enterEvent(QEvent *e); //鼠标进入事件
    void leaveEvent(QEvent *e);//鼠标离开事件
    void setFeedBack(feedback *p);
    feedback* pfeedback;
};


#endif // systeminfo_button_H
