#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    bool connectToArduino(const QString &portName = QString(), int baudRate = 9600);
    void disconnectFromArduino();
    bool isConnected() const;
    QStringList getAvailablePorts() const;
    QString getCurrentPort() const;
    bool isPortAvailable(const QString &portName) const;
    bool waitForPortAvailable(const QString &portName, int maxWaitMs = 5000) const;

signals:
    void accessGranted(const QString &employeeName, const QString &status);
    void accessDenied(const QString &employeeName, const QString &status);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void readData();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort;
    QString m_currentPort;
    bool m_isConnected;
    QTimer *m_checkTimer;
    
    void parseArduinoMessage(const QString &message);
    void checkForData();
};

#endif // SERIALPORTMANAGER_H

