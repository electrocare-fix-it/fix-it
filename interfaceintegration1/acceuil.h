#ifndef ACCEUIL_H
#define ACCEUIL_H

#include <QWidget>

namespace Ui {
class acceuil;
}

class acceuil : public QWidget
{
    Q_OBJECT

public:
    explicit acceuil(QWidget *parent = nullptr);
    ~acceuil();

private:
    Ui::acceuil *ui;
};

#endif // ACCEUIL_H
