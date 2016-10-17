#include "bestscores.h"
#include "ui_bestscores.h"

BestScores::BestScores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BestScores)
{
    ui->setupUi(this);
}

BestScores::~BestScores()
{
    delete ui;
}

int BestScores::getMinimumScore()
{
    QSqlQuery query1;

    query1.exec("SELECT MIN(scores) FROM [Sheet2$]");

    if(query1.first())
    {
        minimumScore = query1.value(0).toInt();
    }

    return minimumScore;
}

int BestScores::getIdOfMinimumScore()
{
    QSqlQuery query2;

    query2.exec("SELECT id FROM [Sheet2$] WHERE scores=(SELECT MIN(scores) FROM [Sheet2$])");

    if(query2.first())
    {
        idOfMinimumScore = query2.value(0).toInt();
    }

    return idOfMinimumScore;
}

void BestScores::showScores()
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(10);

    QStringList labels;
    labels << ("NICK") << ("SCORE");
    ui->tableWidget->setHorizontalHeaderLabels(labels);

    QSqlQuery query;

    query.prepare("SELECT * FROM [Sheet2$] ORDER BY scores DESC");

    short i=0;

    if(query.exec())
    {
      while(query.next())
      {
          QString nick = query.value(1).toString();
          QString score = query.value(2).toString();

          ui->tableWidget->setItem(i,0,new QTableWidgetItem(nick));
          ui->tableWidget->setItem(i,1,new QTableWidgetItem(score));
          i++;
      }
    }
}

void BestScores::on_pushButton_clicked()
{
    BestScores::close();
}


