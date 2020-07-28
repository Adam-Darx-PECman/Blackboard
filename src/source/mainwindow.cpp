#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    electronButton = new QPushButton("Electron");
    photonButton = new QPushButton("Photon");
    vertexButton = new QPushButton("Vertex");
    computeButton = new QPushButton("Compute");
    undoButton = new QPushButton("Undo");
    redoButton = new  QPushButton("Redo");
    cleanButton = new QPushButton("Clean");
    buttonMenu = new QGridLayout;
    featureMenu = new QGridLayout;
    featureMenu->addWidget(undoButton,0,0);
    featureMenu->addWidget(redoButton,0,1);

    buttonMenu->addWidget(electronButton,0,0);
    buttonMenu->addWidget(photonButton,1,0);
    buttonMenu->addWidget(vertexButton,2,0);
    buttonMenu->addWidget(computeButton,3,0);
    buttonMenu->addLayout(featureMenu,4,0);
    buttonMenu->addWidget(cleanButton,5,0);

    QString introduction = "\nBlackboard致力于简化费曼图计算过程\n"
                           "所给出的振幅均为Wolfram Mathematica代码\n"
                           "必需的包: FeynCalc\n"
                           "作者邮箱: 762420244@qq.com\n";
    intro = new QLabel("本软件由 Adam Darx(PECman) 开发\n版本号: 1.0.0\n编译日期: "+QDate().currentDate().toString()+introduction);
    intro->setFont(QFont("Microsoft JhengHei",8));
    intro->adjustSize();
    profile = new QLabel();
    profile->setPixmap(QPixmap(".//img//profile.png"));
    profile->setMaximumWidth(intro->width());
    leftLayout = new QGridLayout;
    leftLayout->addWidget(profile,0,0,Qt::AlignTop);
    leftLayout->addWidget(intro,1,0,Qt::AlignTop);
    leftLayout->addLayout(buttonMenu,2,0,Qt::AlignTop);

    paintArea = new PaintArea;
    computeModel = new ComputeModel;
    rightLayout = new QGridLayout;
    rightLayout->addWidget(paintArea,1,0);
    rightLayout->addWidget(computeModel,0,0);

    mainLayout = new QGridLayout;
    mainLayout->addLayout(leftLayout,0,0);
    mainLayout->addLayout(rightLayout,0,1);

    this->setLayout(mainLayout);
    connect(electronButton, SIGNAL(clicked()), this, SLOT(on_electronButton_triggered()));
    connect(photonButton, SIGNAL(clicked()), this, SLOT(on_photonButton_triggered()));
    connect(vertexButton, SIGNAL(clicked()), this, SLOT(on_vertexButton_triggered()));

    connect(computeButton,SIGNAL(clicked()), paintArea, SLOT(on_compute()));
    connect(paintArea,SIGNAL(compute(QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*)),
            computeModel,SLOT(on_compute(QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*, QList<QPoint>*)));
    connect(computeModel,SIGNAL(on_mark(QList<QList<Particle>>)),paintArea,SLOT(mark(QList<QList<Particle>>)));

    connect(undoButton, SIGNAL(clicked()),paintArea,SLOT(undo()));
    connect(redoButton, SIGNAL(clicked()),paintArea,SLOT(redo()));
    connect(cleanButton, SIGNAL(clicked()), this, SLOT(on_cleanButton_triggered()));
}

void MainWindow::on_photonButton_triggered()
{
    paintArea->setGadget(paintArea->wave);
}

void MainWindow::on_electronButton_triggered()
{
    paintArea->setGadget(paintArea->line);
}

void MainWindow::on_vertexButton_triggered()
{
    paintArea->setGadget(paintArea->dot);
}

void MainWindow::on_cleanButton_triggered()
{
    paintArea->clean();
}
