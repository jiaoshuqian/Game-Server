#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_createroom_clicked()
{
    //取出用户名密码
    QString username = ui->username->text();
    QString password = ui->password->text();

    //创建密码的签名值
    QCryptographicHash md5_hash(QCryptographicHash::Md5);
    md5_hash.addData(password.toUtf8());
    QString md5_password(md5_hash.result().toHex());

    //拼登陆请求(Json格式)
    //构建一个json对象，填入用户名和密码的签名值
    QJsonObject json;
    json["username"] = username;
    json["password"] = md5_password;

    //将json对象放到doc中
    QJsonDocument json_doc(json);

    QString output = json_doc.toJson();

    //发给登陆服务器（发http---》qt网络访问库）
    QNetworkRequest req(QUrl("http://192.168.64.148/login/"));
    req.setHeader(QNetworkRequest::ContentLengthHeader, output.size());
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_reply = m_access_mng.post(req,output.toUtf8());
    //异步处理服务器的回复（connect 服务器回复和回复的处理函数）
    connect(m_reply, SIGNAL(finished()), this, SLOT(proc_login_reply()));

}

void MainWindow::proc_login_reply()
{
    //判断登陆结果
    QJsonDocument json_doc = QJsonDocument::fromJson(m_reply->readAll());

    QJsonObject json = json_doc.object();

    if ("OK" == json["login_result"].toString())
    {
        qDebug() << "login success";
        QProcess pro;
        QStringList argv;
        argv.push_back("192.168.64.148");
        argv.push_back("8899");
        pro.startDetached("client.exe", argv,".");
        qApp->exit();
    }
    else {
        QMessageBox::information(this, "登陆失败", "用户名或密码错误");
        d.show();
    }
}

void MainWindow::on_register_2_clicked()
{
    //启动浏览器并访问注册界面
    QDesktopServices::openUrl(QUrl("http://192.168.64.148"));
}
