#include "window.hpp"
#include "ui_window.h"
#include <iostream>

QString Window::getString()
{
    QJsonObject jObj;
    //вставка границ экрана
    QJsonObject obj;
    obj.insert("dx", QJsonValue::fromVariant(mainFrame.first));
    obj.insert("dy", QJsonValue::fromVariant(mainFrame.second));
    jObj.insert("boarders",obj);

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

    QJsonArray jArrLight;

    foreach(Light l, lights)
    {
        jArrLight.append(l.toJsonObject());
    }

    jObj.insert("sour", jArrLight);

    QJsonDocument jDoc(jObj);

    QString text;
    text = jDoc.toJson(QJsonDocument::Compact);

    return text;
}


// SERVER

Server::Server(Window* i_w, QObject *parent) : QObject(parent)
{
    // инициализация окна
    w = i_w;
    w->lines.clear();
    w->ui->plainTextEdit->setPlainText(w->getString());
    // создание сокета для отправки
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    bool connected = false;
    // подкдючение к вычислительной программе
    try
    {
        socket->connectToHost("192.168.0.105", 3333);
        if(socket->waitForConnected(1000))
        {
            connected = true;
        }
    }
    catch(...)
    {
        //idle
    }

    // создание сервера
    server = new QTcpServer(this);

    int wait_time = 1000;

    // прослушивание всех доступных ип с порта 3333
    server->listen(QHostAddress::Any, 3333);


    if((server->waitForNewConnection(wait_time)) && connected)
    {
        w->on_pushButtonClear_clicked();
        w->on_pushButtonCalculate_clicked();

        do
        {
            // получение первого сокета
            socket = server->nextPendingConnection();
            // если сокет можно прочитать
            if(socket->waitForReadyRead(wait_time))
            {
                //чтение сокета
                QString data(socket->readAll());
                QStringList temp = data.split(" ");
                Line l;
                //добавление линии
                l.coord1.first = temp.at(0).toDouble();
                l.coord1.second = temp.at(1).toDouble();
                l.coord2.first = temp.at(2).toDouble();
                l.coord2.second = temp.at(3).toDouble();
                w->lines.append(l);
            }
            else
            {
                return;
            }

            server->waitForNewConnection(100);

        }
        while(server->hasPendingConnections());


        QPen pen;
        pen.setWidth(0);
        foreach(Line l, w->lines)
        {
            w->scene->addLine(l.coord1.first, l.coord1.second, l.coord2.first, l.coord2.second, pen);
        }
    }


}

//действия при подключении к вычислительной программе
void Server::connected()
{

    // отправка места нахождения с файлом сохранения
    //QString data = w->currentFile;
    //data = data + "!";
    //socket->write(data.toUtf8());

    QString text = w->getString();
    text = text + "!";

    socket->write(text.toUtf8());

    if(!socket->waitForBytesWritten(1000))
    {
        QMessageBox::information(w, "Information", "Data haven't send");
        return;
    }

}




//WINDOW

Window::Window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Window)
{


    ui->setupUi(this);
    setWindowTitle("TheProgramm");
    setWindowIcon(QIcon(":/resources/data/Logo.png"));
    connect(ui->actionNew, &QAction::triggered, this, &Window::newDocument);
    connect(ui->actionLoad, &QAction::triggered, this, &Window::load);
    connect(ui->actionSave, &QAction::triggered, this, &Window::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &Window::save);
    connect(ui->actionSave, &QAction::triggered, this, &Window::save);


    //дополнительные действия
    // выключение некоторых полей
    on_comboBoxLightType_activated("Single");

    currentFile = "C:/Users/Nick/desktop/save";
    load();
}

Window::~Window()
{
    delete ui;
}

