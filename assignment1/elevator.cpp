#include "elevator.h"

void Elevator::collectUpAndDown(QButtonGroup *up, QButtonGroup *down) {
    this->up = up;
    this->up->setExclusive(false);
    this->down = down;
    this->down->setExclusive(false);
}

void Elevator::checkPushedFloor() {

    QObject::connect(this->eleFloor, SIGNAL(buttonClicked(int)), this, SLOT(setPushedFloor(int)));
    QObject::connect(this->runAndStop, SIGNAL(buttonClicked(int)), this, SLOT(eleRun(int)));
}

void Elevator::setPushedFloor(int i) {
    this->isFloorPushed[i+1] = 1;
    if(!this->eleFloor->button(i)->isChecked()) {
        this->eleFloor->button(i)->setChecked(true);
    }
//    cout << isFloorPushed[i+1] << endl;
}

void Elevator::setAskedFloor(int status, int i, int number) {
//    cout << "ele " << this->eleNumber << endl;
    if (this->eleNumber == number) {
        if (status == 1) {
            this->isUpAsked[i] = 1;
        }
        if (status == 2) {
            this->isDownAsked[i] = 1;
        }
    }
}

void Elevator::startScheduler() {
    this->checkPushedFloor();
    this->eleScheduler();


}

//实现电梯的关闭和运行
void Elevator::eleRun(int i) {
    if (i == 0) {
        for (int j = 0; j < 20; j++) {
            this->eleFloor->button(j)->setEnabled(true);
        }

        this->runAndStop->button(0)->setEnabled(false);
        this->runAndStop->button(1)->setEnabled(true);

        this->status = 0;
        this->statusLabel->setText("STAY");
        this->display->button(nowFloor-1)->setEnabled(true);
        this->display->button(nowFloor-1)->setChecked(true);
        this->run = 0;
        this->eleScheduler();
    }
    if (i == 1) {
        for (int j = 0; j < 20; j++) {
            this->eleFloor->button(j)->setEnabled(false);
        }

        this->runAndStop->button(0)->setEnabled(true);
        this->runAndStop->button(1)->setEnabled(false);

        //清空所有请求
        for (int j = 1; j <= 20; j++) {
            if (this->isUpAsked[j] == 1) {
                this->isUpAsked[j] = 0;
                this->up->button(j-1)->setChecked(false);
            }
            if (this->isDownAsked[j] == 1) {
                this->isDownAsked[j] = 0;
                this->down->button(j-1)->setChecked(false);
            }
            this->isFloorPushed[j] = 0;
            this->eleFloor->button(j-1)->setChecked(false);


        }
        this->status = -1;
        this->statusLabel->setText("ERROR");
        this->display->button(nowFloor-1)->setChecked(false);
        this->display->button(nowFloor-1)->setEnabled(false);
        this->run = -1;
    }
}

//实现电梯的开门和关门
void Elevator::eleDoor() {

    this->doorLabel->setText("OPENED");
    //这里将该线程暂停一定时间
    QTime t;
    t.start();
    while(t.elapsed()<1000)
        QCoreApplication::processEvents();

    this->doorLabel->setText("CLOSED");
    this->eleFloor->button(this->nowFloor-1)->setChecked(false);

}

//实现电梯的上行和下行
void Elevator::eleMove(int i) {
    if (this->status != -1) {
        if (i == 1) {
            this->statusLabel->setText("UP");
            //这里将该线程暂停一定时间
            QTime t;
            t.start();
            while(t.elapsed()<1000)
                QCoreApplication::processEvents();
            if (this->status != -1) {
                this->display->button(nowFloor-1)->setChecked(false);
                this->display->button(nowFloor-1)->setEnabled(false);
                this->nowFloor += 1;
                this->display->button(nowFloor-1)->setEnabled(true);
                this->display->button(nowFloor-1)->setChecked(true);
                this->floorLabel->setNum(this->nowFloor);
            }

        }
        if (i == 2) {
            this->statusLabel->setText("DOWN");
            //这里将该线程暂停一定时间
            QTime t;
            t.start();
            while(t.elapsed()<1000)
                QCoreApplication::processEvents();
            if (this->status != -1) {
                this->display->button(nowFloor-1)->setChecked(false);
                this->display->button(nowFloor-1)->setEnabled(false);
                this->nowFloor -= 1;
                this->display->button(nowFloor-1)->setEnabled(true);
                this->display->button(nowFloor-1)->setChecked(true);
                this->floorLabel->setNum(this->nowFloor);
            }
        }
    }
}

