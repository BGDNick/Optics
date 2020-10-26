#include "window.hpp"
#include "ui_window.h"
#include <iostream>

//WINDOW

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window),
      secondScreen(),
      thirdScreen(),
      lib("Opticsproject_4.dll")
{
    std::cout << lib.load() << lib.errorString().toStdString() << std::endl;
    ui->setupUi(this);
    setWindowTitle("TheProgramm");
    setWindowIcon(QIcon(":/resources/data/Logo.png"));
    connect(ui->actionNew, &QAction::triggered, this, &Window::newDocument);
    connect(ui->actionLoad, &QAction::triggered, this, &Window::load);
    connect(ui->actionSave, &QAction::triggered, this, &Window::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Window::saveAs);
    connect(ui->actionSave, &QAction::triggered, this, &Window::save);
    connect(ui->actionRemove_lines, &QAction::triggered, this, &Window::removeLines);
    connect(ui->actionRemove_All, &QAction::triggered, this, &Window::removeAll);


    //дополнительные действия
    this->setFixedSize(1280, 720);
    secondScreen.setVisible(false);
    thirdScreen.setVisible(false);
    //добавление первого пикселя
    pixels.push_back(secondScreen.pixels.at(0).at(0));
    ui->plainTextEdit->setPlainText(this->getString());
    // выключение некоторых полей
    //on_comboBoxLightType_activated("Single");
    ui->progressBar->setVisible(false);
    scene = std::make_unique<QGraphicsScene>(this);
    ui->graphicsView->setScene(scene.get());
    screenPosition = 500.0;
    on_pushButtonCalculate_clicked();


}

Window::~Window()
{
    delete ui;
    //Server sender_of_the_end(this, false); // конец работы сервера
}

//Функции вкладки File
void Window::save()
{
    QString fileName;
    if(currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
        setWindowTitle(fileName);
    }
    else
    {
        fileName = currentFile;
    }

    QFile file(fileName);

    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file" + file.errorString());
        return;
    }

    QTextStream out(&file);

    QString text;
    text = this->getString();
    out << text;
    file.close();
    is_saved = true;

}

void Window::load()
{
    if(!is_saved) // предложение сохранить файл если он не сохранен
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Would you like to save?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            save();
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Load the file");
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open file");
        return;
    }
    setWindowTitle(fileName);

    //очистка всех объектов
    lines.clear();
    //rects.clear();
    lens.clear();
    pixels.clear();
    items.clear();
    ui->listWidget->clear();
    scene.get()->clear();

    //считвание файла
    QTextStream in(&file);
    QString text = in.readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject jObj = jDoc.object();

    //настройка экрана
    QJsonObject obj = jObj.value("boarders").toObject();

    mainFrame.first = obj.value("dx").toDouble();
    mainFrame.second = obj.value("dy").toDouble();
    mainFrameZ = obj.value("dz").toDouble();

    ui->doubleSpinBox->setValue(mainFrame.first);
    ui->doubleSpinBox_2->setValue(mainFrame.second);
    ui->doubleSpinBox_3->setValue(mainFrameZ);

    //настройка изображения

    QJsonObject screenObj = jObj.value("screen").toObject();

    ui->spinBoxScreenBrightLevel->setValue(screenObj.value("bright").toInt());
    thirdScreen.bright_level = screenObj.value("bright").toInt();

    screenPosition = screenObj.value("pos").toDouble();
    ui->doubleSpinBoxScreenPos->setValue(screenPosition);
    thirdScreen.position = screenPosition;

    ui->spinBoxScreenSize->setValue(screenObj.value("size").toInt());
    thirdScreen.size = screenObj.value("size").toInt();
    //настройка всего остального

    QJsonArray jArr_lines = jObj.value("line").toArray();
    //QJsonArray jArr_rects = jObj.value("screen").toArray();
    QJsonArray jArr_lens = jObj.value("lens").toArray();
    QJsonArray jArr_pixels = jObj.value("sour").toArray();

    // добавление линий
    foreach(QJsonValue jValue, jArr_lines)
    {
        QJsonObject jLine = jValue.toObject();
        Line temp;
        temp.fromJsonObject(jLine);
        lines.append(temp);
    }

    //добавление пикселей
    foreach (QJsonValue jValue, jArr_pixels)
    {
        QJsonObject jPixel = jValue.toObject();
        Pixel temp;
        temp.fromJsonObject(jPixel);
        pixels.append(temp);
    }
    //добавлениие и прорисовка пикселей во втором окне
    secondScreen.pixels.clear();
    int amount = int(sqrt(pixels.size()));

    for(int i = 0; i < amount; i++)
    {
        secondScreen.pixels.push_back(std::vector<Pixel>());
        for(int j = 0; j < amount; j++)
        {
            secondScreen.pixels.at(i).push_back(pixels.at(i+j));
        }
    }
    secondScreen.on_spinBox_valueChanged(secondScreen.pixels.size());

    /*// добавление прямоугольников
    foreach(QJsonValue jValue, jArr_rects)
    {
        QJsonObject jRect = jValue.toObject();
        Rect temp;
        temp.fromJsonObject(jRect);
        rects.append(temp);

        //добваление в список объектов
        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Rectangle");
        ui->listWidget->addItem(item);
        items.append(*item);
    }
*/
    // добавление линз
    foreach(QJsonValue jValue, jArr_lens)
    {
        QJsonObject jLens = jValue.toObject();
        Lens temp;
        temp.fromJsonObject(jLens);
        lens.append(temp);

        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Lens");
        ui->listWidget->addItem(item);
        items.append(*item);
    }


    currentFile = fileName;
    is_saved = true;

    ui->plainTextEdit->setPlainText(text);
    on_pushButtonCalculate_clicked();
}

