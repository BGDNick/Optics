#ifndef PICTURE_HPP
#define PICTURE_HPP

#include <QDialog>

namespace Ui {
class Picture;
}

class Picture : public QDialog
{
    Q_OBJECT

public:
    explicit Picture(QWidget *parent = nullptr);
    ~Picture();

private:
    Ui::Picture *ui;
};

#endif // PICTURE_HPP
