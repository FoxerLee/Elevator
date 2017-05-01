#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //初始化的部分

    //初始化两个buttongroup
    this->up = new QButtonGroup();
    this->down = new QButtonGroup();
    //初始化电梯的类
    ele[0] = new Elevator(ui, 0, ui->one_door, ui->one_status, ui->one_floor);
    ele[1] = new Elevator(ui, 1, ui->two_door, ui->two_status, ui->two_floor);
    ele[2] = new Elevator(ui, 2, ui->three_door, ui->three_status, ui->three_floor);
    ele[3] = new Elevator(ui, 3, ui->four_door, ui->four_status, ui->four_floor);
    ele[4] = new Elevator(ui, 4, ui->five_door, ui->five_status, ui->five_floor);
    //关联button
    this->initButtonGroup();
    for (int i = 0; i < 5; i++) {
        ele[i]->collectUpAndDown(this->up, this->down);
        //初始的时候，电梯默认处于ERROR状态，不能运行
        ele[i]->runAndStop->button(1)->setEnabled(false);
        for (int j = 0; j < 20; j++) {
            ele[i]->eleFloor->button(j)->setEnabled(false);
        }
    }
    //初始化其他的标记
    for (int i = 0; i <=20; i++) {
        this->isUpAsked[i] = 0;
        this->isDownAsked[i] = 0;
    }
    //初始化所有层的显示
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 20; j++) {
            ele[i]->display->button(j)->setEnabled(false);
        }
    }
//    //初始化第一层的显示
//    for (int i = 0; i < 5; i++) {
//        ele[i]->display->button(0)->setEnabled(true);
//        ele[i]->display->button(0)->setChecked(true);
//    }

    r = 0;
    QThread *t[5];

    this->checkAskedFloor();

    for (int i = 0; i < 5; i++) {
        t[i] = new QThread();
        ele[i]->moveToThread(t[i]);
        t[i]->start();

        connect(this,SIGNAL(sendStart()),ele[i],SLOT(startScheduler()));
        emit sendStart();
    }





}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::checkAskedFloor() {
    QObject::connect(this->up, SIGNAL(buttonClicked(int)), this, SLOT(setUpFloor(int)));
    QObject::connect(this->down,SIGNAL(buttonClicked(int)), this, SLOT(setDownFloor(int)));
}

void MainWindow::setUpFloor(int i) {
    if(!this->up->button(i)->isChecked()) {
        this->up->button(i)->setChecked(true);
    }
    this->isUpAsked[i+1] = 1;    
    //首先检查是否所有电梯均处于ERROR状态
    if (ele[0]->status == -1 && ele[1]->status == -1 && ele[2]->status == -1 && ele[3]->status == -1 && ele[4]->status == -1) {
        this->up->button(i)->setChecked(false);
        this->isUpAsked[i+1] = 0;
        return;
    }

    int temp = -1;
    int lastEle = 0;
    //选择最优的电梯

    //先查找是否有不动的电梯
    for (int j = 0; j < 5; j++) {
        if (ele[j]->status == 0) {
//            cout << j << " " << ele[j]->nowFloor << " " << lastEle << " " << ele[lastEle]->nowFloor << endl;
            if (abs((i+1) - ele[j]->nowFloor) <= abs((i+1) - ele[lastEle]->nowFloor)) {
                temp = j;
                lastEle = j;
            }
        }

    }


    //有不动的电梯
    if (temp != -1) {
        this->isUpAsked[i+1] = 0;

        QObject::connect(this, SIGNAL(sendFloor(int,int,int)), ele[temp], SLOT(setAskedFloor(int,int,int)));
        emit sendFloor(1, i+1, temp);


    }


    //没有不动的电梯
    else {
        for (int j = 0; j < 5; j++) {
            if(ele[j]->status == 1 && ele[j]->nowFloor < i+1) {
                if (ele[r]->nowFloor < ele[j]->nowFloor) {
                    r = j;
                }
            }
        }
        cout << r << " ";
        //这里有两种可能，一种是找到了一个正在靠近请求楼层的电梯，那么r的值对于该电梯
        //另一种是没有满足条件的电梯，那么则循环地选择一个电梯去响应请求
        this->isUpAsked[i+1] = 0;
        while (ele[r]->run == -1) {
//            cout << r << " ";
            if (r == 4) {
                r = 0;
            }
            else {
               r++;
            }
        }
        cout << endl;
        QObject::connect(this, SIGNAL(sendFloor(int,int,int)), ele[r], SLOT(setAskedFloor(int,int,int)));
        emit sendFloor(1, i+1, r);
        if (r == 4) {
            r = 0;
        }
        else {
           r++;
        }

    }

}