//实现电梯的调度算法
void Elevator::eleScheduler() {
    while (1) {
        QCoreApplication::processEvents();

        if (this->run == -1) {
            break;
        }

        if (!this->display->button(nowFloor-1)->isChecked()) {
            this->display->button(nowFloor-1)->setChecked(true);
        }
        //说明现在电梯没有移动
        if (status == 0) {
            //查找离电梯最近的请求
            int tempFloor = 100;
            //记录是否有请求
            int temp = 0;
            for (int i = 1; i <= 20; i++) {
                if (this->isDownAsked[i] == 1) {
                    if (abs(tempFloor - this->nowFloor) > abs(i - this->nowFloor)) {
                        tempFloor = i;
                        temp = 1;
                    }
                }

                if (this->isUpAsked[i] == 1) {
                    if (abs(tempFloor - this->nowFloor) > abs(i - this->nowFloor)) {
                        tempFloor = i;
                        temp = 1;
                    }
                }

                if (this->isFloorPushed[i] == 1) {
                    if (abs(tempFloor - this->nowFloor) > abs(i - this->nowFloor)) {
                        tempFloor = i;
                        temp = 1;
                    }
                }

            }

            if (temp == 1)
                this->isFloorPushed[tempFloor] = 1;
            else
                continue;

            //判断该请求会使电梯上行、下行还是就在本楼
            if (tempFloor > this->nowFloor) {
                this->status = 1;
                this->eleMove(this->status);
            }
            else if (tempFloor < this->nowFloor) {
                this->status = 2;
                this->eleMove(this->status);
            }
            else {
                this->isFloorPushed[nowFloor] = 0;

                this->isUpAsked[nowFloor] = 0;
                this->up->button(nowFloor-1)->setChecked(false);

                this->isDownAsked[nowFloor] = 0;
                this->down->button(nowFloor-1)->setChecked(false);
                this->eleDoor();
            }

            continue;
        }

        //电梯正在上行
        if (status == 1) {
            int temp = 0;
            //检查本层楼是否内部被按下，是否外部有请求
            if (this->isUpAsked[nowFloor] == 1) {
                temp = 1;
                this->isUpAsked[nowFloor] = 0;
                this->up->button(nowFloor-1)->setChecked(false);
            }

            if (this->isFloorPushed[nowFloor] == 1) {
                temp = 1;
                this->isFloorPushed[nowFloor] = 0;
            }
            //如果有，执行开关门动作
            if (temp == 1) {
                this->eleDoor();
            }

            temp = 0;
            //检查楼上是否还有按钮被按下
            for (int i = nowFloor + 1; i <= 20; i++) {
                if (isFloorPushed[i] == 1) {
                    temp = 1;
                    break;
                }
            }
            //说明还有按钮被按下
            if (temp == 1) {
                this->eleMove(this->status);
            }
            //说明已经没有按钮被按下了
            else {
                this->status = 0;
                if (this->isDownAsked[nowFloor] == 1) {
                    this->isDownAsked[nowFloor] = 0;
                    this->down->button(nowFloor-1)->setChecked(false);
                }
                this->statusLabel->setText("STAY");
            }
            continue;
        }

        //电梯正在下行
        if (status == 2) {
            int temp = 0;
            //检查本层楼是否内部被按下，是否外部有请求

            if (this->isDownAsked[nowFloor] == 1) {
                temp = 1;
                this->isDownAsked[nowFloor] = 0;
                this->down->button(nowFloor-1)->setChecked(false);
            }
            if (this->isFloorPushed[nowFloor] == 1) {
                temp = 1;
                this->isFloorPushed[nowFloor] = 0;
            }
            //如果有，执行开关门动作
            if (temp == 1) {
                this->eleDoor();
            }

            temp = 0;
            //检查楼上是否还有按钮被按下
            for (int i = nowFloor - 1; i >= 1; i--) {
                if (isFloorPushed[i] == 1) {
                    temp = 1;
                    break;
                }
            }
            //说明还有按钮被按下
            if (temp == 1) {
                this->eleMove(this->status);
            }
            //说明已经没有按钮被按下了
            else {
                this->status = 0;
                if (this->isUpAsked[nowFloor] == 1) {
                    this->isUpAsked[nowFloor] = 0;
                    this->up->button(nowFloor-1)->setChecked(false);
                }
                this->statusLabel->setText("STAY");
            }
            continue;
        }


    }

}



