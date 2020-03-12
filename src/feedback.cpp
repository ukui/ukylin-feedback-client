#include "feedback.h"
#include "ui_feedback.h"

#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>


feedback::feedback(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::feedback)
{
    ui->setupUi(this);
    QString name;
    ui->lineEdit->setReadOnly(true);
    ui->pushButton_3->setFeedBack(this);
    ui->pushButton_3->setFlat(true);    //隐藏按钮边框
    //ui->pushButton_3->setCursor(QCursor(Qt::PointingHandCursor));   //设置鼠标接触按钮变为手形
    ui->verticalWidget->hide();
    ui->verticalWidget->setStyleSheet(QString::fromUtf8("border:0.5px solid black"));

    add_systeminfo();//将系统信息添加到信息框

    ui->pushButton_2->setEnabled(false);
    qDebug()<<frameGeometry().width()<<"bbbbbb";

}

feedback::~feedback()
{
    delete ui;
}
//获取图片
void feedback::on_pushButton_clicked()
{
    ui->pushButton->setStyleSheet("font: 14pt 'Sans Serif';background-color:rgb(65,95,196);color: rgb(68, 68, 68)");
    filename=QFileDialog::getOpenFileName(this,tr("select image"),"/","Image file(*.gif *.jpg *.png)",0);
    if ((filename.isNull()) == false){
        ui->lineEdit->setText(filename);
    }
    qDebug() << filename;
        qDebug()<<frameGeometry().width()<<"kkkkkk";
}


//设置详细描述框最大字符数
void feedback::on_textEdit_textChanged()
{
    describeflag = 1;//详细描述是否填写
    if (emailflag == 1){//邮箱和详细描述都已经填写
        ui->pushButton_2->setEnabled(true);//设置提交按钮属性
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(61,107,229);color: rgb(68, 68, 68)");
    }
    QString textContent = ui->textEdit->toPlainText();
    //qDebug() << textContent;

    int length = textContent.count();

    int maxLength = 200; // 最大字符数

    if(length > maxLength) {
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        if(cursor.hasSelection()) {
            cursor.clearSelection();
        }
        cursor.deletePreviousChar();
        //设置当前的光标为更改后的光标
        ui->textEdit->setTextCursor(cursor);
    }

    length = ui->textEdit->toPlainText().count();
    ui->label_9->setText(QString("%1/%2").arg(200-length).arg(200));//显示剩余字符数和最大字符数

}
//系统信息显示
void feedback::systeminfo_show()
{
    ui->verticalWidget->show();
}
//系统信息隐藏
void feedback::systeminfo_hide()
{
    ui->verticalWidget->hide();
}
//添加系统信息
void feedback::add_systeminfo()
{
    //获取系统信息
    //1.获取系统版本
    string system_info_str;
    string system_name;
    string system_version_id;
    string s;
    ifstream fp("/etc/os-release");
    if(!fp){
        system_info_str = "None";
    }
    else{
        while (getline(fp,s)){
            string::size_type idx;
            idx = s.find("=");//字符串中查找字符串
            if (idx == string::npos){//不存在
            }
            else{
                string str2 = s.substr(0,idx);//截取字符串中=前面的内容
                if(str2 == "NAME"){
                    system_name = s.substr(5);//截取"NAME="后面的内容
                }
                else if(str2 =="VERSION_ID"){
                    system_version_id = s.substr(11);//截取"VERSION_ID="后面的内容
                }
            }
        }
        system_info_str = "system: "+system_name +" " + system_version_id;
    }
    QString system_info = QString::fromStdString(system_info_str);//string 转QString
    system_info.remove(QChar('"'), Qt::CaseInsensitive);  //将字符串中"字符删除
    ui->label_10->setText(system_info);
    //2.获取桌面环境信息
    char * desktop = getenv("DESKTOP_SESSION");
    char desktop_info[128] = "Desktop env: ";
    strcat(desktop_info,desktop);
    ui->label_12->setText(desktop_info);
    //3.获取编码格式
    char * encoding = getenv("LANG");
    char encoding_info[128] = "Desktop lang: ";
    strcat(encoding_info,encoding);
    ui->label_11->setText(encoding_info);

    all_systeminfo.append(system_info);
    all_systeminfo.append("    ");
    all_systeminfo.append(desktop_info);
    all_systeminfo.append("    ");
    all_systeminfo.append(encoding_info);

    qDebug()<<all_systeminfo;
}