void MainWindow::setDownFloor(int i) {
    this->isDownAsked[i+1] = 1;

    if(!this->down->button(i)->isChecked()) {
        this->down->button(i)->setChecked(true);
    }
    //首先检查是否所有电梯均处于ERROR状态
    if (ele[0]->status == -1 && ele[1]->status == -1 && ele[2]->status == -1 && ele[3]->status == -1 && ele[4]->status == -1) {
        this->down->button(i)->setChecked(false);
        this->isDownAsked[i+1] = 0;
        return;
    }

    int temp = -1;
    int lastEle = 0;
    //选择最优的电梯

    //先查找是否有不动的电梯
    for (int j = 0; j < 5; j++) {
        if (ele[j]->status == 0) {
//            cout << j << " " << ele[j]->nowFloor << " " << lastEle << " " << ele[lastEle]->nowFloor << endl;
            if (abs((i+1) - ele[j]->nowFloor) <= abs((i+1) - ele[lastEle]->nowFloor)) {
                temp = j;
                lastEle = j;
            }
        }
    }

    //有不动的电梯
    if (temp != -1) {
        this->isDownAsked[i+1] = 0;

        QObject::connect(this, SIGNAL(sendFloor(int,int,int)), ele[temp], SLOT(setAskedFloor(int,int,int)));
        emit sendFloor(2, i+1, temp);

    }
    //没有不动的电梯
    else {
        for (int j = 0; j < 5; j++) {
            if(ele[j]->status == 2 && ele[j]->nowFloor > i+1) {
                if (ele[r]->nowFloor > ele[j]->nowFloor) {
                    r = j;
                }
            }
        }
        cout << r << " ";
        //这里有两种可能，一种是找到了一个正在靠近请求楼层的电梯，那么r的值对于该电梯
        //另一种是没有满足条件的电梯，那么则循环地选择一个电梯去响应请求
        this->isDownAsked[i+1] = 0;
        while (ele[r]->run == -1) {
//            cout << r << " ";
            if (r == 4) {
                r = 0;
            }
            else {
               r++;
            }
        }
        cout << endl;
        QObject::connect(this, SIGNAL(sendFloor(int,int,int)), ele[r], SLOT(setAskedFloor(int,int,int)));
        emit sendFloor(2, i+1, r);
        if (r == 4) {
            r = 0;
        }
        else {
           r++;
        }
    }
}

