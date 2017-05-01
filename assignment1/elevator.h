#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <QButtonGroup>
#include <QObject>
#include <iostream>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <cmath>
#include "ui_mainwindow.h"

using namespace std;

class Elevator : public QObject {
    Q_OBJECT
public:
    Elevator(Ui::MainWindow *ui, int eleNumber, QLabel *doorLabel, QLabel *statusLabel,
             QLabel *floorLabel) {
        eleFloor = new QButtonGroup();
        runAndStop = new QButtonGroup();
        display = new QButtonGroup();

        for (int i = 0; i <= 20; i++) {
            isFloorPushed[i] = 0;
            isUpAsked[i] = 0;
            isDownAsked[i] = 0;
        }


        uiCopy = ui;
        this->eleNumber = eleNumber;
        this->doorLabel = doorLabel;
        this->statusLabel = statusLabel;
        this->floorLabel = floorLabel;

        this->nowFloor = 1;
        this->status = 0;
        this->door = 0;


    }

    void eleMove(int i);
    void eleDoor();
    void eleScheduler();
    void checkPushedFloor();
    void collectUpAndDown(QButtonGroup *up, QButtonGroup *down);

public slots:
    void startScheduler();


protected:
    Ui::MainWindow *uiCopy;
    QButtonGroup *eleFloor;
    QButtonGroup *up;
    QButtonGroup *down;
    QButtonGroup *runAndStop;
    QButtonGroup *display;

    //表示电梯内某一楼层是否按下
    int isFloorPushed[21];
    //表示电梯外部某一楼层是否有请求 0为无请求 1为有请求
    int isUpAsked[21];
    int isDownAsked[21];
    //表示是哪一部电梯
    int eleNumber;
    //表示电梯 0不动 1上 2下
    int status;
    //表示电梯的门是否打开 0关闭 1打开
    int door;
    //表示电梯现在是在哪一层楼
    int nowFloor;


    QLabel *doorLabel;
    QLabel *statusLabel;
    QLabel *floorLabel;

private slots:
    void setPushedFloor(int i);
    void setAskedFloor(int status, int i, int number);
    void eleRun(int i);


friend class MainWindow;
};



#endif // ELEVATOR_H
