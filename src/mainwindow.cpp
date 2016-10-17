#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //dodanie bazy danych pobieranej z pliku excel
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("DRIVER={Microsoft Excel Driver (*.xls)};READONLY=FALSE;DBQ=" + QString("flags.xls"));


    //kontrola błędów - połączenie z bazą danych
    if(!db.open())
    {
        QMessageBox::warning(this,"Connection status","Not connected to database","OK");
    }
    else
        QMessageBox::information(this,"Connection status","Connected to database","OK");

    AppState = START;
    temp = 1;
    points = 0;
    actualFlagNumber = 1;
    getNumberofAllflags();
    NumberofFlags = 5;

    ui->label_4->setText("SCORE: "+ QString::number(points));
    ui->label->setText("PRESS PLAY TO START");
    ui->label_3->setText(QString::number(actualFlagNumber)+"/"+QString::number(NumberofFlags));
    ui->pushButton->setText("PLAY");
    ui->lineEdit->setVisible(false);

    srand(time(NULL));

}

MainWindow::~MainWindow()
{
    db.close();
    delete [] flag;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    PlayGame();
}

void MainWindow::askQuestion()
{
    ui->radioButton->setCheckable(true);
    ui->radioButton_2->setCheckable(true);
    ui->radioButton_3->setCheckable(true);
    ui->radioButton_4->setCheckable(true);

    QSqlQuery query;

    //losowanie pytania z pliku excel
    idRandomize();

    query.prepare("SELECT * FROM [Sheet1$] WHERE id=:id");
    query.bindValue(":id",id);

    if(query.exec())
    {

        while(query.next())
        {
        ui->label_2->setText("Whose flag is it?");

        answerA = query.value(1).toString();
        ui->radioButton->setText(answerA);

        answerB = query.value(2).toString();
        ui->radioButton_2->setText(answerB);

        answerC = query.value(3).toString();
        ui->radioButton_3->setText(answerC);

        answerD = query.value(4).toString();
        ui->radioButton_4->setText(answerD);

        correctAnswer = query.value(5).toString();

        source = query.value(6).toString();
        QPixmap flag = source;
        //resize obrazka
        ui->label->setPixmap(flag.scaled(250,160,Qt::KeepAspectRatio,Qt::SmoothTransformation));

        }
    }

    ui->pushButton->setText("CHECK");

}

void MainWindow::checkAnswer()
{
    if(ui->radioButton->isChecked())
        currentAnswer = answerA;

    if(ui->radioButton_2->isChecked())
        currentAnswer = answerB;

    if(ui->radioButton_3->isChecked())
        currentAnswer = answerC;

    if(ui->radioButton_4->isChecked())
        currentAnswer = answerD;

    if(currentAnswer == correctAnswer)
    {
        ui->label_2->setText("Excelent, correct answer!");
        currentAnswer = "0";
        points ++;
        ui->label_4->setText("SCORE: "+ QString::number(points));
        ui->radioButton->setCheckable(false);
        ui->radioButton_2->setCheckable(false);
        ui->radioButton_3->setCheckable(false);
        ui->radioButton_4->setCheckable(false);

    }
    else
    {
        ui->label_2->setText("Sorry, it is "+correctAnswer);
        ui->radioButton->setCheckable(false);
        ui->radioButton_2->setCheckable(false);
        ui->radioButton_3->setCheckable(false);
        ui->radioButton_4->setCheckable(false);
    }

    ui->pushButton->setText("NEXT");

}

void MainWindow::getNumberofAllflags()
{
    QSqlQueryModel model;

    model.setQuery("SELECT * FROM [Sheet1$]");

    NumberofAllflags = model.rowCount();

    ui->label_3->setText(QString::number(actualFlagNumber)+"/"+QString::number(NumberofFlags));

}

int MainWindow::idRandomize()
{
    do
    {
    id = rand()%NumberofAllflags+1;
    randomize_ok = true;

        for(int i=0;i<NumberofFlags;i++)
        {
            if(flag[i] == id) randomize_ok = false;
        }

        if(randomize_ok == true)
        {
            flag[actualFlagNumber] = id;
        }
    }
    while(randomize_ok == false);

    return id;
}

void MainWindow::getAppState()
{
    if(actualFlagNumber < NumberofFlags)
    {
        AppState = ISRUNNING;
    }

    if(actualFlagNumber == (NumberofFlags+1))
    {
        AppState = END;
    }

}

void MainWindow::PlayGame()
{
    ui->label_4->setText("SCORE: "+ QString::number(points));
    //getNumberofAllflags();

    if(AppState == START)
    {
       points = 0;
       actualFlagNumber = 1;
       NumberofFlags = settings.getNumberOfFlags();
       ui->label_4->setText("SCORE: "+ QString::number(points));
    }

    getAppState();

    if(AppState == ISRUNNING)
    {
        ui->label_3->setText(QString::number(actualFlagNumber)+"/"+QString::number(NumberofFlags));

        temp *=-1;

         if(temp == -1)
         {
          ui->pushButton->setStyleSheet("background-color: #E1F5C4;"
                                        "border: 1px solid gray;"
                                        "border-radius: 10px;");
          askQuestion();
         }

         else
         {

          ui->pushButton->setStyleSheet("background-color: #336666;"
                                        "border: 1px solid gray;"
                                        "border-radius: 10px;"
                                        "color: #FFFFFF");
          checkAnswer();
          actualFlagNumber++;
         }
    }

    if(AppState == END)
    {
        ui->label->setText("YOUR SCORE IS: " +QString::number(points));
        ui->pushButton->setText("PLAY AGAIN");
        actualFlagNumber = 1;

        min_scores = bestscores.getMinimumScore();

        if(points > min_scores)
        {
            ui->lineEdit->setVisible(true);
            ui->pushButton->setDisabled(true);

        }
        AppState = START;
    }


}

void MainWindow::on_actionSettings_triggered()
{
    settings.setModal(true);
    settings.exec();

    AppState = START;
    ui->pushButton->setText("PLAY");
    temp = 1;
}

void MainWindow::on_actionBest_scores_triggered()
{
    bestscores.showScores();
    bestscores.setModal(true);
    bestscores.exec();
}

void MainWindow::SavePoints()
{
    QSqlQuery update_query;

          int min_scoresID;
          min_scoresID = bestscores.getIdOfMinimumScore();

          //zapisz wynik w miejsce id z najmniejszym wynikiem
          update_query.prepare("UPDATE [Sheet2$] SET nick=:nick, scores=:points WHERE id=:min_scoresID");
          update_query.bindValue(":nick",nick);
          update_query.bindValue(":points",points);
          update_query.bindValue(":min_scoresID",min_scoresID);
          update_query.exec();

}

void MainWindow::on_lineEdit_returnPressed()
{
    nick = ui->lineEdit->text();
    ui->lineEdit->setVisible(false);
    ui->pushButton->setDisabled(false);

    SavePoints();

    on_actionBest_scores_triggered();

}


