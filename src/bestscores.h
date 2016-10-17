#ifndef BESTSCORES_H
#define BESTSCORES_H

#include <QDialog>
#include <QSqlQuery>
#include <QtSql>

namespace Ui {
class BestScores;
}

class BestScores : public QDialog
{
    Q_OBJECT

public:
    explicit BestScores(QWidget *parent = 0);
    ~BestScores();

    void showScores();
    int getIdOfMinimumScore();
    int getMinimumScore();

private slots:
    void on_pushButton_clicked();



private:
    Ui::BestScores *ui;  
    int idOfMinimumScore;
    int minimumScore;
};

#endif // BESTSCORES_H
