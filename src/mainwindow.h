#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QPixmap>
#include <ctime>
#include <settings.h>
#include <bestscores.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void askQuestion();
    void checkAnswer();
    void getNumberofAllflags();
    void getAppState();
    void PlayGame();
    void SavePoints();
    int idRandomize();

private slots:
    void on_pushButton_clicked();

    void on_actionSettings_triggered();

    void on_actionBest_scores_triggered();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
    QString answerA,answerB,answerC,answerD,currentAnswer, correctAnswer;
    QString source;
    QString nick;

    short temp; //zmienna pomocnicza do push button
    short points;
    int NumberofFlags;
    int NumberofAllflags;
    int actualFlagNumber;
    int min_scores;

    int id;
    int *flag = new int[50]; //tablica z id flag

    bool randomize_ok;

    enum States {START, ISRUNNING, END};
    States AppState;

    Settings settings;
    BestScores bestscores;
};

#endif // MAINWINDOW_H
