#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QMainWindow>
#include "IncludeAll.hpp"
#include "rectangle.hpp"
#include "line.hpp"
#include "lens.hpp"
#include "light.hpp"
#include "makepicture.hpp"

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

public slots:
    void on_pushButtonCalculate_clicked(QVector<bool> check = {true, true, true, true});

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

    void on_comboBoxLightType_activated(const QString &arg1);

    void on_pushButtonLightAdd_clicked();

    void on_spinBoxLightAmount_valueChanged(int arg1);

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButtonCalculate_2_clicked();

    void on_doubleSpinBox_2_valueChanged(double arg1);

    void on_actionAll_triggered();

    void on_actionLines_triggered();

    void on_actionDraw_triggered();

    void on_actionCreate_triggered();

public:
    QList<Line> lines;
    QList<Lens> lens;
    QList<Light> lights;
    //QList<Rect> rects;
    Ui::Window *ui;
    std::unique_ptr<QGraphicsScene> scene;
    MakePicture secondScreen;
    //QGraphicsScene *scene;
private:
    std::pair<double, double> mainFrame = std::make_pair(500.0, 500.0);
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
