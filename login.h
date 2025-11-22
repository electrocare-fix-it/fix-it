#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QString>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    void clearFields();

signals:
    void loginSuccessful(const QString& role);
    void loginCancelled();

private slots:
    void on_btnLogin_clicked();
    void on_btnCancel_clicked();

private:
    Ui::Login *ui;
    bool authenticateUser(const QString& login, const QString& password, const QString& role);
    void createUsersTableIfNotExists();
};

#endif // LOGIN_H

