#ifndef GESTIONPIECESDETACHEES_H
#define GESTIONPIECESDETACHEES_H

#include <QWidget>

namespace Ui {
class gestionpiecesdetachees;
}

class gestionpiecesdetachees : public QWidget
{
    Q_OBJECT

public:
    explicit gestionpiecesdetachees(QWidget *parent = nullptr);
    ~gestionpiecesdetachees();

private slots:
    void on_pushButton_clicked();

private:
    Ui::gestionpiecesdetachees *ui;
signals:
    void HomeCliked();
};

#endif // GESTIONPIECESDETACHEES_H
