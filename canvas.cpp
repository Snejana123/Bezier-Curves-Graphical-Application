#include "canvas.h"
#include <math.h>
#include <QGraphicsItem>
#include <QMessageBox>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(300, 400);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainGrView = new QGraphicsView;
    mainGrView->setStyleSheet("background: white;");
    mainScene = new QGraphicsScene;
    mainGrView->setScene(mainScene);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(mainGrView);
    //calcMaxXY();
    maxX = 100;
    maxY = 100;
    drawCoordinateSystemFigurse();
    setLayout(mainLayout);

    height = mainGrView->height();
    width = mainGrView->width();
    coordPen = QPen(QBrush(Qt::black), 3);
    linesPen = QPen(QBrush(Qt::gray), 2);
    minLinesPen = QPen(QBrush(Qt::gray), 1);
    circlePen = QPen(QBrush(Qt::darkGreen), 3);
    dotePen = QPen(QBrush(Qt::red), 3);
    smDotePen = QPen(QBrush(Qt::blue), 1);
    linePen = QPen(QBrush(Qt::blue), 3);
    noBrushF = QBrush(Qt::NoBrush);
    doteBrush = QBrush(Qt::red);
    smDoteBrush = QBrush(Qt::blue);


    mesNum = 10;




}

void Canvas::removeAllFigures(){
     circleVector.clear();
     doteVector.clear();
     lineVector.clear();
     drawCoordinateSystemFigurse();
}

void Canvas::addCircle(qreal x, qreal y, qreal radius){
    circleVector.append(QVector3D(x,y,radius));
    calcMaxXY();
}

void Canvas::addDote(qreal x1, qreal y1){
    doteVector.append(QVector2D(x1, y1));
    calcMaxXY();
}

void Canvas::changeDote(int num, QVector2D dote){
    if (num <= doteVector.length()){
        doteVector[num-1].setX(dote.x());
        doteVector[num-1].setY(dote.y());
        calcMaxXY();
    }else{
        QMessageBox::warning(this, tr("Graph Application"),
                             tr("Invalid changing dote number."));
    }
}

void Canvas::addLine(qreal x1, qreal y1, qreal x2, qreal y2){
    lineVector.append(QVector4D(x1,y1,x2,y2));
    calcMaxXY();
}

void Canvas::deleteDote(int num){
    if (num <= doteVector.length()){
        doteVector.removeAt(num-1);
        calcMaxXY();
    }else{
        QMessageBox::warning(this, tr("Graph Application"),
                             tr("Invalid deleting dote number."));
    }
}

