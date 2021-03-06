#ifndef MAKEPICTURE_HPP
#define MAKEPICTURE_HPP
#include "IncludeAll.hpp"
#include <QDialog>
#include <QMouseEvent>
#include <iostream>

namespace Ui {
class MakePicture;
}

class MakePicture : public QDialog
{
    Q_OBJECT

public:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    explicit MakePicture(QWidget *parent = nullptr);
    ~MakePicture();

private slots:
    void on_buttonBox_accepted();

    void on_spinBoxR_valueChanged(int arg1);

    void on_spinBoxG_valueChanged(int arg1);

    void on_spinBoxB_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MakePicture *ui;
    std::unique_ptr<QGraphicsScene> colorScene;
    std::unique_ptr<QGraphicsScene> drawingScene;
};

#endif // MAKEPICTURE_HPP
