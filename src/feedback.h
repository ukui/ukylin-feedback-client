#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QMainWindow>

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <QProcess>
#include <fstream>
#include <iostream>
#include <cstring>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class feedback; }
QT_END_NAMESPACE

class feedback : public QMainWindow
{
    Q_OBJECT

public:
    feedback(QWidget *parent = nullptr);
    ~feedback();

    void systeminfo_show();
    void systeminfo_hide();
    void add_systeminfo();
    void hide_add_file();
    void show_add_file();
    void set_filename_to_label();
    void set_filesize_to_label();
    void add_del_file_button();
    void add_file_change_window();

signals:
    void syslog();

private slots:
    void on_pushButton_clicked();


    void on_textEdit_textChanged();



    void on_checkBox_stateChanged(int state);

    void on_checkBox_2_stateChanged(int state);

    void on_checkBox_3_stateChanged(int state);

    void on_checkBox_4_stateChanged(int state);

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_textEdit_2_textChanged();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::feedback *ui;

//logflag
    int syslogflag = 0;
    int apportlogflag = 0;
    int dpkglogflag = 0;
    int describeflag = 0;
    int emailflag = 0;
    int get_systeminfoflag = 0;

    QString feedback_type = "系统问题";  //反馈类型
    QString email_str;   //详细描述
    QString filename;

    QString all_systeminfo;

    //控件坐标变量
    //window size
    int window_w = 600;
    int window_h =750;
    //checkBox_4
    int checkbox4_x = 140;
    int checkbox4_y = 560;
    int checkbox4_w = 121;
    int checkbox4_h = 24;
    //pushButton_3
    int pushbutton3_x = 260;
    int pushbutton3_y = 560;
    int pushbutton3_w = 71;
    int pushbutton3_h = 24;
    //verticalWidget
    int widget_x = 230;
    int widget_y = 600;
    int widget_w = 181;
    int widget_h = 80;
    //pushButton_2
    int pushbutton2_x = 440;
    int pushbutton2_y = 630;
    int pushbutton2_w = 120;
    int pushbutton2_h = 45;

    //文件名字宽度
    int filename_width;
    int filesize_width;
    int filename_x = 140;
    int filename_y = 500;
    int filename_w = 200;
    int filename_h = 24;
    //添加文件个数
    int add_file_num = 0;


};
#endif // FEEDBACK_H