//Функции вкладки File
void Window::save()
{
    QString fileName;
    if(currentFile.isEmpty())
    {
        fileName = QFileDialog::getSaveFileName(this, "Save");
        currentFile = fileName;
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

    setWindowTitle(fileName);


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
        reply = QMessageBox::question(this, "Wrning", "Would you like to save?", QMessageBox::Yes|QMessageBox::No);
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
    lights.clear();
    items.clear();
    ui->listWidget->clear();
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //считвание файла
    QTextStream in(&file);
    QString text = in.readAll();
    QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject jObj = jDoc.object();

    QJsonArray jArr_lines = jObj.value("line").toArray();
    //QJsonArray jArr_rects = jObj.value("screen").toArray();
    QJsonArray jArr_lens = jObj.value("lens").toArray();
    QJsonArray jArr_lights = jObj.value("sour").toArray();

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

    // добавление источников света
    foreach(QJsonValue jValue, jArr_lights)
    {
        QJsonObject jLight = jValue.toObject();
        Light temp;
        temp.fromJsonObject(jLight);
        lights.append(temp);

        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Light");
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
    is_saved = false;
    //работа с объектами
    lines.clear();
    //rects.clear();
    lens.clear();
    lights.clear();

    on_pushButtonClearScreen_clicked();
    items.clear();
    ui->listWidget->clear();
    ui->plainTextEdit->clear();
}

void Window::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as");
    currentFile = fileName;
    save();
}


void Window::on_pushButtonTextEnter_clicked()
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

    QString text;
    text = ui->plainTextEdit->toPlainText();
    lines.clear();
    //rects.clear();
    lens.clear();
    lights.clear();
    items.clear();
    QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8());
    QJsonObject jObj = jDoc.object();

    // добавление линий
    QJsonArray jArr_lines = jObj.value("line").toArray();

    foreach(QJsonValue jValue, jArr_lines)
    {
        QJsonObject jLine = jValue.toObject();
        Line temp;
        temp.fromJsonObject(jLine);
        lines.append(temp);
    }

    /*// добавление прямоугольников
    QJsonArray jArr_rects = jObj.value("rectangle").toArray();

    foreach (QJsonValue jValue, jArr_rects)
    {
        QJsonObject jRect = jValue.toObject();
        Rect temp;
        temp.fromJsonObject(jRect);
        rects.append(temp);

        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Rectangle");
        ui->listWidget->addItem(item);
        items.append(*item);
    }*/

    //добавление линз
    QJsonArray jArr_lens = jObj.value("lens").toArray();

    foreach (QJsonValue jValue, jArr_lens)
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
    //добавление источников света

    QJsonArray jArr_light = jObj.value("light").toArray();

    foreach (QJsonValue jValue, jArr_light)
    {
        QJsonObject jLight = jValue.toObject();
        Light temp;
        temp.fromJsonObject(jLight);
        lights.append(temp);

        QListWidgetItem* item = new QListWidgetItem(temp.name, ui->listWidget);
        item->setWhatsThis("Light");
        ui->listWidget->addItem(item);
        items.append(*item);
    }

    is_saved = false;

    on_pushButtonCalculate_clicked();

}

void Window::on_pushButtonClear_clicked()
{
    ui->plainTextEdit->clear();
    is_saved = false;
}

void Window::on_pushButtonCalculate_clicked()
{
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    scene->addRect(-mainFrame.first, - mainFrame.second, 2 * mainFrame.first, 2 * mainFrame.second);

    QPen pen;
    pen.setWidth(int(current_scale));
    //отрисовка линий
    foreach (Line l, lines) {
        scene->addLine(l.coord1.first, l.coord1.second, l.coord2.first, l.coord2.second, pen);
    }

    /*//отрисовка прямоугольников
    foreach (Rect r, rects)
    {
        scene->addLine(r.coord1.first, r.coord1.second, r.coord2.first, r.coord2.second, QPen(QColorConstants::Svg::firebrick));
    }*/

    //отрисовка линз
    foreach( Lens l, lens)
    {
        l.draw(scene);

    }

    //отрисовка источников света
    foreach(Light l, lights)
    {
        l.draw(scene);
    }

}

void Window::on_pushButtonClearScreen_clicked() // кнопка очистки экрана
{
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
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
    added_lens.radius = std::pair<double, double>(ui->lineEditLensRadiusLeft->text().toDouble(), ui->lineEditLensRadiusRight->text().toDouble());
    added_lens.length = ui->lineEditLensLength->text().toDouble();
    added_lens.r_index = ui->lineEditLensR_Index->text().toDouble();
    added_lens.name = ui->lineEditLensName->text();
    added_lens.type = "Flat";
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
    if((added_lens.radius.first<=0)||(added_lens.radius.second<=0)||(added_lens.length < 0) || ((added_lens.radius.first+added_lens.radius.second) <= added_lens.length))
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
    ui->listWidget->takeItem(ui->listWidget->row(item));
    //удаление объекта из items
    QList<QListWidgetItem> temp;
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
    }

    //отрисовка без удаленного предмета
    on_pushButtonCalculate_clicked();
    //корректировка json текста
    ui->plainTextEdit->setPlainText(this->getString());

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
    }

}




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

void Window::on_pushButtonLightAdd_clicked() //при нжатии на кнопку добавления линзы
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

    // в начале проверяем какой тип объекта выбран


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



void Window::on_spinBoxLightAmount_valueChanged(int arg1) //будет удалено
{
    //idle
}


void Window::on_doubleSpinBox_valueChanged(double arg1)
{
    mainFrame.first = arg1;
    on_pushButtonCalculate_clicked();
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
}

void Window::on_pushButtonCalculate_2_clicked()
{
    //отправка данных на сервер

    Server new_server(this);

}

void Window::on_doubleSpinBox_2_valueChanged(double arg1)
{
    mainFrame.second = arg1;
    on_pushButtonCalculate_clicked();
    lines.clear();
    ui->plainTextEdit->setPlainText(this->getString());
}