void Window::newDocument()
{
    if(!is_saved) // предложение сохранить файл если он не сохранен
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Wrning", "Would you like to save?", QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            save();
        }

    }

    setWindowTitle("TheProgramm");
    currentFile.clear();
    is_saved = true;
    //работа с объектами
    lines.clear();
    //rects.clear();
    lens.clear();
    pixels.clear();
    //настройка рамок
    ui->doubleSpinBox->setValue(1000);
    ui->doubleSpinBox_2->setValue(1000);
    ui->doubleSpinBox_3->setValue(1000);

    //настройка изображения
    ui->spinBoxScreenBrightLevel->setValue(1);
    ui->spinBoxScreenSize->setValue(440);
    ui->doubleSpinBoxScreenPos->setValue(500);

    thirdScreen.bright_level = 1;
    thirdScreen.quantity = 1;
    thirdScreen.position = 500;
    thirdScreen.pixels.clear();

    //настройка окна построения избражения
    secondScreen.pixels.clear();
    std::vector<Pixel> first_pixel;
    first_pixel.push_back(Pixel(0, 0, 0, 1, 255, 255, 255, 440, 440));
    secondScreen.pixels.push_back(first_pixel);
    secondScreen.pixels.back().back().drawPixel(secondScreen.drawingScene.get());

    //настройка всего остального

    on_actionAll_triggered();
    items.clear();
    ui->listWidget->clear();
    ui->plainTextEdit->clear();
    on_pushButtonCalculate_clicked();
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    currentFile = fileName;
    save();
}


