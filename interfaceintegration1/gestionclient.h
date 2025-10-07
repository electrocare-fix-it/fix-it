#ifndef GESTIONCLIENT_H
#define GESTIONCLIENT_H

#include <QWidget>

namespace Ui {
class gestionclient;
}

class gestionclient : public QWidget
{
    Q_OBJECT

public:
    explicit gestionclient(QWidget *parent = nullptr);
    ~gestionclient();

private:
    Ui::gestionclient *ui;
signals:
    void HomeCliked();
private slots:
    void on_pushButton_clicked();
};

#endif // GESTIONCLIENT_H
