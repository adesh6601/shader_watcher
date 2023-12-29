#include "stdafx.h"
#include "Visualizer.h"
#include "OpenGLWindow.h"
#include "Point3D.h"
#include "Line.h"
#include "Grid.h"
#include "vector"


Visualizer::Visualizer(QWindow* parent) : QMainWindow(nullptr)
{
    setupUi();
    connect(mPushButton1, &QPushButton::clicked, this, &Visualizer::addGrid);
    //connect(mPushButton2, &QPushButton::clicked, this, &Visualizer::addLine);
    connect(mPushButton3, &QPushButton::clicked, this, &Visualizer::simpleDDALine);
    connect(mPushButton4, &QPushButton::clicked, this, &Visualizer::symmetricDDALine);
    connect(mPushButton5, &QPushButton::clicked, this, &Visualizer::bazier);
    connect(mPushButton6, &QPushButton::clicked, this, &Visualizer::hermite);
    connect(mRenderer, SIGNAL(shapeUpdate()), mRenderer, SLOT(update()));
}

Visualizer::~Visualizer()
{

}


//Implimentation of UI
void Visualizer::setupUi()
{

    resize(1530, 785);
    mLabel1 = new QLabel(this);
    mLabel1->setObjectName("label");
    mLabel1->setText("Grid Size");
    mLabel1->setGeometry(QRect(125, 30, 85, 20));

    mLineEdit1 = new QLineEdit(this);
    mLineEdit1->setObjectName("lineEdit_6");
    mLineEdit1->setGeometry(QRect(105, 55, 85, 20));

    mPushButton1 = new QPushButton(this);
    mPushButton1->setObjectName("pushButton");
    mPushButton1->setText("Draw Grid");
    mPushButton1->setGeometry(QRect(205, 50, 150, 30));

    mLabel2 = new QLabel(this);
    mLabel2->setObjectName("label");
    mLabel2->setText("x1");
    mLabel2->setGeometry(QRect(140, 80, 85, 20));

    mLineEdit2 = new QLineEdit(this);
    mLineEdit2->setObjectName("lineEdit_6");
    mLineEdit2->setGeometry(QRect(105, 105, 85, 20));

    mLabel3 = new QLabel(this);
    mLabel3->setObjectName("label");
    mLabel3->setText("y1");
    mLabel3->setGeometry(QRect(240, 80, 85, 20));

    mLineEdit3 = new QLineEdit(this);
    mLineEdit3->setObjectName("lineEdit_6");
    mLineEdit3->setGeometry(QRect(205, 105, 85, 20));

    mLabel4 = new QLabel(this);
    mLabel4->setObjectName("label");
    mLabel4->setText("x2");
    mLabel4->setGeometry(QRect(340, 80, 85, 20));

    mLineEdit4 = new QLineEdit(this);
    mLineEdit4->setObjectName("lineEdit_6");
    mLineEdit4->setGeometry(QRect(305, 105, 85, 20));

    mLabel5 = new QLabel(this);
    mLabel5->setObjectName("label");
    mLabel5->setText("y2");
    mLabel5->setGeometry(QRect(440, 80, 85, 20));

    mLineEdit5 = new QLineEdit(this);
    mLineEdit5->setObjectName("lineEdit_6");
    mLineEdit5->setGeometry(QRect(405, 105, 85, 20));

    /*mPushButton2 = new QPushButton(this);
    mPushButton2->setObjectName("pushButton");
    mPushButton2->setText("Draw Line");
    mPushButton2->setGeometry(QRect(870, 0, 85, 50));*/

    mPushButton3 = new QPushButton(this);
    mPushButton3->setObjectName("pushButton");
    mPushButton3->setText("Simple DDA");
    mPushButton3->setGeometry(QRect(505, 100, 150, 30));

    mPushButton4 = new QPushButton(this);
    mPushButton4->setObjectName("pushButton");
    mPushButton4->setText("Symmetric DDA");
    mPushButton4->setGeometry(QRect(670, 100, 150, 30));

    mPushButton5 = new QPushButton(this);
    mPushButton5->setObjectName("pushButton");
    mPushButton5->setText("Bazier Curve");
    mPushButton5->setGeometry(QRect(1100, 80, 200, 30));

    mPushButton6 = new QPushButton(this);
    mPushButton6->setObjectName("pushButton");
    mPushButton6->setText("Hermite Curve");
    mPushButton6->setGeometry(QRect(1330, 80, 200, 30));

    mLabel6 = new QLabel(this);
    mLabel6->setObjectName("label");
    mLabel6->setText("x1");
    mLabel6->setGeometry(QRect(1130, 170, 85, 30));

    mLineEdit6 = new QLineEdit(this);
    mLineEdit6->setObjectName("lineEdit_7");
    mLineEdit6->setGeometry(QRect(1100, 200, 85, 30));

    mLabel7 = new QLabel(this);
    mLabel7->setObjectName("label");
    mLabel7->setText("y1");
    mLabel7->setGeometry(QRect(1230, 170, 85, 30));

    mLineEdit7 = new QLineEdit(this);
    mLineEdit7->setObjectName("lineEdit_8");
    mLineEdit7->setGeometry(QRect(1200, 200, 85, 30));

    mLabel8 = new QLabel(this);
    mLabel8->setObjectName("label");
    mLabel8->setText("x2");
    mLabel8->setGeometry(QRect(1130, 270, 85, 30));

    mLineEdit8 = new QLineEdit(this);
    mLineEdit8->setObjectName("lineEdit_9");
    mLineEdit8->setGeometry(QRect(1100, 300, 85, 30));

    mLabel9 = new QLabel(this);
    mLabel9->setObjectName("label");
    mLabel9->setText("y2");
    mLabel9->setGeometry(QRect(1230, 270, 85, 30));

    mLineEdit9 = new QLineEdit(this);
    mLineEdit9->setObjectName("lineEdit_10");
    mLineEdit9->setGeometry(QRect(1200, 300, 85, 30));

    mLabel10 = new QLabel(this);
    mLabel10->setObjectName("label");
    mLabel10->setText("x3");
    mLabel10->setGeometry(QRect(1130, 370, 85, 30));

    mLineEdit10 = new QLineEdit(this);
    mLineEdit10->setObjectName("lineEdit_11");
    mLineEdit10->setGeometry(QRect(1100, 400, 85, 30));

    mLabel11 = new QLabel(this);
    mLabel11->setObjectName("label");
    mLabel11->setText("y3");
    mLabel11->setGeometry(QRect(1230, 370, 85, 30));

    mLineEdit11 = new QLineEdit(this);
    mLineEdit11->setObjectName("lineEdit_12");
    mLineEdit11->setGeometry(QRect(1200, 400, 85, 30));

    mLabel12 = new QLabel(this);
    mLabel12->setObjectName("label");
    mLabel12->setText("x4");
    mLabel12->setGeometry(QRect(1130, 470, 85, 30));

    mLineEdit12 = new QLineEdit(this);
    mLineEdit12->setObjectName("lineEdit_13");
    mLineEdit12->setGeometry(QRect(1100, 500, 85, 30));

    mLabel13 = new QLabel(this);
    mLabel13->setObjectName("label");
    mLabel13->setText("y4");
    mLabel13->setGeometry(QRect(1230, 470, 85, 30));

    mLineEdit13 = new QLineEdit(this);
    mLineEdit13->setObjectName("lineEdit_13");
    mLineEdit13->setGeometry(QRect(1200, 500, 85, 30));



    mRenderer = new OpenGLWindow(QColor(0, 0, 0), this);
    mRenderer->setGeometry(QRect(90, 140, 1000, 600));

    setWindowTitle(QCoreApplication::translate("Simple DDA, Symemmetric DDA", "Simple DDA, Symemmetric DDA", nullptr));

}