void Window::on_pushButtonTextEnter_clicked()
{
    QString text;
    text = ui->plainTextEdit->toPlainText();
    lines.clear();
    //rects.clear();
    lens.clear();
    pixels.clear();
    items.clear();
    ui->listWidget->clear();
    //паста из load
    QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject jObj = jDoc.object();

    //настройка границ
    QJsonObject obj = jObj.value("boarders").toObject();

    mainFrame.first = obj.value("dx").toDouble();
    mainFrame.second = obj.value("dy").toDouble();
    mainFrameZ = obj.value("dz").toDouble();

    ui->doubleSpinBox->setValue(mainFrame.first);
    ui->doubleSpinBox_2->setValue(mainFrame.second);
    ui->doubleSpinBox_3->setValue(mainFrameZ);
    //настройка экрана

    QJsonObject screenObj = jObj.value("screen").toObject();

    ui->spinBoxScreenBrightLevel->setValue(screenObj.value("bright").toInt());
    thirdScreen.bright_level = screenObj.value("bright").toInt();

    screenPosition = screenObj.value("pos").toDouble();
    ui->doubleSpinBoxScreenPos->setValue(screenPosition);
    thirdScreen.position = screenPosition;

    ui->spinBoxScreenSize->setValue(screenObj.value("sixe").toInt());
    thirdScreen.size = screenObj.value("size").toInt();
    //настройка всего остального

    QJsonArray jArr_lines = jObj.value("line").toArray();
    //QJsonArray jArr_rects = jObj.value("screen").toArray();
    QJsonArray jArr_lens = jObj.value("lens").toArray();
    QJsonArray jArr_pixels = jObj.value("sour").toArray();

    // добавление линий
    foreach(QJsonValue jValue, jArr_lines)
    {
        QJsonObject jLine = jValue.toObject();
        Line temp;
        temp.fromJsonObject(jLine);
        lines.append(temp);
    }

    /*// добавление прямоугольников
    foreach(QJsonValue jValue, jArr_rects)
    {
        QJsonObject jRect = jValue.toObject();
        Rect temp;
        temp.fromJsonObject(jRect);
        rects.append(temp);

        //добваление в список объектов
        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Rectangle");
        ui->listWidget->addItem(item);
        items.append(*item);
    }
*/
    // добавление линз
    foreach(QJsonValue jValue, jArr_lens)
    {
        QJsonObject jLens = jValue.toObject();
        Lens temp;
        temp.fromJsonObject(jLens);
        lens.append(temp);

        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Lens");
        ui->listWidget->addItem(item);
        items.append(*item);
    }

    //добавление пикселей
    foreach (QJsonValue jValue, jArr_pixels)
    {
        QJsonObject jPixel = jValue.toObject();
        Pixel temp;
        temp.fromJsonObject(jPixel);
        pixels.append(temp);
    }
    //добавлениие и прорисовка пикселей во втором окне
    secondScreen.pixels.clear();
    int amount = int(sqrt(pixels.size()));
    for(int i = 0; i < amount; i++)
    {
        secondScreen.pixels.push_back(std::vector<Pixel>());
        for(int j = 0; j < amount; j++)
        {
            secondScreen.pixels.at(i).push_back(pixels.at(i+j));
        }
    }
    //прорисовка делается таким образом, что количество пикселей не менятся
    secondScreen.on_spinBox_valueChanged(secondScreen.pixels.size());


    ui->plainTextEdit->setPlainText(text);
    on_pushButtonCalculate_clicked();

    //конец пасты

    is_saved = false;
    on_pushButtonCalculate_clicked();

}

void Window::on_pushButtonClear_clicked()
{
    ui->plainTextEdit->clear();
}

//отрисовка всего
void Window::on_pushButtonCalculate_clicked()
{
    scene.get()->clear();

    scene.get()->addRect(-mainFrame.first, - mainFrame.second, 2 * mainFrame.first, 2 * mainFrame.second);

    //отрисовка линий
    foreach (Line l, lines)
    {
        QPen pen(QColor(l.rgb["r"], l.rgb["g"], l.rgb["b"]));
        pen.setWidth(int(current_scale));
        scene.get()->addLine(l.x1, l.y1, l.x2, l.y2, pen);
    }


    /*//отрисовка прямоугольников
    foreach (Rect r, rects)
    {
        scene->addLine(r.coord1.first, r.coord1.second, r.coord2.first, r.coord2.second, QPen(QColorConstants::Svg::firebrick));
    }*/

    //отрисовка линз

    foreach( Lens l, lens)
    {
        l.draw(scene.get());
    }

    /*
    //отрисовка источников света
    if(check[2])
    {
        foreach(Light l, lights)
        {
            l.draw(scene.get());
        }
    }*/

    //отрисовка местоположения изображения
    scene.get()->addRect(0, 0, 10, 440, QPen(), QBrush(QColor(255,255,0)));
    //отрисовка местоположения экрана
    scene.get()->addRect(screenPosition, 0, 10, ui->spinBoxScreenSize->text().toInt(), QPen(), QBrush(QColor(0,255,255)));
}

void Window::on_toolButtonScaleInc_clicked() // кнопка увеличения масштаба картинки
{
    ui->graphicsView->scale(1.5,1.5);
}

void Window::on_toolButtonDec_clicked() // кнопка уменьшения масштаба картинки
{
    ui->graphicsView->scale(0.75, 0.75);
}

