#ifndef ARDUINOSERIAL_H
#define ARDUINOSERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QTimer>

class ArduinoSerial : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoSerial(QObject *parent = nullptr);
    ~ArduinoSerial();

    bool connectToArduino();
    void disconnectFromArduino();
    bool isConnected() const;
    QString getPortName() const;
    QStringList getAvailablePorts() const;

signals:
    void cinReceived(const QString& cin);
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString& error);

private slots:
    void readSerialData();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort* m_serialPort;
    QString m_buffer;
    QTimer* m_connectionTimer;
    bool m_isConnected;

    void parseReceivedData(const QString& data);
    bool tryConnectToPort(const QString& portName);
};

#endif // ARDUINOSERIAL_H

