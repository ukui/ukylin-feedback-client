#include "feedback.h"
#include "ui_feedback.h"

#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QLabel>
#include <QPushButton>
#include <QSqlRecord>
#include <QMessageBox>
#include "database.h"


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




    model = new QSqlTableModel(this);
    model->setTable("clock");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行

    ui->textEdit->setPlaceholderText("请输入内容");//设置详细输入框的提示信息
    ui->lineEdit->setPlaceholderText("文件大小不能超过3MB");
}

feedback::~feedback()
{
    //程序结束时删除所有数据------
    int rowNum =model->rowCount();
    model->removeRows(0,rowNum);
    model->submitAll();
    //---------------------------


    delete ui;
}
//获取图片
void feedback::on_pushButton_clicked()
{
    ui->pushButton->setStyleSheet("font: 14px;border-radius:4px;background-color:rgb(65,95,196);color: rgb(68, 68, 68)");
    filename=QFileDialog::getOpenFileName(this,tr("select image"),"/","Image file(*.gif *.jpg *.png)",0);
    //判断文件是否重复添加
    int rowNum = model->rowCount();
    if (rowNum ==0)
    {
        ui->lineEdit->setText(filename);
    }
    else{
        for(int fileNum=0; fileNum<rowNum; fileNum++)
        {
            if(filename.compare(model->index(fileNum, 2).data().toString()) == 0)
            {
                qDebug("file xiangtong");//添加的文件已经添加过
                return ;
            }
            else{
                ui->lineEdit->setText(filename);
            }
        }
    }

}


//设置详细描述框最大字符数
void feedback::on_textEdit_textChanged()
{
    QString textContent = ui->textEdit->toPlainText();
    if (textContent.isEmpty()){
        describeflag = 0;
    }
    else
        describeflag = 1;//详细描述是否填写
    if (emailflag == 1 && describeflag == 1){//邮箱和详细描述都已经填写
        ui->pushButton_2->setEnabled(true);//设置提交按钮属性
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(61,107,229);color: rgb(68, 68, 68)");
    }
    else
    {
        ui->pushButton_2->setEnabled(false);//设置提交按钮属性
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(233, 233, 233);color: rgb(255, 255, 255)");
    }

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
}

//syslog点选
void feedback::on_checkBox_stateChanged(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        emit syslog();
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

//是否获取系统信息
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
    email_str = ui->textEdit_2->toPlainText();
    if (email_str.isEmpty()){
        emailflag = 0;
    }
    else{
        emailflag = 1;
    }
    if (describeflag == 1 && emailflag == 1){//邮箱和详细描述都已经填写
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(61,107,229);color: rgb(68, 68, 68)");
    }
    else
    {
        ui->pushButton_2->setEnabled(false);//设置提交按钮属性
        ui->pushButton_2->setStyleSheet("font: 18pt 'Sans Serif';background-color:rgb(233, 233, 233);color: rgb(255, 255, 255)");
    }
}

