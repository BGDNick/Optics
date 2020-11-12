#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP
#include <iostream>
#include <QMainWindow>
#include <QLibrary>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_HPP