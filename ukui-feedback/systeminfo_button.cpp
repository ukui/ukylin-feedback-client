#include"systeminfo_button.h"
#include "feedback.h"


systeminfo_button::systeminfo_button(QWidget* parent) :QPushButton(parent)
{

}
systeminfo_button::~systeminfo_button()
{

}
void systeminfo_button::enterEvent(QEvent *e)
{
    pfeedback->systeminfo_show();
}
void systeminfo_button::leaveEvent(QEvent *e)
{
    pfeedback->systeminfo_hide();
}

void systeminfo_button::setFeedBack(feedback *p)
{
    pfeedback = p;
}