void Canvas::drawCoordinateSystemFigurse(){
    delete mainScene;
    mainScene = new QGraphicsScene;
    mainGrView->setScene(mainScene);
    height = mainGrView->height();
    width = mainGrView->width();
    qreal xMid = width / 2;
    qreal yMid = height / 2;
    int step = int((height / 35) < (width / 35) ? (height / 35)/2 : (width / 35)/2);
    mesNum = maxX / width  > maxY / height  ? maxX /(width / 72) : maxY / (height / 72);

    /*for (qreal stepCoord = fracStep / 2 * 25; stepCoord <= width; stepCoord += 25){
        mainScene->addLine(stepCoord, 0, stepCoord, height, linesPen);
    }*/
    int num = 1;
    for (qreal xCoord = xMid + 7; xCoord <= width; xCoord += 7){
        if (num % 5 != 0){
            mainScene->addLine(xCoord, 0, xCoord, height, minLinesPen);
            num += 1;
        }else{
            mainScene->addLine(xCoord, 0, xCoord, height, linesPen);
            num += 1;
        }
    }
    num = 1;
    for (qreal xCoord = xMid - 7; xCoord >= 0; xCoord -= 7){
        if (num % 5 != 0){
            mainScene->addLine(xCoord, 0, xCoord, height, minLinesPen);
            num += 1;
        }else{
            mainScene->addLine(xCoord, 0, xCoord, height, linesPen);
            num += 1;
        }
    }
    num = 1;
    for (qreal yCoord = yMid + 7; yCoord <= height; yCoord += 7){
        if (num % 5 != 0){
            mainScene->addLine(0, yCoord, width, yCoord, minLinesPen);
            num += 1;
        }else{
            mainScene->addLine(0, yCoord, width, yCoord, linesPen);
            num += 1;
        }
    }
    num = 1;
    for (qreal yCoord = yMid - 7; yCoord >= 0; yCoord -= 7){
        if (num % 5 != 0){
            mainScene->addLine(0, yCoord, width, yCoord, minLinesPen);
            num += 1;
        }else{
            mainScene->addLine(0, yCoord, width, yCoord, linesPen);
            num += 1;
        }
    }
    float xText = 0;
    QString curText;
    QGraphicsTextItem *textIt;
    QFont numFont = QFont ("Times", 8, 700);
    //numFont.setWeight(4);
    textIt = mainScene->addText(curText.setNum(xText, 'g', 4), numFont);
    textIt->setPos(width/2, height/2 + 5);
    for (qreal xCoord = xMid + 35; xCoord <= width; xCoord += 35){
        xText += mesNum;
        textIt = mainScene->addText(curText.setNum(xText, 'g', 4), numFont);
        textIt->setPos(xCoord, height/2 + 5);
    }
    xText = 0 - mesNum;
    for (qreal xCoord = xMid - 35; xCoord >= 0; xCoord -= 35){
        textIt = mainScene->addText(curText.setNum(xText, 'g', 4), numFont);
        textIt->setPos(xCoord, height/2 + 5);
        xText -= mesNum;
    }
    xText = - mesNum;
    for (qreal yCoord = yMid + 35; yCoord <= height; yCoord += 35){
        textIt = mainScene->addText(curText.setNum(xText, 'g', 4), numFont);
        textIt->setPos(width/2 + 5, yCoord);
        xText -= mesNum;
    }
    xText = mesNum;
    for (qreal yCoord = yMid - 35; yCoord >= 0; yCoord -= 35){
        textIt = mainScene->addText(curText.setNum(xText, 'g', 4), numFont);
        textIt->setPos(width/2 + 5, yCoord);
        xText += mesNum;
    }
    mainScene->addLine(0, height/2, width, height/2, coordPen);
    mainScene->addLine(width/2, 0, width/2, height, coordPen);
    if (height/70 < 10){
        mainScene->addLine(width/2, 0, width/2 + height/70, 0 + height/70 * 2, coordPen);
        mainScene->addLine(width/2, 0, width/2 - height/70, 0 + height/70 * 2, coordPen);
        //mainScene->addLine(width/2, height, width/2 + height/70, height - height/70 * 2, coordPen);
        //mainScene->addLine(width/2, height, width/2 - height/70, height - height/70 * 2, coordPen);
        //mainScene->addLine(0, height/2, height/70 * 2, height/2 + height/70, coordPen);
        //mainScene->addLine(0, height/2, height/70 * 2, height/2 - height/70, coordPen);
        mainScene->addLine(width, height/2, width - height/70 * 2, height/2 + height/70, coordPen);
        mainScene->addLine(width, height/2, width - height/70 * 2, height/2 - height/70, coordPen);
    }else{
        mainScene->addLine(width/2, 0, width/2 + 10, 20, coordPen);
        mainScene->addLine(width/2, 0, width/2 - 10, 20, coordPen);
        //mainScene->addLine(width/2, height, width/2 + 10, height - 20, coordPen);
        //mainScene->addLine(width/2, height, width/2 - 10, height - 20, coordPen);
        //mainScene->addLine(0, height/2, 20, height/2 + 10, coordPen);
        //mainScene->addLine(0, height/2, 20, height/2 - 10, coordPen);
        mainScene->addLine(width, height/2, width - 20, height/2 + 10, coordPen);
        mainScene->addLine(width, height/2, width - 20, height/2 - 10, coordPen);
    }
    //int mesNumX = int(xMid / 7), mesNumY = int(yMid / 7);
    for (int i = 0; i < circleVector.length(); i++){
        QVector3D curCirc = circleVector.at(i);
        drawCircle(curCirc.x(), curCirc.y(), curCirc.z());
    }
    for (int i = 0; i < lineVector.length(); i++){
        QVector4D curLine = lineVector.at(i);
        drawLine(curLine.x(), curLine.y(), curLine.z(), curLine.w());
    }
    for (int i = 0; i < doteVector.length(); i++){
        QVector2D curDote = doteVector.at(i);
        drawDote(curDote.x(), curDote.y());
    }
    calcDraw();
    emit ready();
}