//删除文件按钮槽函数
void feedback::del_file_button_clicked()
{
    int rowNum = model->rowCount();

    //QObject::sender()返回发送信号的对象的指针
    QPushButton *btn = qobject_cast<QPushButton*>(QObject::sender());

    if( btn == deletefileBtn[0] )
    {
        model->removeRows(0, 1);
        qDebug() << "delete 0";
    }
    else if( btn == deletefileBtn[1] )
    {
        model->removeRows(1, 1);

        qDebug() << "delete 1";
    }
    else if( btn == deletefileBtn[2] )
    {
        model->removeRows(2, 1);
        qDebug() << "delete 2";
    }
    else if( btn == deletefileBtn[3] )
    {
        model->removeRows(3, 1);
    }
    else if( btn == deletefileBtn[4])
    {
        model->removeRows(4, 1);
    }
    for(int i=0; i<rowNum; i++)
    {
        delete filename_label[i];
        delete filesize_label[i];
        delete deletefileBtn[i];
    }

    model->submitAll();   //提交, 在数据库中删除该行

    update_add_file_window();
    del_file_change_window();

    qDebug()<<model->rowCount()<<"-------";

    //删除文件后 把上传附件中内容更新
    //update_linedit_add_or_del_file();
}
//删除附件后调整窗口
void feedback::del_file_change_window()
{
    int rowNum = model->rowCount();
    window_h -= 29;
    pushbutton2_y -= 29;
    pushbutton3_y -= 29;
    checkbox4_y -= 29;
    widget_y -= 29;


    ui->pushButton_2->setGeometry(pushbutton2_x,pushbutton2_y,pushbutton2_w,pushbutton2_h);
    ui->pushButton_3->setGeometry(pushbutton3_x,pushbutton3_y,pushbutton3_w,pushbutton3_h);
    ui->checkBox_4->setGeometry(checkbox4_x,checkbox4_y,checkbox4_w,checkbox4_h);
    ui->verticalWidget->setGeometry(widget_x,widget_y,widget_w,widget_h);
    setFixedSize(600,window_h);
}
//添加附件后调整窗口
void feedback::add_file_change_window()
{
    if(filename.isEmpty()){
        return;
    }

    window_h += 29;
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
//添加附件框改变
void feedback::on_lineEdit_textChanged(const QString &arg1)
{
    add_fileinfo_model();
    add_file_change_window();
}
//添加文件后把文件信息加入数据库
void feedback::add_fileinfo_model()
{
    if(filename.isEmpty()){
        return;
    }
    //获取文件名字
    int nIndex = filename.lastIndexOf('/');
    QString file_name = filename.mid(nIndex+1);
    //获取文件大小
    QFileInfo info;
    QString file_size;
    info.setFile(filename);
    file_size = QString::number((float)info.size()/(float)1000,'f',1) + "K";

    int rowNum = model->rowCount();
    if(rowNum < 5)
    {
        model->insertRow(rowNum);
        model->setData(model->index(rowNum, 0), file_name);
        model->setData(model->index(rowNum, 1), file_size);
        model->setData(model->index(rowNum, 2), filename);
        model->submitAll();
        model->setTable("clock");
        model->select();

        for(int i=0; i<rowNum; i++)
        {
            delete filename_label[i];
            delete filesize_label[i];
            delete deletefileBtn[i];
        }
        update_add_file_window();
    }
}
//根据数据库 刷新窗口
void feedback::update_add_file_window()
{
    if(filename.isEmpty()){
        return;
    }
    int rowNum = model->rowCount();
    for(int filenum=0; filenum<rowNum; filenum++)
    {
        filename_label[filenum] = new QLabel(this);
        filename_label[filenum]->move(filename_x,filename_y+(filenum * 29));
        filename_label[filenum]->setText(model->index(filenum, 0).data().toString().mid(model->index(filenum, 0).data().toString().lastIndexOf('/')+1));
        filename_label[filenum]->setStyleSheet("font: 10pt 'Sans Serif'");
        filename_label[filenum]->adjustSize();
        filename_label[filenum]->show();
        int filename_width = filename_label[filenum]->geometry().width();

        filesize_label[filenum] = new QLabel(this);
        filesize_label[filenum]->setGeometry(filename_x+filename_width+20,filename_y+ (filenum * 29),filename_w,filename_h);
        filesize_label[filenum]->setText(model->index(filenum, 1).data().toString());
        filesize_label[filenum]->setStyleSheet("font: 10pt 'Sans Serif'");
        filesize_label[filenum]->adjustSize();
        filesize_label[filenum]->show();
        int filesize_width = filesize_label[filenum]->geometry().width();

        deletefileBtn[filenum] = new QPushButton(this);
        deletefileBtn[filenum]->setGeometry(filename_x+filename_width+20+filesize_width+20,filename_y+ (filenum * 29),31,26);
        deletefileBtn[filenum]->setText("删除");
        deletefileBtn[filenum]->setStyleSheet("font: 10pt 'Sans Serif';color: rgb(107,190,235)");
        deletefileBtn[filenum]->setFlat(true);
        deletefileBtn[filenum]->show();
        connect( deletefileBtn[filenum], SIGNAL(clicked()), this, SLOT(del_file_button_clicked()) );
    }
}
//在删除文件之后更新文件信息框
void feedback::update_linedit_add_or_del_file()
{
}