void MainWindow::initButtonGroup() {
    ele[0]->eleFloor->addButton(ui->eve_one_1, 0);
    ele[0]->eleFloor->addButton(ui->eve_one_2, 1);
    ele[0]->eleFloor->addButton(ui->eve_one_3, 2);
    ele[0]->eleFloor->addButton(ui->eve_one_4, 3);
    ele[0]->eleFloor->addButton(ui->eve_one_5, 4);
    ele[0]->eleFloor->addButton(ui->eve_one_6, 5);
    ele[0]->eleFloor->addButton(ui->eve_one_7, 6);
    ele[0]->eleFloor->addButton(ui->eve_one_8, 7);
    ele[0]->eleFloor->addButton(ui->eve_one_9, 8);
    ele[0]->eleFloor->addButton(ui->eve_one_10, 9);
    ele[0]->eleFloor->addButton(ui->eve_one_11, 10);
    ele[0]->eleFloor->addButton(ui->eve_one_12, 11);
    ele[0]->eleFloor->addButton(ui->eve_one_13, 12);
    ele[0]->eleFloor->addButton(ui->eve_one_14, 13);
    ele[0]->eleFloor->addButton(ui->eve_one_15, 14);
    ele[0]->eleFloor->addButton(ui->eve_one_16, 15);
    ele[0]->eleFloor->addButton(ui->eve_one_17, 16);
    ele[0]->eleFloor->addButton(ui->eve_one_18, 17);
    ele[0]->eleFloor->addButton(ui->eve_one_19, 18);
    ele[0]->eleFloor->addButton(ui->eve_one_20, 19);

    ele[0]->runAndStop->addButton(ui->one_run, 0);
    ele[0]->runAndStop->addButton(ui->one_stop, 1);

    ele[1]->eleFloor->addButton(ui->eve_two_1, 0);
    ele[1]->eleFloor->addButton(ui->eve_two_2, 1);
    ele[1]->eleFloor->addButton(ui->eve_two_3, 2);
    ele[1]->eleFloor->addButton(ui->eve_two_4, 3);
    ele[1]->eleFloor->addButton(ui->eve_two_5, 4);
    ele[1]->eleFloor->addButton(ui->eve_two_6, 5);
    ele[1]->eleFloor->addButton(ui->eve_two_7, 6);
    ele[1]->eleFloor->addButton(ui->eve_two_8, 7);
    ele[1]->eleFloor->addButton(ui->eve_two_9, 8);
    ele[1]->eleFloor->addButton(ui->eve_two_10, 9);
    ele[1]->eleFloor->addButton(ui->eve_two_11, 10);
    ele[1]->eleFloor->addButton(ui->eve_two_12, 11);
    ele[1]->eleFloor->addButton(ui->eve_two_13, 12);
    ele[1]->eleFloor->addButton(ui->eve_two_14, 13);
    ele[1]->eleFloor->addButton(ui->eve_two_15, 14);
    ele[1]->eleFloor->addButton(ui->eve_two_16, 15);
    ele[1]->eleFloor->addButton(ui->eve_two_17, 16);
    ele[1]->eleFloor->addButton(ui->eve_two_18, 17);
    ele[1]->eleFloor->addButton(ui->eve_two_19, 18);
    ele[1]->eleFloor->addButton(ui->eve_two_20, 19);

    ele[1]->runAndStop->addButton(ui->two_run, 0);
    ele[1]->runAndStop->addButton(ui->two_stop, 1);

    ele[2]->eleFloor->addButton(ui->eve_three_1, 0);
    ele[2]->eleFloor->addButton(ui->eve_three_2, 1);
    ele[2]->eleFloor->addButton(ui->eve_three_3, 2);
    ele[2]->eleFloor->addButton(ui->eve_three_4, 3);
    ele[2]->eleFloor->addButton(ui->eve_three_5, 4);
    ele[2]->eleFloor->addButton(ui->eve_three_6, 5);
    ele[2]->eleFloor->addButton(ui->eve_three_7, 6);
    ele[2]->eleFloor->addButton(ui->eve_three_8, 7);
    ele[2]->eleFloor->addButton(ui->eve_three_9, 8);
    ele[2]->eleFloor->addButton(ui->eve_three_10, 9);
    ele[2]->eleFloor->addButton(ui->eve_three_11, 10);
    ele[2]->eleFloor->addButton(ui->eve_three_12, 11);
    ele[2]->eleFloor->addButton(ui->eve_three_13, 12);
    ele[2]->eleFloor->addButton(ui->eve_three_14, 13);
    ele[2]->eleFloor->addButton(ui->eve_three_15, 14);
    ele[2]->eleFloor->addButton(ui->eve_three_16, 15);
    ele[2]->eleFloor->addButton(ui->eve_three_17, 16);
    ele[2]->eleFloor->addButton(ui->eve_three_18, 17);
    ele[2]->eleFloor->addButton(ui->eve_three_19, 18);
    ele[2]->eleFloor->addButton(ui->eve_three_20, 19);

    ele[2]->runAndStop->addButton(ui->three_run, 0);
    ele[2]->runAndStop->addButton(ui->three_stop, 1);

    ele[3]->eleFloor->addButton(ui->eve_four_1, 0);
    ele[3]->eleFloor->addButton(ui->eve_four_2, 1);
    ele[3]->eleFloor->addButton(ui->eve_four_3, 2);
    ele[3]->eleFloor->addButton(ui->eve_four_4, 3);
    ele[3]->eleFloor->addButton(ui->eve_four_5, 4);
    ele[3]->eleFloor->addButton(ui->eve_four_6, 5);
    ele[3]->eleFloor->addButton(ui->eve_four_7, 6);
    ele[3]->eleFloor->addButton(ui->eve_four_8, 7);
    ele[3]->eleFloor->addButton(ui->eve_four_9, 8);
    ele[3]->eleFloor->addButton(ui->eve_four_10, 9);
    ele[3]->eleFloor->addButton(ui->eve_four_11, 10);
    ele[3]->eleFloor->addButton(ui->eve_four_12, 11);
    ele[3]->eleFloor->addButton(ui->eve_four_13, 12);
    ele[3]->eleFloor->addButton(ui->eve_four_14, 13);
    ele[3]->eleFloor->addButton(ui->eve_four_15, 14);
    ele[3]->eleFloor->addButton(ui->eve_four_16, 15);
    ele[3]->eleFloor->addButton(ui->eve_four_17, 16);
    ele[3]->eleFloor->addButton(ui->eve_four_18, 17);
    ele[3]->eleFloor->addButton(ui->eve_four_19, 18);
    ele[3]->eleFloor->addButton(ui->eve_four_20, 19);

    ele[3]->runAndStop->addButton(ui->four_run, 0);
    ele[3]->runAndStop->addButton(ui->four_stop, 1);

    ele[4]->eleFloor->addButton(ui->eve_five_1, 0);
    ele[4]->eleFloor->addButton(ui->eve_five_2, 1);
    ele[4]->eleFloor->addButton(ui->eve_five_3, 2);
    ele[4]->eleFloor->addButton(ui->eve_five_4, 3);
    ele[4]->eleFloor->addButton(ui->eve_five_5, 4);
    ele[4]->eleFloor->addButton(ui->eve_five_6, 5);
    ele[4]->eleFloor->addButton(ui->eve_five_7, 6);
    ele[4]->eleFloor->addButton(ui->eve_five_8, 7);
    ele[4]->eleFloor->addButton(ui->eve_five_9, 8);
    ele[4]->eleFloor->addButton(ui->eve_five_10, 9);
    ele[4]->eleFloor->addButton(ui->eve_five_11, 10);
    ele[4]->eleFloor->addButton(ui->eve_five_12, 11);
    ele[4]->eleFloor->addButton(ui->eve_five_13, 12);
    ele[4]->eleFloor->addButton(ui->eve_five_14, 13);
    ele[4]->eleFloor->addButton(ui->eve_five_15, 14);
    ele[4]->eleFloor->addButton(ui->eve_five_16, 15);
    ele[4]->eleFloor->addButton(ui->eve_five_17, 16);
    ele[4]->eleFloor->addButton(ui->eve_five_18, 17);
    ele[4]->eleFloor->addButton(ui->eve_five_19, 18);
    ele[4]->eleFloor->addButton(ui->eve_five_20, 19);

    ele[4]->runAndStop->addButton(ui->five_run, 0);
    ele[4]->runAndStop->addButton(ui->five_stop, 1);

    for (int i = 0 ;i < 5; i++) {
        ele[i]->eleFloor->setExclusive(false);
    }

    this->up->addButton(ui->up_1, 0);
    this->up->addButton(ui->up_2, 1);
    this->up->addButton(ui->up_3, 2);
    this->up->addButton(ui->up_4, 3);
    this->up->addButton(ui->up_5, 4);
    this->up->addButton(ui->up_6, 5);
    this->up->addButton(ui->up_7, 6);
    this->up->addButton(ui->up_8, 7);
    this->up->addButton(ui->up_9, 8);
    this->up->addButton(ui->up_10, 9);
    this->up->addButton(ui->up_11, 10);
    this->up->addButton(ui->up_12, 11);
    this->up->addButton(ui->up_13, 12);
    this->up->addButton(ui->up_14, 13);
    this->up->addButton(ui->up_15, 14);
    this->up->addButton(ui->up_16, 15);
    this->up->addButton(ui->up_17, 16);
    this->up->addButton(ui->up_18, 17);
    this->up->addButton(ui->up_19, 18);
    this->up->addButton(ui->up_20, 19);

    this->down->addButton(ui->down_1, 0);
    this->down->addButton(ui->down_2, 1);
    this->down->addButton(ui->down_3, 2);
    this->down->addButton(ui->down_4, 3);
    this->down->addButton(ui->down_5, 4);
    this->down->addButton(ui->down_6, 5);
    this->down->addButton(ui->down_7, 6);
    this->down->addButton(ui->down_8, 7);
    this->down->addButton(ui->down_9, 8);
    this->down->addButton(ui->down_10, 9);
    this->down->addButton(ui->down_11, 10);
    this->down->addButton(ui->down_12, 11);
    this->down->addButton(ui->down_13, 12);
    this->down->addButton(ui->down_14, 13);
    this->down->addButton(ui->down_15, 14);
    this->down->addButton(ui->down_16, 15);
    this->down->addButton(ui->down_17, 16);
    this->down->addButton(ui->down_18, 17);
    this->down->addButton(ui->down_19, 18);
    this->down->addButton(ui->down_20, 19);

    this->up->setExclusive(false);
    this->down->setExclusive(false);

    ele[0]->display->addButton(ui->one_floor_1, 0);
    ele[0]->display->addButton(ui->one_floor_2, 1);
    ele[0]->display->addButton(ui->one_floor_3, 2);
    ele[0]->display->addButton(ui->one_floor_4, 3);
    ele[0]->display->addButton(ui->one_floor_5, 4);
    ele[0]->display->addButton(ui->one_floor_6, 5);
    ele[0]->display->addButton(ui->one_floor_7, 6);
    ele[0]->display->addButton(ui->one_floor_8, 7);
    ele[0]->display->addButton(ui->one_floor_9, 8);
    ele[0]->display->addButton(ui->one_floor_10, 9);
    ele[0]->display->addButton(ui->one_floor_11, 10);
    ele[0]->display->addButton(ui->one_floor_12, 11);
    ele[0]->display->addButton(ui->one_floor_13, 12);
    ele[0]->display->addButton(ui->one_floor_14, 13);
    ele[0]->display->addButton(ui->one_floor_15, 14);
    ele[0]->display->addButton(ui->one_floor_16, 15);
    ele[0]->display->addButton(ui->one_floor_17, 16);
    ele[0]->display->addButton(ui->one_floor_18, 17);
    ele[0]->display->addButton(ui->one_floor_19, 18);
    ele[0]->display->addButton(ui->one_floor_20, 19);

    ele[1]->display->addButton(ui->two_floor_1, 0);
    ele[1]->display->addButton(ui->two_floor_2, 1);
    ele[1]->display->addButton(ui->two_floor_3, 2);
    ele[1]->display->addButton(ui->two_floor_4, 3);
    ele[1]->display->addButton(ui->two_floor_5, 4);
    ele[1]->display->addButton(ui->two_floor_6, 5);
    ele[1]->display->addButton(ui->two_floor_7, 6);
    ele[1]->display->addButton(ui->two_floor_8, 7);
    ele[1]->display->addButton(ui->two_floor_9, 8);

    ele[1]->display->addButton(ui->two_floor_10, 9);
    ele[1]->display->addButton(ui->two_floor_11, 10);
    ele[1]->display->addButton(ui->two_floor_12, 11);
    ele[1]->display->addButton(ui->two_floor_13, 12);
    ele[1]->display->addButton(ui->two_floor_14, 13);
    ele[1]->display->addButton(ui->two_floor_15, 14);
    ele[1]->display->addButton(ui->two_floor_16, 15);
    ele[1]->display->addButton(ui->two_floor_17, 16);
    ele[1]->display->addButton(ui->two_floor_18, 17);
    ele[1]->display->addButton(ui->two_floor_19, 18);
    ele[1]->display->addButton(ui->two_floor_20, 19);

    ele[2]->display->addButton(ui->three_floor_1, 0);
    ele[2]->display->addButton(ui->three_floor_2, 1);
    ele[2]->display->addButton(ui->three_floor_3, 2);
    ele[2]->display->addButton(ui->three_floor_4, 3);
    ele[2]->display->addButton(ui->three_floor_5, 4);
    ele[2]->display->addButton(ui->three_floor_6, 5);
    ele[2]->display->addButton(ui->three_floor_7, 6);
    ele[2]->display->addButton(ui->three_floor_8, 7);
    ele[2]->display->addButton(ui->three_floor_9, 8);

    ele[2]->display->addButton(ui->three_floor_10, 9);
    ele[2]->display->addButton(ui->three_floor_11, 10);
    ele[2]->display->addButton(ui->three_floor_12, 11);
    ele[2]->display->addButton(ui->three_floor_13, 12);
    ele[2]->display->addButton(ui->three_floor_14, 13);
    ele[2]->display->addButton(ui->three_floor_15, 14);
    ele[2]->display->addButton(ui->three_floor_16, 15);
    ele[2]->display->addButton(ui->three_floor_17, 16);
    ele[2]->display->addButton(ui->three_floor_18, 17);
    ele[2]->display->addButton(ui->three_floor_19, 18);
    ele[2]->display->addButton(ui->three_floor_20, 19);

    ele[3]->display->addButton(ui->four_floor_1, 0);
    ele[3]->display->addButton(ui->four_floor_2, 1);
    ele[3]->display->addButton(ui->four_floor_3, 2);
    ele[3]->display->addButton(ui->four_floor_4, 3);
    ele[3]->display->addButton(ui->four_floor_5, 4);
    ele[3]->display->addButton(ui->four_floor_6, 5);
    ele[3]->display->addButton(ui->four_floor_7, 6);
    ele[3]->display->addButton(ui->four_floor_8, 7);
    ele[3]->display->addButton(ui->four_floor_9, 8);

    ele[3]->display->addButton(ui->four_floor_10, 9);
    ele[3]->display->addButton(ui->four_floor_11, 10);
    ele[3]->display->addButton(ui->four_floor_12, 11);
    ele[3]->display->addButton(ui->four_floor_13, 12);
    ele[3]->display->addButton(ui->four_floor_14, 13);
    ele[3]->display->addButton(ui->four_floor_15, 14);
    ele[3]->display->addButton(ui->four_floor_16, 15);
    ele[3]->display->addButton(ui->four_floor_17, 16);
    ele[3]->display->addButton(ui->four_floor_18, 17);
    ele[3]->display->addButton(ui->four_floor_19, 18);
    ele[3]->display->addButton(ui->four_floor_20, 19);

    ele[4]->display->addButton(ui->five_floor_1, 0);
    ele[4]->display->addButton(ui->five_floor_2, 1);
    ele[4]->display->addButton(ui->five_floor_3, 2);
    ele[4]->display->addButton(ui->five_floor_4, 3);
    ele[4]->display->addButton(ui->five_floor_5, 4);
    ele[4]->display->addButton(ui->five_floor_6, 5);
    ele[4]->display->addButton(ui->five_floor_7, 6);
    ele[4]->display->addButton(ui->five_floor_8, 7);
    ele[4]->display->addButton(ui->five_floor_9, 8);

    ele[4]->display->addButton(ui->five_floor_10, 9);
    ele[4]->display->addButton(ui->five_floor_11, 10);
    ele[4]->display->addButton(ui->five_floor_12, 11);
    ele[4]->display->addButton(ui->five_floor_13, 12);
    ele[4]->display->addButton(ui->five_floor_14, 13);
    ele[4]->display->addButton(ui->five_floor_15, 14);
    ele[4]->display->addButton(ui->five_floor_16, 15);
    ele[4]->display->addButton(ui->five_floor_17, 16);
    ele[4]->display->addButton(ui->five_floor_18, 17);
    ele[4]->display->addButton(ui->five_floor_19, 18);
    ele[4]->display->addButton(ui->five_floor_20, 19);

    for (int j = 0; j < 5; j++) {
        ele[j]->display->setExclusive(false);
    }
}
