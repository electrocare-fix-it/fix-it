#ifndef MENUPRINCIPALE_H
#define MENUPRINCIPALE_H

#include <QWidget>

namespace Ui {
class menuprincipale;
}

class menuprincipale : public QWidget
{
    Q_OBJECT

public:
    explicit menuprincipale(QWidget *parent = nullptr);
    ~menuprincipale();

private slots:
    void on_homeButton_clicked();
    void on_gestionObjectsButton_clicked();
    void on_gestionClientsButton_clicked();

private:
    Ui::menuprincipale *ui;

signals:
    void homeClicked();
    void openObjects();
    void openClients();
};

#endif // MENUPRINCIPALE_H
