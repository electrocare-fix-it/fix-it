#include "arduinoserial.h"
#include <QDebug>
#include <QRegularExpression>

ArduinoSerial::ArduinoSerial(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)
    , m_isConnected(false)
{
    m_serialPort = new QSerialPort(this);
    m_connectionTimer = new QTimer(this);
    
    connect(m_serialPort, &QSerialPort::readyRead, this, &ArduinoSerial::readSerialData);
    
    #if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &ArduinoSerial::handleError);
    #else
    connect(m_serialPort, SIGNAL(error(QSerialPort::SerialPortError)), 
            this, SLOT(handleError(QSerialPort::SerialPortError)));
    #endif
    
    m_connectionTimer->setSingleShot(true);
    m_connectionTimer->setInterval(2000);
}

ArduinoSerial::~ArduinoSerial()
{
    disconnectFromArduino();
}

bool ArduinoSerial::connectToArduino()
{
    if (m_isConnected) {
        return true;
    }

    QStringList ports = getAvailablePorts();
    
    for (const QString& portName : ports) {
        if (tryConnectToPort(portName)) {
            m_isConnected = true;
            emit connectionStatusChanged(true);
            qDebug() << "Arduino connecte sur" << portName;
            return true;
        }
    }
    
    emit errorOccurred("Aucun Arduino detecte");
    return false;
}

bool ArduinoSerial::tryConnectToPort(const QString& portName)
{
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    
    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
    
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        m_buffer.clear();
        return true;
    }
    
    return false;
}

void ArduinoSerial::disconnectFromArduino()
{
    if (m_serialPort && m_serialPort->isOpen()) {
        m_serialPort->close();
    }
    m_isConnected = false;
    emit connectionStatusChanged(false);
}

bool ArduinoSerial::isConnected() const
{
    return m_isConnected && m_serialPort && m_serialPort->isOpen();
}

QString ArduinoSerial::getPortName() const
{
    if (m_serialPort && m_serialPort->isOpen()) {
        return m_serialPort->portName();
    }
    return QString();
}

QStringList ArduinoSerial::getAvailablePorts() const
{
    QStringList ports;
    QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
    
    for (const QSerialPortInfo& info : availablePorts) {
        QString portName = info.portName();
        if (!portName.isEmpty()) {
            ports << portName;
        }
    }
    
    return ports;
}

void ArduinoSerial::readSerialData()
{
    if (!m_serialPort || !m_serialPort->isOpen()) {
        return;
    }
    
    QByteArray data = m_serialPort->readAll();
    QString newData = QString::fromUtf8(data);
    m_buffer += newData;
    
    m_buffer = m_buffer.trimmed();
    
    QRegularExpression cinRegex("^(\\d{8})");
    QRegularExpressionMatch match = cinRegex.match(m_buffer);
    
    if (match.hasMatch()) {
        QString cin = match.captured(1);
        qDebug() << "CIN complet detecte:" << cin;
        emit cinReceived(cin);
        m_buffer = m_buffer.mid(8).trimmed();
    } else if (m_buffer.length() >= 8) {
        m_buffer.clear();
    }
    
    if (m_buffer.length() > 20) {
        m_buffer.clear();
    }
}

void ArduinoSerial::parseReceivedData(const QString& data)
{
    QString trimmedData = data.trimmed();
    
    if (trimmedData.length() == 8) {
        QRegularExpression cinRegex("^\\d{8}$");
        QRegularExpressionMatch match = cinRegex.match(trimmedData);
        
        if (match.hasMatch()) {
            QString cin = match.captured(0);
            qDebug() << "CIN complet recu depuis Arduino:" << cin;
            emit cinReceived(cin);
            return;
        }
    }
    
    if (trimmedData.length() > 0 && trimmedData.length() < 8) {
        qDebug() << "Donnees partielles recues (en attente):" << trimmedData;
    } else if (trimmedData.length() > 8) {
        QString cin = trimmedData.left(8);
        QRegularExpression cinRegex("^\\d{8}$");
        if (cinRegex.match(cin).hasMatch()) {
            qDebug() << "CIN extrait depuis donnees:" << cin;
            emit cinReceived(cin);
        } else {
            qDebug() << "Donnees recues (format invalide):" << trimmedData;
        }
    } else {
        qDebug() << "Donnees recues (non CIN):" << trimmedData;
    }
}

void ArduinoSerial::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        m_isConnected = false;
        emit connectionStatusChanged(false);
        emit errorOccurred("Erreur de connexion serie");
        qDebug() << "Erreur serie:" << m_serialPort->errorString();
    }
}