void Window::on_pushButtonLensAdd_clicked() //кнопка добавления линзы
{
    lines.clear();

    // добавление линзы
    Lens added_lens;
    added_lens.coord = std::pair<double, double>(ui->lineEditLensCoordX->text().toDouble(), ui->lineEditLensCoordY->text().toDouble());
    added_lens.radius = std::pair<double, double>(ui->lineEditLensRadiusLeft->text().toDouble(),
                                                  ui->lineEditLensRadiusRight->text().toDouble());
    added_lens.length = ui->lineEditLensLength->text().toDouble();
    added_lens.width = ui->lineEditLensWidth->text().toDouble();
    added_lens.r_index = ui->lineEditLensR_Index->text().toDouble();
    added_lens.name = ui->lineEditLensName->text();
    added_lens.type = "Flat"; //константа перечисления
    added_lens.angle = 0;
    added_lens.height = 0;

    // проверка на то занято ли имя
    int counter = 0;
    int count = ui->listWidget->count();
    while(counter < count)
    {

        if((items[counter].text().toStdString() == added_lens.name.toStdString()))
        {
            QMessageBox::information(this, "Warning", "Names must be different");
            return;
        }
        counter++;
    }

    //проверка на коректность введенных данных
    if((abs(added_lens.radius.first) < added_lens.length / 2 + 1) ||(abs(added_lens.radius.second) < (added_lens.length / 2 + 1)) ||
            added_lens.r_index > 5)
    {
        QMessageBox::information(this, "Warning", "Incorrect lens' data");
        return;
    }
    //Добавление в линзы
    lens.append(added_lens);

    //добавление  объект в список объектов

    QListWidgetItem* item = new QListWidgetItem(added_lens.name, ui->listWidget);
    item->setWhatsThis("Lens");
    ui->listWidget->addItem(item);
    items.append(*item);
    // восстановление виджета
    ui->lineEditLensName->setText("Lens");
    ui->lineEditLensCoordX->clear();
    ui->lineEditLensCoordY->clear();
    ui->lineEditLensLength->clear();
    ui->lineEditLensWidth->clear();
    ui->lineEditLensRadiusLeft->clear();
    ui->lineEditLensRadiusRight->clear();
    ui->lineEditLensR_Index->clear();

    is_saved = false;

    //отрисовка с новой линзой
    on_pushButtonCalculate_clicked();
    //корректировка json текста
    ui->plainTextEdit->setPlainText(this->getString());

}

/*void Window::on_pushButtonRectAdd_clicked() //кнопка добавления прямоугольника
{
    lines.clear();

    //добавление прямоугольника
    Rect added_rect;
    added_rect.coord1 = std::make_pair(ui->lineEditRectX->text().toDouble(), ui->lineEditRectY->text().toDouble() + ui->lineEditRectLength->text().toDouble() / 2);
    added_rect.coord2 = std::make_pair(ui->lineEditRectX->text().toDouble(), ui->lineEditRectY->text().toDouble() - ui->lineEditRectLength->text().toDouble() / 2);
    added_rect.name = ui->lineEditRectName->text();

    //проверка на то занято ли имя
    int counter = 0;
    int count = ui->listWidget->count();
    while(counter < count)
    {

        if((items[counter].text().toStdString() == added_rect.name.toStdString()))
        {
            QMessageBox::information(this, "Warning", "Names must be different");
            return;
        }
        counter++;
    }

    rects.append(added_rect);


    //добавление объекта в список объектов
    QListWidgetItem* item = new QListWidgetItem(added_rect.name, ui->listWidget);
    item->setWhatsThis("Rectangle");
    items.append(*item);
    ui->listWidget->addItem(item);
    //восставновление виджета
    ui->lineEditRectName->setText("Rectangle");
    ui->lineEditRectX->clear();
    ui->lineEditRectY->clear();
    //отрисовка с новым прямоугольником
    on_pushButtonCalculate_clicked();
    //добавление в json текст
    ui->plainTextEdit->setPlainText(this->getString());
}*/

void Window::on_listWidget_itemDoubleClicked(QListWidgetItem *item) // если по одному из элементов в списке элементов нажали два раза
{
    is_saved = false;
    //удаление объекта из items
    QList<QListWidgetItem> temp; // qvector
    foreach(QListWidgetItem it, items)
    {
        if(it.text() == item->text())
        {
            //pass
        }
        else
        {
            temp.append(it);
        }
    }
    items = temp;
    /*//если объект - прямоугольник
    if(item->whatsThis() == "Rectangle")
    {
        QList<Rect> temp;
        foreach(Rect r, rects )
        {
            if(r.name == item->text())
            {
                //pass
            }
            else
            {
                temp.append(r);
            }

        }
        rects = temp;
    }*/
    if(item->whatsThis() == "Lens")
    {
        QList<Lens> temp;
        foreach(Lens l, lens )
        {
            if(l.name == item->text())
            {
                //pass
            }
            else
            {
                temp.append(l);
            }

        }
        lens = temp;
    }
    /*
    if(item->whatsThis() == "Light")
    {
        QList<Light> temp;
        foreach (Light l, lights)
        {
            if(l.name == item->text())
            {
                //pass
            }
            else
            {
                temp.append(l);
            }
        }
        lights = temp;
    }*/

    //отрисовка без удаленного предмета
    on_pushButtonCalculate_clicked();
    //корректировка json текста
    ui->plainTextEdit->setPlainText(this->getString());

    delete ui->listWidget->takeItem(ui->listWidget->row(item));

}


