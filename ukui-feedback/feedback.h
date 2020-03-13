#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QMainWindow>
#include <QLabel>

#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <QProcess>
#include <fstream>
#include <iostream>
#include <cstring>
#include <QSqlDatabase>
#include <QSqlQuery>

class QSqlTableModel;
class QPushButton;
class QLabel;

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
    void add_file_change_window();
    void del_file_change_window();

    void add_fileinfo_model();
    void update_add_file_window();
    void update_linedit_add_or_del_file();


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
    void del_file_button_clicked();

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
    int window_h =695;
    //checkBox_4
    int checkbox4_x = 35;
    int checkbox4_y = 570;
    int checkbox4_w = 121;
    int checkbox4_h = 24;
    //pushButton_3
    int pushbutton3_x = 138;
    int pushbutton3_y = 570;
    int pushbutton3_w = 71;
    int pushbutton3_h = 24;
    //verticalWidget
    int widget_x = 100;
    int widget_y = 603;
    int widget_w = 181;
    int widget_h = 80;
    //pushButton_2
    int pushbutton2_x = 440;
    int pushbutton2_y = 600;
    int pushbutton2_w = 120;
    int pushbutton2_h = 45;

    //文件名字宽度
    //int filename_width;
    //int filesize_width;
    int filename_x = 140;
    int filename_y = 480;
    int filename_w = 200;
    int filename_h = 24;


    QSqlTableModel * model;

    QPushButton *deletefileBtn[5];
    QLabel *filename_label[5];
    QLabel *filesize_label[5];

    QString file_name = "sssssssssssssss";


};
#endif // FEEDBACK_H
