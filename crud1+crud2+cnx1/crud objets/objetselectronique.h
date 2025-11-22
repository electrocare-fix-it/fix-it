#ifndef GESTIONOBJETELECTRONIQUE_H
#define GESTIONOBJETELECTRONIQUE_H

#include <QWidget>

namespace Ui {
class gestionobjetelectronique;
}

class gestionobjetelectronique : public QWidget
{
    Q_OBJECT

public:
    explicit gestionobjetelectronique(QWidget *parent = nullptr);
    ~gestionobjetelectronique();

signals:
    void homeRequested();

private slots:
    void on_pushButton_clicked();

private:
    Ui::gestionobjetelectronique *ui;
};

#endif // GESTIONOBJETELECTRONIQUE_H
