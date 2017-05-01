#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <elevator.h>


namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    Elevator *ele[5];
    int isUpAsked[21];
    int isDownAsked[21];

    QButtonGroup *up;
    QButtonGroup *down;

    int r;

    void initButtonGroup();
    void checkAskedFloor();



private:
    Ui::MainWindow *ui;


private slots:
    void setUpFloor(int i);
    void setDownFloor(int i);

signals:
    void sendStart();
    void sendFloor(int status, int i, int number);
};




#endif // MAINWINDOW_H
