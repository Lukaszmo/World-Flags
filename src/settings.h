#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    int getNumberOfFlags();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Settings *ui;

    int Number_of_flags;
};

#endif // SETTINGS_H
