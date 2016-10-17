#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    Number_of_flags = 5;
}

Settings::~Settings()
{
    delete ui;
}

int Settings::getNumberOfFlags()
{
    return Number_of_flags;
}
void Settings::on_pushButton_clicked()
{
    Number_of_flags = ui->comboBox->currentText().toInt();

    Settings::close();
}
