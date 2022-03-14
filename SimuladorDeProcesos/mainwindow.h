#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <thread>
#include <windows.h>
#include <conio.h>
#include <chrono>
#include <random>
#include <functional>

#include <QTimer>
#include <QTime>
#include <QThread>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtDebug>
#include <QMutex>
#include <QEventLoop>

#include "proceso.h"

#define CAPTURE_DATA 0
#define SHOW_LOTS 1
#define PROCESS_PER_LOT 5
#define NUMBER_TO_DELAY 500000000

enum Operations{ADD,SUB,MULT,DIV,MOD};
enum States{PAUSED,RUNNING};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void generateProcesses();
    void createOperation(Proceso &p,int num1,int num2,int signOperator);
    void startProcesses();
    void showWorkingLot();
    void runProcess();
    void showFinishedProcesses(bool finishedType);
    void interruptProcesss();
    void delay();

protected:

    void keyPressEvent(QKeyEvent *event);

private slots:

    void on_processNumberSP_valueChanged(int arg1);

    void on_processAcountPB_clicked();

    void on_startRunPB_clicked();

private:
    Ui::MainWindow *ui;
    QEventLoop pause;

    int capturedProcess = 0;
    int totalProcess;
    int processLot = 0;
    int lotsCounter = 0;
    int globalCounter = 0;
    int pendientLots;
    int currentLot;
    int currentProcess;
    int tT;
    int tR;
    int timerSeconds = 0;
    bool state = RUNNING;
    bool interrupted = false;
    std::vector<std::vector<Proceso>> lots;
    std::vector<Proceso> lot;
};
#endif // MAINWINDOW_H