//syslog点选
void feedback::on_checkBox_stateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        emit syslog();
        qDebug() <<"选中";
        syslogflag = 1;
    }
    else // 未选中 - Qt::Unchecked
    {
        qDebug() <<"未选中";
        syslogflag = 0;
    }
}
//apport.log点选
void feedback::on_checkBox_2_stateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        emit syslog();
        qDebug() <<"选中";
        apportlogflag = 1;
    }
    else // 未选中 - Qt::Unchecked
    {
        qDebug() <<"未选中";
        apportlogflag = 0;
    }

}
//dpkglog点选
void feedback::on_checkBox_3_stateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        emit syslog();
        qDebug() <<"选中";
        dpkglogflag = 1;
    }
    else // 未选中 - Qt::Unchecked
    {
        qDebug() <<"未选中";
        dpkglogflag = 0;
    }
}


void feedback::on_checkBox_4_stateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        qDebug() <<"选中";
        get_systeminfoflag = 1;
    }
    else // 未选中 - Qt::Unchecked
    {
        qDebug() <<"未选中";
        get_systeminfoflag = 0;
    }
}


//获取反馈类型
void feedback::on_comboBox_currentIndexChanged(const QString &arg1)
{
    feedback_type = ui->comboBox->currentText();
}
//提交按钮
void feedback::on_pushButton_2_clicked()
{
    ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(65,95,196);color: rgb(68, 68, 68)");
}

//邮箱是否填写
void feedback::on_textEdit_2_textChanged()
{
    emailflag = 1;
    email_str = ui->textEdit_2->toPlainText();
    if (describeflag == 1){//邮箱和详细描述都已经填写
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(61,107,229);color: rgb(68, 68, 68)");
    }
}

void feedback::hide_add_file()
{

}
void feedback::show_add_file()
{

}
//添加文件删除按钮
void feedback::add_del_file_button()
{
    QPushButton *file_del_button = new QPushButton(this);
    file_del_button->setGeometry(filename_x+filename_width+20+filesize_width+20,filename_y+ (add_file_num * 29),filename_w,filename_h);
    file_del_button->setText("删除");
    //file_del_button->setFlat(true);
    file_del_button->setStyleSheet("font: 9pt 'Sans Serif';color: rgb(107,190,235)");
    file_del_button->adjustSize();
    file_del_button->show();
}
//文件名添加到label中
void feedback::set_filename_to_label()
{
      int nIndex = filename.lastIndexOf('/');
      QString file_name = filename.mid(nIndex+1);
      QLabel *filename_label= new QLabel(this);

      filename_label->setGeometry(filename_x,filename_y+ (add_file_num * 29),filename_w,filename_h);

      filename_label->setText(file_name);
      filename_label->setStyleSheet("font: 10pt 'Sans Serif';background-color:rgb(107,190,235)");
      filename_label->adjustSize();
      filename_width = filename_label->geometry().width();
      filename_label->show();
}
//文件大小添加到label中
void feedback::set_filesize_to_label()
{
    QFileInfo info;
    QString file_size;
    info.setFile(filename);
    file_size = QString::number((float)info.size()/(float)1000,'f',1) + "K";
    QLabel *filesize_label= new QLabel(this);
    filesize_label->setGeometry(filename_x+filename_width+20,filename_y + (add_file_num * 29),filename_w,filename_h);
    filesize_label->setText(file_size);
    filesize_label->setStyleSheet("font: 10pt 'Sans Serif';background-color:rgb(107,190,0)");
    filesize_label->adjustSize();
    filesize_width = filesize_label->geometry().width();

    filesize_label->show();
}
//添加附件后 调整窗口
void feedback::add_file_change_window()
{
    QString name;
    if (add_file_num > 1){
        window_h += 29;
    }
    pushbutton2_y += 29;
    pushbutton3_y += 29;
    checkbox4_y += 29;
    widget_y += 29;


    ui->pushButton_2->setGeometry(pushbutton2_x,pushbutton2_y,pushbutton2_w,pushbutton2_h);
    ui->pushButton_3->setGeometry(pushbutton3_x,pushbutton3_y,pushbutton3_w,pushbutton3_h);
    ui->checkBox_4->setGeometry(checkbox4_x,checkbox4_y,checkbox4_w,checkbox4_h);
    ui->verticalWidget->setGeometry(widget_x,widget_y,widget_w,widget_h);
    setFixedSize(600,window_h);
}

void feedback::on_lineEdit_textChanged(const QString &arg1)
{
    set_filename_to_label();
    set_filesize_to_label();
    add_del_file_button();
    add_file_num += 1;   //添加文件个数
    add_file_change_window();
}


