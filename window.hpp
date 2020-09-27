#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include "IncludeAll.hpp"
#include "rectangle.hpp"
#include "line.hpp"
#include "lens.hpp"
#include "light.hpp"
#include "makepicture.hpp"
#include "picture.hpp"

QT_BEGIN_NAMESPACE
namespace Ui { class Window; }
QT_END_NAMESPACE

class Window : public QMainWindow
{
    Q_OBJECT

public:

    Window(QWidget *parent = nullptr);

    QString getString(bool flag = true);

    ~Window();

    void send(bool flag = true);

    void on_pushButtonCalculate_clicked();
public slots:


    void on_pushButtonClear_clicked();

private slots:

    void newDocument();

    void load();

    void save();

    void saveAs();

    void removeLines();

    void removeAll();

    void on_pushButtonTextEnter_clicked();

    void on_toolButtonScaleInc_clicked();

    void on_toolButtonDec_clicked();

    void on_pushButtonLensAdd_clicked();

    //void on_pushButtonRectAdd_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    //void on_comboBoxLightType_activated(const QString &arg1);

    //void on_pushButtonLightAdd_clicked();

    //void on_spinBoxLightAmount_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButtonCalculate_2_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_actionAll_triggered();

    void on_actionLines_triggered();

    void on_actionDraw_triggered();

    void on_actionCreate_triggered();

    void on_actionAdd_Picture_triggered();

    void on_doubleSpinBoxScreenPos_valueChanged(double arg1);

    void on_spinBoxScreenBrightLevel_valueChanged(int arg1);

    void on_actionShow_Result_triggered();


    void on_pushButton_clicked();

public:
    QList<Line> lines;
    QList<Lens> lens;
    QList<Pixel> pixels;
    //QList<Rect> rects;
    Ui::Window *ui;
    std::unique_ptr<QGraphicsScene> scene;
    MakePicture secondScreen;
    Picture thirdScreen;
    double screenPosition;
    //QGraphicsScene *scene;
private:
    std::pair<double, double> mainFrame = std::make_pair(1000, 1000);
    bool is_saved = true; 
    QList<QListWidgetItem> items;   
    QGraphicsView view;
    QString currentFile;
    double current_scale = 0.0;
    const QLibrary lib;

};
#endif // WINDOW_HPP

/*
//сделать др класса win
#ifndef SERVER_HPP
#define SERVER_HPP
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(Window* i_w, bool flag_i = true, QObject *parent = nullptr);

signals:

public slots:
    void connected();

private:
    Window* w;
    QTcpSocket* socket;
    QTcpServer* server;
    bool flag;
};
#endif // SERVER_HPP
*/
