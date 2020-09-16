#ifndef MAKEPICTURE_HPP
#define MAKEPICTURE_HPP

#include <QDialog>

namespace Ui {
class MakePicture;
}

class MakePicture : public QDialog
{
    Q_OBJECT

public:
    explicit MakePicture(QWidget *parent = nullptr);
    ~MakePicture();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::MakePicture *ui;
};

#endif // MAKEPICTURE_HPP
