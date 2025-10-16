#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QWidget>

namespace Ui {
class employe;
}

class employe : public QWidget
{
    Q_OBJECT

public:
    explicit employe(QWidget *parent = nullptr);
    ~employe();

private:
    Ui::employe *ui;
};

#endif // EMPLOYE_H
