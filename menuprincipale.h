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
<<<<<<< HEAD
    void on_gestionClientsButton_clicked();
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0

private:
    Ui::menuprincipale *ui;

signals:
    void homeClicked();
    void openObjects();
<<<<<<< HEAD
    void openClients();
=======
>>>>>>> ce00fc6046d5fbcbf1f0d666eb5b330477ad36f0
};

#endif // MENUPRINCIPALE_H
