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

private:
    Ui::gestionobjetelectronique *ui;

signals:
    void HomeCliked();
private slots:
    void on_pushButton_clicked();
};

#endif // GESTIONOBJETELECTRONIQUE_H