void Visualizer::addLine()
{
    float x1 = mLineEdit2->text().toFloat();
    float y1 = mLineEdit3->text().toFloat();
    float x2 = mLineEdit4->text().toFloat();
    float y2 = mLineEdit5->text().toFloat();

    mRenderer->addLine(Point3D(x1, y1), Point3D(x2, y2));
}

void Visualizer::addGrid()
{
    int gridSize = mLineEdit1->text().toInt();
    mRenderer->addGrid(gridSize);
}

void Visualizer::simpleDDALine()
{
    float x1 = mLineEdit2->text().toFloat();
    float y1 = mLineEdit3->text().toFloat();
    float x2 = mLineEdit4->text().toFloat();
    float y2 = mLineEdit5->text().toFloat();

    mRenderer->simpleDDA(Point3D(x1, y1), Point3D(x2, y2));
}

void Visualizer::symmetricDDALine()
{
    float x1 = mLineEdit2->text().toFloat();
    float y1 = mLineEdit3->text().toFloat();
    float x2 = mLineEdit4->text().toFloat();
    float y2 = mLineEdit5->text().toFloat();

    mRenderer->symmetricDDA(Point3D(x1, y1), Point3D(x2, y2));
}

void Visualizer::bazier() {
    float x1 = mLineEdit6->text().toFloat();
    float y1 = mLineEdit7->text().toFloat();
    float x2 = mLineEdit8->text().toFloat();
    float y2 = mLineEdit9->text().toFloat();
    float x3 = mLineEdit10->text().toFloat();
    float y3 = mLineEdit11->text().toFloat();
    float x4 = mLineEdit12->text().toFloat();
    float y4 = mLineEdit13->text().toFloat();

    std::vector<Point3D> v;
    v.push_back(Point3D(x1, y1));
    v.push_back(Point3D(x2, y2));
    v.push_back(Point3D(x3, y3));
    v.push_back(Point3D(x4, y4));

    mRenderer->bazierCurve(v);
}

void Visualizer::hermite() {
    float x1 = mLineEdit6->text().toFloat();
    float y1 = mLineEdit7->text().toFloat();
    float x2 = mLineEdit8->text().toFloat();
    float y2 = mLineEdit9->text().toFloat();
    float x3 = mLineEdit10->text().toFloat();
    float y3 = mLineEdit11->text().toFloat();
    float x4 = mLineEdit12->text().toFloat();
    float y4 = mLineEdit13->text().toFloat();

    std::vector<Point3D> v;
    v.push_back(Point3D(x1, y1));
    v.push_back(Point3D(x2, y2));
    v.push_back(Point3D(x3, y3));
    v.push_back(Point3D(x4, y4));

    mRenderer->hermiteCurve(v);
}
