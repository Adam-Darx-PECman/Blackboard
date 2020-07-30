#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "paintarea.h"
#include "computemodel.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QDate>
#include <QDial>
#include <QSpinBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
protected:
private:
    QPushButton* electronButton;
    QPushButton* electronArcButton;
    QPushButton* photonButton;
    QPushButton* photonArcButton;
    QPushButton* vertexButton;
    QPushButton* computeButton;
    QPushButton* undoButton;
    QPushButton* redoButton;
    QPushButton* cleanButton;
    QDial* dial;
    QSpinBox* sbox;
    QLabel* intro;
    QLabel* profile;
    PaintArea* paintArea;
    ComputeModel* computeModel;

    QGridLayout* mainLayout;
    QGridLayout* leftLayout;
    QGridLayout* rightLayout;

    QGridLayout* electronMenu;
    QGridLayout* photonMenu;
    QGridLayout* buttonMenu;
    QGridLayout* featureMenu;
protected slots:
    void on_electronButton_triggered();
    void on_electronArcButton_triggered();
    void on_photonButton_triggered();
    void on_photonArcButton_triggered();
    void on_vertexButton_triggered();
    void on_cleanButton_triggered();
    void on_angle_changed(int);
    void on_number_changed(int);
};

#endif // MAINWINDOW_H