void Canvas::calcDraw(){
    QVector2D newBezDote;
    float x, y;
    for(int i = 1; i + 2 < doteVector.length(); i+= 3){
        if (doteVector.length() - i >= 3){
            float max = (width > height) ? width : height;
            float t_step = 1 / max;
            for (float t = 0; t <=1; t += t_step){
                float invt = 1- t;
                x = pow(invt,3) * doteVector[i - 1].x() + 3 * pow(invt,2) * t * doteVector[i].x() +3* invt * pow(t,2) * doteVector[i+1].x() + pow(t,3) * doteVector[i+2].x();
                y = pow(invt,3) * doteVector[i - 1].y() + 3 * pow(invt,2) * t * doteVector[i].y() +3* invt * pow(t,2) * doteVector[i+1].y() + pow(t,3) * doteVector[i+2].y();
                newBezDote.setX(x);
                newBezDote.setY(y);
                drawSmallDote(x,y);
            }
        }
    }
}


void Canvas::drawCircle(qreal x, qreal y, qreal radius){
    mainScene->addEllipse(width/2 + ((x - radius) / mesNum * 35), height/2 - (y + radius) / mesNum * 35  , 2 * radius / mesNum * 35, 2 * radius / mesNum * 35, circlePen, noBrushF);
}

void Canvas::drawLine(qreal x1, qreal y1, qreal x2, qreal y2){
    mainScene->addLine(width/2 + x1 / mesNum * 35, height/2 - y1 / mesNum * 35, width/2 + x2 / mesNum * 35, height/2 - y2 / mesNum * 35, linePen);
}

void Canvas::drawDote(qreal x, qreal y){
    mainScene->addEllipse(width/2 + (x / mesNum * 35) - 2, height/2 - y / mesNum * 35 - 2 , 4, 4, dotePen, doteBrush);
}


void Canvas::drawSmallDote(qreal x, qreal y){
    mainScene->addEllipse(width/2 + (x / mesNum * 35) - 2, height/2 - y / mesNum * 35 - 2 , 4, 4, smDotePen, smDoteBrush);
}

void Canvas::setMaxXY(qreal maxXVal, qreal maxYVal){
    maxX = maxXVal;
    maxY = maxYVal;
}

void Canvas::calcMaxXY(){
    float x = 0;
    for (int i =0; i < doteVector.length(); i++){
        if (abs(doteVector[i].x()) > x){
            x = abs(doteVector[i].x());
        }
    }
    for (int i =0; i < lineVector.length(); i++){
        if (abs(lineVector[i].x()) > x){
            x = abs(lineVector[i].x());
        }
        if (abs(lineVector[i].z()) > x){
            x = abs(lineVector[i].z());
        }
    }
    for (int i =0; i < circleVector.length(); i++){
        if (abs(circleVector[i].x()) + circleVector[i].z() > x){
            x = abs(circleVector[i].x()) + circleVector[i].z();
        }
    }
    float y = 0;
    for (int i =0; i < doteVector.length(); i++){
        if (abs(doteVector[i].y()) > y){
            y = abs(doteVector[i].y());
        }
    }
    for (int i =0; i < lineVector.length(); i++){
        if (abs(lineVector[i].y()) > y){
            y = abs(lineVector[i].y());
        }
        if (abs(lineVector[i].z()) > y){
            y = abs(lineVector[i].w());
        }
    }
    for (int i =0; i < circleVector.length(); i++){
        if (abs(circleVector[i].y()) + circleVector[i].z() > y){
            y = abs(circleVector[i].y()) + circleVector[i].z();
        }
    }
    maxX = x;
    maxY = y;
}

/*void Canvas::paintEvent(QPaintEvent*)
{
    QPainter p;
    p.begin(this);
    p.fillRect(0,0, width(), height(), QBrush(Qt::white));
    s->draw(&p,rect(),Qt::green);
    p.end();
}*/


void Canvas::resizeEvent(QResizeEvent *)
{
    delete mainScene;
    mainScene = new QGraphicsScene;
    mainGrView->setScene(mainScene);
    drawCoordinateSystemFigurse();
}


void Canvas::exportPng(QString filename){
    /*QImage image("drawing");
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    mainScene->render(&painter);
    image.save(filename);*/

    QImage img(this->width,this->height,QImage::Format_ARGB32_Premultiplied);
    QPainter p;
    p.begin(&img);
    p.fillRect(0,0, this->width, this->height, QBrush(Qt::white));
    //s->draw(&p,rect(),Qt::green);
    mainScene->render(&p);
    p.end();
    img.save(filename);
}


int Canvas::getDoteCount(){
    return doteVector.length();
}


Canvas::~Canvas(){
    delete mainLayout;
    delete mainScene;
    delete mainGrView;

}
