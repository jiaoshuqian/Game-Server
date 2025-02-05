#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QDesktopServices>
#include "dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createroom_clicked();
    void proc_login_reply();

    void on_register_2_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager m_access_mng;
    QNetworkReply *m_reply = NULL;
    Dialog d;
};

#endif // MAINWINDOW_H