//действия при нажатии на объект в списке
void Window::on_listWidget_itemClicked(QListWidgetItem *item)
{
    if(item->whatsThis() == "Lens")
    {
        ui->toolBox->setCurrentIndex(0);
        foreach(Lens l, lens)
        {
            if(l.name == item->text())
            {
                ui->lineEditLensCoordX->setText(QString::number(l.coord.first));
                ui->lineEditLensCoordY->setText(QString::number(l.coord.second));
                ui->lineEditLensLength->setText(QString::number(l.length));
                ui->lineEditLensWidth->setText(QString::number(l.width));
                ui->lineEditLensName->setText(l.name);
                ui->lineEditLensRadiusLeft->setText(QString::number(l.radius.first));
                ui->lineEditLensRadiusRight->setText(QString::number(l.radius.second));
                ui->lineEditLensR_Index->setText(QString::number(l.r_index));

                return;
            }
        }
    }
    /*else
    if(item->whatsThis() == "Rectangle")
    {
        ui->toolBox->setCurrentIndex(1);
        foreach(Rect r, rects)
        {
            if(r.name == item->text())
            {
                ui->lineEditRectName->setText((r.name)) ;
                ui->lineEditRectLength->setText(QString::number(r.coord1.second - r.coord2.second));
                ui->lineEditRectX->setText(QString::number(r.coord1.first));
                ui->lineEditRectY->setText(QString::number(r.coord1.second/ 2 + r.coord2.second / 2));
                return;
            }
        }
    }*/
    /*
    else
    if(item->whatsThis() == "Light")
    {
        ui->toolBox->setCurrentIndex(1);
        foreach(Light l, lights)
        {
            if(l.name == item->text())
            {

                ui->lineEditLightName->setText(l.name);
                ui->lineEditLightX->setText(QString::number(l.x1));
                ui->lineEditLightY->setText(QString::number(l.y1));
                ui->doubleSpinBoxLightPower->setValue(l.power);
                if(l.type)
                {
                    ui->lineEditLightX_2->setText(QString::number(l.x2));
                    ui->lineEditLightY_2->setText(QString::number(l.y2));
                    ui->spinBoxLightAmount->setValue(l.amount);
                    ui->comboBoxLightType->setCurrentIndex(1);
                    on_comboBoxLightType_activated("Line"); // включение некоторых полей
                }
                else
                {
                    ui->comboBoxLightType->setCurrentIndex(0);
                    on_comboBoxLightType_activated("Single"); // выключение некоторых полей

                }
                return;
            }
        }
    }*/

}



/*
void Window::on_comboBoxLightType_activated(const QString &arg1) //при изменении типа источника света
{
    if(arg1 == "Single")
    {
        //выключаем поля, которые не соответсвуют выбраному типу объекта
        ui->lineEditLightX_2->setEnabled(false);
        ui->lineEditLightY_2->setEnabled(false);
        ui->spinBoxLightAmount->setEnabled(false);

        return;
    }

    if(arg1 == "Line")
    {
        //включаем поля которые соответсвуют выбраному объекту
        ui->lineEditLightX_2->setEnabled(true);
        ui->lineEditLightY_2->setEnabled(true);
        ui->spinBoxLightAmount->setEnabled(true);
    }

}
*/
/*
void Window::on_pushButtonLightAdd_clicked() //при нжатии на кнопку добавления ист света
{
    lines.clear();

    // создание временного объекта
    Light added_light;
    added_light.x1 = ui->lineEditLightX->text().toDouble();
    added_light.y1 = ui->lineEditLightY->text().toDouble();

    added_light.name = ui->lineEditLightName->text();
    added_light.power = ui->doubleSpinBoxLightPower->text().toDouble();

    added_light.x2 = ui->lineEditLightX_2->text().toDouble();
    added_light.y2 = ui->lineEditLightY_2->text().toDouble();
    added_light.amount = ui->spinBoxLightAmount->text().toDouble();
    if((ui->comboBoxLightType->currentText() == "Single"))
    {
        added_light.type = false;
    }
    else
    {
        added_light.type = true;
    }

    // проверка на то занято ли имя
    int counter = 0;
    int count = ui->listWidget->count();
    while(counter < count)
    {

        if((items[counter].text().toStdString() == added_light.name.toStdString()))
        {
            QMessageBox::information(this, "Warning", "Names must be different");
            return;
        }
        counter++;
    }

    //добавление в общий список источников света
    lights.append(added_light);

    //добавление объекта в список объектов
    QListWidgetItem* item = new QListWidgetItem(added_light.name, ui->listWidget);
    item->setWhatsThis("Light");
    ui->listWidget->addItem(item);
    items.append(*item);



    //отрисовка с новым источником света
    on_pushButtonCalculate_clicked();
    // корректировка json текста
    ui->plainTextEdit->setPlainText(this->getString());
}
*/

