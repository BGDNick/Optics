#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QLibrary lib("C:/Windows/System32/shell32.dll");
    std::cout << lib.load() << lib.isLoaded() << std::endl;
    QLibrary lib2("C:/Users/Nick/Downloads/Opticsproject.dll");
    std::cout << lib.load() << lib.isLoaded() << std::endl;

    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

