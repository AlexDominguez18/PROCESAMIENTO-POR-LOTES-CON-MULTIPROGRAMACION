#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Simulador de Procesos");
    setFixedSize(300,150);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateProcesses(){
    Proceso p;
    int estimatedTime,num1,num2,signOperator,i;
    //Generating random numbers
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution_time(6,15);
    std::uniform_int_distribution<int> distribution_numbers(-100,100);
    std::uniform_int_distribution<int> distribution_sign(0,5);
    auto randomNumber = bind(distribution_numbers, generator);
    auto randomSign = bind(distribution_sign,generator);
    auto randomEstimatedTime = bind(distribution_time, generator);

    i = 0;
    while (i < totalProcess){
        i++;
        p.setId(i);
        num1 = randomNumber();
        num2 = randomNumber();
        signOperator = randomSign();
        createOperation(p,num1,num2,signOperator);
        estimatedTime = randomEstimatedTime();
        p.setTiempoEstimado(estimatedTime);
        lot.push_back(p);
        if (i % PROCESS_PER_LOT == 0 or i == totalProcess){
            lots.push_back(lot);
            lot.clear();
            lotsCounter++;
        }
    }
}

void MainWindow::createOperation(Proceso &p,int num1,int num2,int signOperator)
{
    int result;
    std::string operation = "";
    char sign;
    //Generating random numbers with the system time
    std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution_numbers(-100,100);
    std::uniform_int_distribution<int> distribution_sign(0,5);
    auto randomNumber = bind(distribution_sign,generator);

    switch (signOperator){
    case ADD:
        result = num1 + num2;
        sign = '+';
        break;
    case SUB:
        result = num1 - num2;
        sign = '-';
        break;
    case MULT:
        result = num1 * num2;
        sign = '*';
        break;
    case DIV:
        while (num2 == 0){
            num2 = randomNumber();
        }
        result = num1 / num2;
        sign = '/';
        break;
    case MOD:
        while (num2 == 0){
            num2 = randomNumber();
        }
        result = num1 % num2;
        sign = '%';
        break;
    default:
        sign = '+';
        result = num1 + num2;
        break;
    }
    operation = std::to_string(num1) + sign + std::to_string(num2);
    p.setOperacion(operation);
    p.setResultadoOperacion(result);
}

void MainWindow::startProcesses()
{
    std::string lotsSeparation = "";
    pendientLots = lotsCounter;
    currentLot = 0;

    ui->pendientLotsLCD->display(pendientLots);
    while (lots.size()){
        pendientLots--;
        lotsSeparation = "LOTE " + std::to_string(currentLot+1);
        ui->finishedTB->insertRow(ui->finishedTB->rowCount());
        ui->finishedTB->setItem(currentLot*(PROCESS_PER_LOT+1),0,new QTableWidgetItem(QString(lotsSeparation.c_str())));
        while (lots[0].size()){
            showWorkingLot();
            runProcess();
        }
        ui->lotWorkingTB->setRowCount(0);
        lots.erase(lots.begin());
        currentLot++;
    }
}

void MainWindow::showWorkingLot()
{
    int i = 0;
    int rows = 0;
    int j = 1;
    Proceso p;

    ui->lotWorkingTB->setRowCount(0);
    while (j < (int)lots[i].size()){
        p = lots[i][j];
        ui->lotWorkingTB->insertRow(ui->lotWorkingTB->rowCount());
        ui->lotWorkingTB->setItem(rows,0,new QTableWidgetItem(QString::number(p.getId())));
        ui->lotWorkingTB->setItem(rows,1,new QTableWidgetItem(QString::number(p.getTiempoEstimado())));
        ui->lotWorkingTB->setItem(rows,2,new QTableWidgetItem(QString::number(p.getTiempoTranscurrido())));
        rows++;
        j++;
    }
}