/*
void Window::on_spinBoxLightAmount_valueChanged(int arg1) //будет удалено
{
    //idle
}

*/
void Window::on_doubleSpinBox_valueChanged(double arg1)
{
    is_saved = false;
    mainFrame.first = arg1;
    on_pushButtonCalculate_clicked();
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::on_pushButtonCalculate_2_clicked()
{

    typedef std::string (*Function_Name)(std::string);
    Function_Name function_name = (Function_Name) lib.resolve("system_run");
    QString input;
    if(function_name)
    {
        std::string* text_output = new std::string(this->getString().toStdString());
        input = QString::fromStdString(function_name(*text_output));
        lines.clear();
        QJsonDocument jDoc = QJsonDocument::fromJson(input.toUtf8());
        QJsonArray jArr_lines = jDoc.array();

        foreach(QJsonValue jValue, jArr_lines)
        {
            QJsonObject jLine = jValue.toObject();
            Line temp;
            temp.fromJsonObject(jLine);
            lines.append(temp);

        }
        on_pushButtonCalculate_clicked();
        ui->plainTextEdit->setPlainText(this->getString());
    }
    else
    {
        QMessageBox::warning(this, "Warning", "Cannot find function! Probably dll is missing");
    }

    //отправка данных на сервер
    //Server server(this);
}

void Window::on_doubleSpinBox_2_valueChanged(double arg1)
{
    is_saved = false;
    mainFrame.second = arg1;
    on_pushButtonCalculate_clicked();
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::removeLines()
{
    is_saved = false;
    this->lines.clear();
    on_pushButtonCalculate_clicked();
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::removeAll()
{
    is_saved = false;
    //работа с объектами
    lines.clear();
    //rects.clear();
    lens.clear();
    pixels.clear();

    on_actionAll_triggered();
    items.clear();
    ui->listWidget->clear();
    ui->plainTextEdit->setPlainText(this->getString());
}

QString Window::getString(bool flag)
{
    if(!flag)
    {
        return "";
    }
    QJsonObject jObj;
    //вставка границ экрана
    QJsonObject obj;
    obj.insert("dx", QJsonValue::fromVariant(mainFrame.first));
    obj.insert("dy", QJsonValue::fromVariant(mainFrame.second));
    obj.insert("dz", QJsonValue::fromVariant(mainFrameZ));
    jObj.insert("boarders",obj);

    //вставка параметров экрана
    QJsonObject screenObj;
    screenObj.insert("bright", QJsonValue::fromVariant(ui->spinBoxScreenBrightLevel->value()));
    screenObj.insert("pos", QJsonValue::fromVariant(ui->doubleSpinBoxScreenPos->value()));
    screenObj.insert("size", QJsonValue::fromVariant(ui->spinBoxScreenSize->text().toInt()));
    jObj.insert("screen", screenObj);

    //вставка отрезков
    QJsonArray jArrLines;
    foreach(Line l, lines)
    {
        jArrLines.append(l.toJsonObject());
    }
    jObj.insert("line",jArrLines);


    /*//вставка прямоугольников
    QJsonArray jArrRect;
    foreach(Rect r, rects)
    {
        jArrRect.append(r.toJsonObject());
    }
    jObj.insert("screen", jArrRect);
    */
    //вставка линз
    QJsonArray jArrLens;

    foreach(Lens ls, lens)
    {
        jArrLens.append(ls.toJsonObject());
    }

    jObj.insert("lens", jArrLens);

    //вставка источников света

    QJsonArray jArrPixel;
    //обновление пикселей
    pixels.clear();
    int amount = secondScreen.pixels.size();
    for(int i = 0; i < amount; i++)
    {
        for(int j = 0; j < amount; j++)
        {
            pixels.push_back(secondScreen.pixels.at(i).at(j));
        }
    }

//
    foreach(Pixel l, pixels)
    {
        jArrPixel.append(l.toJsonObject());
    }


    jObj.insert("sour", jArrPixel);

    QJsonDocument jDoc(jObj);

    QString text;
    text = jDoc.toJson(QJsonDocument::Compact);

    return text;
}




//
/*
void Window::send(bool flag)
{
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
    on_pushButtonCalculate_clicked();
    std::unique_ptr<QTcpSocket> socket = std::unique_ptr<QTcpSocket>(new QTcpSocket(this));
    //std::unique_ptr<QTcpServer> server = std::unique_ptr<QTcpServer>(new QTcpServer(this));


    QString self = "127.0.0.1";

    QString ip = self;
    size_t port = 3333;

    try
    {
        socket.get()->connectToHost(ip, port);
        if(!socket->waitForConnected(10000))
        {
            QMessageBox::information(this, "Information", "Connection lost, please rerun the programm");
        }
        else
        {
            QString text = this->getString(flag);
            text = text + "!";

            socket.get()->write(text.toUtf8());

            if(!socket.get()->waitForBytesWritten(1000))
            {
                QMessageBox::information(this, "Information", "Data haven't send");
                return;
            }
        }

    }
    catch(...)
    {
        qDebug() << "Connection failed";
    }

    try
    {

        //настройка окна загрузки
        int amount = lens.length() * lights.length() * 150;

        std::cout << 1 << std::endl;
        ui->progressBar->setVisible(true);

        //server.get()->listen(QHostAddress::Any, 3333);

        //server.get()->waitForNewConnection(1000);

        //if(!server.get()->hasPendingConnections())
        //{
        //   return;
        //}

        //std::unique_ptr<QTcpSocket> new_socket = std::unique_ptr<QTcpSocket>(server.get()->nextPendingConnection());

        while(socket.get()->waitForBytesWritten(1000))
        {

            std::cout << 4 << std::endl;
            //чтение сокета
            QString data(socket->readAll());
            if(data == "!")
            {
                return;
            }
            std::cout << data.toStdString() << std::endl;
            QStringList temp = data.split(" ");
            if(temp.length() < 4)
            {
                continue;
            }
            Line l;
            bool is_nan_i = false;
            //добавление линии
            l.coord1.first = temp.at(0).toDouble();
            if(std::isnan(l.coord1.first))
                is_nan_i = true;
            l.coord1.second = temp.at(1).toDouble();
            if(std::isnan(l.coord1.second))
                is_nan_i = true;
            l.coord2.first = temp.at(2).toDouble();
            if(std::isnan(l.coord2.first))
                is_nan_i = true;
            l.coord2.second = temp.at(3).toDouble();
            if(std::isnan(l.coord2.second))
                is_nan_i = true;
            if(!is_nan_i)
            {
                lines.append(l);
                QPen pen;
                pen.setWidth(0);
                scene.get()->addLine(l.coord1.first, l.coord1.second, l.coord2.first, l.coord2.second);

                if ((double(lines.length()) / double(amount)) >= 0.9)
                {
                    ui->progressBar->setValue(92);
                }
                else
                {
                    ui->progressBar->setValue(int(double(lines.length())/ double(amount) * 100));
                }
            }

        }
        ui->progressBar->setValue(100);
        ui->progressBar->setVisible(false);
    }
    catch(...)
    {
        qDebug() << "Lines didn't recieve";
    }
}





Server::Server(Window* i_w, bool flag_i, QObject *parent) : QObject(parent), flag(flag_i), w(i_w)
{
    // инициализация окна
    w->lines.clear();
    w->ui->plainTextEdit->setPlainText(w->getString());
    // создание сокета для отправки
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    bool connected = false;
    // подкдючение к вычислительной программе
    try // пофиксить
    {
        QString self = "127.0.0.1";
        socket->connectToHost(self, 3333);
        if(socket->waitForConnected(1000))
        {
            connected = true;
        }
        else
        {
            QMessageBox::information(w, "Information", "Connection lost, please rerun the programm");
        }
    }
    catch(...)
    {
        //idle
    }

    // создание сервера
    server = new QTcpServer(this);

    int wait_time = 1000;

    // прослушивание всех доступных ип с порта 3334
    server->listen(QHostAddress::Any, 3333);

    if((server->waitForNewConnection(wait_time)) && connected)
    {
        w->on_pushButtonClear_clicked();
        w->on_pushButtonCalculate_clicked();

        //настройка окна загрузки
        int amount = w->lens.length() * w->lights.length() * 150;
        w->ui->progressBar->setVisible(true);

        do
        {
            // получение первого сокета
            socket = server->nextPendingConnection();
            // если сокет можно прочитать
            if(socket->waitForReadyRead(wait_time))
            {
                try
                {

                    //чтение сокета
                    QString data(socket->readAll());

                    std::cout << data.toStdString() << std::endl;
                    QStringList temp = data.split(" ");
                    Line l;
                    bool is_nan_i = false;
                    //добавление линии
                    l.coord1.first = temp.at(0).toDouble();
                    if(std::isnan(l.coord1.first))
                        is_nan_i = true;
                    l.coord1.second = temp.at(1).toDouble();
                    if(std::isnan(l.coord1.second))
                        is_nan_i = true;
                    l.coord2.first = temp.at(2).toDouble();
                    if(std::isnan(l.coord2.first))
                        is_nan_i = true;
                    l.coord2.second = temp.at(3).toDouble();
                    if(std::isnan(l.coord2.second))
                        is_nan_i = true;
                    if(!is_nan_i)
                    {
                        w->lines.append(l);
                        if ((double(w->lines.length()) / double(amount)) >= 0.9)
                        {
                            w->ui->progressBar->setValue(92);
                        }
                        else
                        {
                            w->ui->progressBar->setValue(int(double(w->lines.length())/ double(amount) * 100));
                        }
                    }

                }
                catch(...)
                {
                    //idle
                }

            }
            else
            {
                return;
            }

            server->waitForNewConnection(5000);

        }
        while(server->hasPendingConnections());

        w->ui->progressBar->setValue(100);

        QPen pen;
        pen.setWidth(0);
        foreach(Line l, w->lines)
        {
            w->scene->addLine(l.coord1.first, l.coord1.second, l.coord2.first, l.coord2.second, pen);
        }
        w->ui->progressBar->setVisible(false);
        w->ui->progressBar->setValue(100);
    }
}

//действия при подключении к вычислительной программе
void Server::connected()
{

    // отправка места нахождения с файлом сохранения
    //QString data = w->currentFile;
    //data = data + "!";
    //socket->write(data.toUtf8());

    QString text = w->getString(flag);
    text = text + "!";

    socket->write(text.toUtf8());


    std::cout << text.toStdString() << std::endl;
    if(!socket->waitForBytesWritten(1000))
    {
        QMessageBox::information(w, "Information", "Data haven't send");
        return;
    }

}
*/
void Window::on_actionAll_triggered()
{
    scene.get()->clear();
}

void Window::on_actionLines_triggered()
{
    on_pushButtonCalculate_clicked();
}

void Window::on_actionDraw_triggered()
{
    on_pushButtonCalculate_clicked();
}

void Window::on_actionCreate_triggered()
{
    secondScreen.setVisible(true);
}

void Window::on_actionAdd_Picture_triggered()
{
    ui->plainTextEdit->setPlainText(this->getString());
    thirdScreen.quantity = secondScreen.pixels.size();
}

void Window::on_doubleSpinBoxScreenPos_valueChanged(double arg1)
{
    is_saved = false;
    thirdScreen.position = arg1;
    screenPosition = arg1;
    on_pushButtonCalculate_clicked();
    ui->plainTextEdit->setPlainText(this->getString());
    is_saved = false;
}

void Window::on_spinBoxScreenBrightLevel_valueChanged(int arg1)
{
    is_saved = false;
    thirdScreen.bright_level = arg1;
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::on_actionShow_Result_triggered()
{
    thirdScreen.size = ui->spinBoxScreenSize->text().toInt();
    thirdScreen.calculate(lines);
    thirdScreen.setVisible(true);
}


void Window::on_pushButton_clicked()
{
    Line temp;
    temp.x1 = 0;
    temp.x2 = 500;
    temp.y1 = 0;
    temp.y2 = 20;
    temp.z1 = 0;
    temp.z2 = 300;
    temp.rgb["b"] = 255;
    lines.append(temp);
    ui->plainTextEdit->setPlainText(this->getString());
    on_pushButtonCalculate_clicked();
}



void Window::on_spinBoxScreenSize_valueChanged(int arg1)
{
    on_pushButtonCalculate_clicked();
}

void Window::on_doubleSpinBox_3_valueChanged(double arg1)
{
    is_saved = false;
    mainFrameZ = arg1;
    on_pushButtonCalculate_clicked();
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
}
