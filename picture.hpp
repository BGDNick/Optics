#ifndef PICTURE_HPP
#define PICTURE_HPP
#include <vector>
#include <QDialog>
#include "pixel.hpp"
#include "line.hpp"
#include "IncludeAll.hpp"

namespace Ui {
class Picture;
}

class Picture : public QDialog
{
    Q_OBJECT

public:
    explicit Picture(QWidget *parent = nullptr);
    ~Picture();
    void calculate(QList<Line> lines);
public:
    std::vector<std::vector<Pixel>> pixels;
    size_t bright_level;
    size_t quantity;
    double position;
    std::unique_ptr<QGraphicsScene> scene;
private:
    Ui::Picture *ui;
};

#endif // PICTURE_HPP