void MainWindow::runProcess()
{
    Proceso& p = lots[0][0];
    p.setFinalizacion(SUCCESFUL_FINISH);
    tT = p.getTiempoTranscurrido();
    tR = lots[0][0].getTiempoEstimado() - tT;
    interrupted = false;

    ui->processRuningTB->setItem(0,0,new QTableWidgetItem(QString::number(p.getId())));
    ui->processRuningTB->setItem(1,0,new QTableWidgetItem(QString(p.getOperacion().c_str())));
    ui->processRuningTB->setItem(2,0,new QTableWidgetItem(QString::number(p.getTiempoEstimado())));
    ui->processRuningTB->setItem(3,0,new QTableWidgetItem(QString::number(p.getTiempoTranscurrido())));
    ui->processRuningTB->setItem(4,0,new QTableWidgetItem(QString::number(tR)));

    while (tT < p.getTiempoEstimado() and tT != ACTION_CODE){
        tR--;
        tT++;
        p.setTiempoTranscurrido(tT);
        ui->processRuningTB->setItem(3,0,new QTableWidgetItem(QString::number(p.getTiempoTranscurrido())));
        ui->processRuningTB->setItem(4,0,new QTableWidgetItem(QString::number(tR)));
        globalCounter++;
        ui->globalCountLCD->display(globalCounter);
        ui->pendientLotsLCD->display(pendientLots);
        delay();
    }
    if (p.getFinalizacion() != ERROR_FINISH and !interrupted){
        showFinishedProcesses(SUCCESFUL_FINISH);
    }
    lots[0].erase(lots[0].begin());
    ui->processRuningTB->clearContents();
}

void MainWindow::showFinishedProcesses(bool finishedType)
{
    Proceso& p = lots[0][0];
    int rowsFinished(ui->finishedTB->rowCount());
    p.setFinalizacion(finishedType);
    ui->finishedTB->insertRow(rowsFinished);
    ui->finishedTB->setItem(rowsFinished,0,new QTableWidgetItem(QString::number(p.getId())));
    ui->finishedTB->setItem(rowsFinished,1,new QTableWidgetItem(QString(p.getOperacion().c_str())));
    ui->finishedTB->setItem(rowsFinished,3,new QTableWidgetItem(QString::number(currentLot)));
    if (finishedType == ERROR_FINISH){
        ui->finishedTB->setItem(rowsFinished,2,new QTableWidgetItem(QString("ERROR")));
    }
    else{
        ui->finishedTB->setItem(rowsFinished,2,new QTableWidgetItem(QString::number(p.getResultadoOperacion())));
    }
}

void MainWindow::interruptProcesss()
{
    Proceso p = lots[0][0];;
    lots[0].push_back(p);
}

void MainWindow::delay()
{
    QTime delayTime = QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < delayTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

void MainWindow::on_processNumberSP_valueChanged(int arg1)
{
    if (arg1 > 0){
        ui->processAcountPB->setEnabled(true);
    }
    else{
        ui->processAcountPB->setEnabled(false);
    }
}

void MainWindow::on_processAcountPB_clicked()
{
    ui->processNumberSP->setEnabled(false);
    totalProcess = ui->processNumberSP->value();
    generateProcesses();
    ui->stackedWidget->setCurrentIndex(SHOW_LOTS);
    setFixedSize(900,580);
}

void MainWindow::on_startRunPB_clicked()
{
    startProcesses();
    ui->startRunPB->setEnabled(false);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_I and state == RUNNING){
        qDebug() << "Interrupcion";
        interrupted = true;
        tT = ACTION_CODE;
        interruptProcesss();
    }
    else if (event->key() == Qt::Key_E and state == RUNNING){
        qDebug() << "Error";
        tT = ACTION_CODE;
        showFinishedProcesses(ERROR_FINISH);
    }
    else if (event->key() == Qt::Key_P and state == RUNNING){
        qDebug() << "Pause";
        state = PAUSED;
        pause.exec();
    }
    else if (event->key() == Qt::Key_C and state == PAUSED){
        qDebug() << "Continue";
        state = RUNNING;
        pause.quit();
    }
    else{
        qDebug() << "Nothing";
    }
}
