#ifndef GESTIONEMPLOYE_H
#define GESTIONEMPLOYE_H

#include <QWidget>

namespace Ui {
class gestionemploye;
}

class gestionemploye : public QWidget
{
    Q_OBJECT

public:
    explicit gestionemploye(QWidget *parent = nullptr);
    ~gestionemploye();

private:
    Ui::gestionemploye *ui;
signals:
    void HomeRequested();
private slots:
    void on_pushButton_clicked();
};

#endif // GESTIONEMPLOYE_H
