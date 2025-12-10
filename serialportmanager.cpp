#include "serialportmanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QSerialPortInfo>
#include <QTimer>

SerialPortManager::SerialPortManager(QObject *parent)
    : QObject(parent)
    , m_serialPort(new QSerialPort(this))
    , m_isConnected(false)
    , m_checkTimer(new QTimer(this))
{
    qDebug() << "=== SerialPortManager CONSTRUCTEUR ===";
    
    // Connecter le signal readyRead
    bool connected = connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortManager::readData);
    qDebug() << "Signal readyRead connecte:" << connected;
    
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortManager::handleError);
#else
    connect(m_serialPort, static_cast<void(QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &SerialPortManager::handleError);
#endif
    
    // Créer un timer pour vérifier périodiquement les données (au cas où readyRead ne fonctionne pas)
    connect(m_checkTimer, &QTimer::timeout, this, &SerialPortManager::checkForData);
    m_checkTimer->setInterval(100); // Vérifier toutes les 100ms
    
    qDebug() << "SerialPortManager initialise";
}

SerialPortManager::~SerialPortManager()
{
    disconnectFromArduino();
}

bool SerialPortManager::connectToArduino(const QString &portName, int baudRate)
{
    // ============================================
    // SOLUTION AU CONFLIT : Attendre que le port soit libre
    // Le port peut être utilisé par gestionobjetelectronique (ArduinoSerial)
    // ============================================
    
    qDebug() << "=== DÉBUT CONNEXION ARDUINO RFID ===";
    
    // Fermer le port s'il est déjà ouvert
    if (m_serialPort->isOpen()) {
        m_serialPort->close();
        QThread::msleep(200);
    }
    
    m_isConnected = false;
    m_currentPort.clear();

    // Obtenir la liste des ports disponibles
    QStringList portsToTry;
    if (portName.isEmpty()) {
        portsToTry = getAvailablePorts();
        if (portsToTry.isEmpty()) {
            emit errorOccurred("Aucun port série disponible.\n\nVérifiez que:\n- L'Arduino est branché via USB\n- Les pilotes USB sont installés");
            return false;
        }
        qDebug() << "Ports détectés:" << portsToTry;
    } else {
        portsToTry << portName;
        qDebug() << "Port spécifié:" << portName;
    }

    // Essayer chaque port disponible
    for (const QString &currentPort : portsToTry) {
        qDebug() << "Tentative de connexion au port:" << currentPort;
        
        // NOUVEAU: Vérifier si le port est déjà utilisé et attendre qu'il soit libre
        // Le port peut être utilisé par gestionobjetelectronique (ArduinoSerial)
        qDebug() << "Vérification si le port est libre...";
        int maxWaitAttempts = 20; // Attendre jusqu'à 4 secondes (20 * 200ms)
        bool portIsFree = false;
        
        for (int waitAttempt = 0; waitAttempt < maxWaitAttempts; waitAttempt++) {
            // Essayer d'ouvrir le port en mode test pour voir s'il est libre
            QSerialPort testPort;
            testPort.setPortName(currentPort);
            
            if (testPort.open(QIODevice::ReadWrite)) {
                testPort.close();
                portIsFree = true;
                qDebug() << "Port" << currentPort << "est maintenant libre!";
                QThread::msleep(300); // Attendre un peu après la libération
                break;
            } else {
                if (waitAttempt == 0) {
                    qDebug() << "Port" << currentPort << "est occupé, attente de libération...";
                }
                QThread::msleep(200); // Attendre 200ms avant de réessayer
            }
        }
        
        if (!portIsFree) {
            qDebug() << "Port" << currentPort << "toujours occupé après" << (maxWaitAttempts * 200) << "ms";
            // Continuer quand même, peut-être que ça va fonctionner
        }
        
        // Fermer le port s'il est ouvert
        if (m_serialPort->isOpen()) {
            m_serialPort->close();
            QThread::msleep(100);
        }
        
        // Configurer le port
        m_serialPort->setPortName(currentPort);
        m_serialPort->setBaudRate(baudRate);
        m_serialPort->setDataBits(QSerialPort::Data8);
        m_serialPort->setParity(QSerialPort::NoParity);
        m_serialPort->setStopBits(QSerialPort::OneStop);
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
        
        // Essayer d'ouvrir le port
        if (m_serialPort->open(QIODevice::ReadWrite)) {
            m_currentPort = currentPort;
            m_isConnected = true;
            emit connectionStatusChanged(true);
            qDebug() << "✓✓✓ SUCCÈS: Connecté à l'Arduino sur le port:" << currentPort;
            qDebug() << "Port ouvert en mode ReadWrite";
            qDebug() << "Bytes disponibles:" << m_serialPort->bytesAvailable();
            
            // S'assurer que le signal readyRead est connecté (reconnecter pour être sûr)
            disconnect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortManager::readData);
            connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortManager::readData);
            qDebug() << "Signal readyRead connecte";
            
            // Attendre un peu pour stabiliser
            QThread::msleep(500);
            
            // Vider le buffer
            m_serialPort->clear();
            
            // Tester la lecture
            qDebug() << "Test de lecture - Bytes disponibles:" << m_serialPort->bytesAvailable();
            
            // Démarrer le timer de vérification
            m_checkTimer->start();
            qDebug() << "Timer de verification demarre (toutes les 100ms)";
            
            // Test de connexion : envoyer un message de test (optionnel)
            qDebug() << "=== CONNEXION ARDUINO ETABLIE ===";
            qDebug() << "Port:" << currentPort;
            qDebug() << "BaudRate: 9600";
            qDebug() << "En attente de messages de l'Arduino...";
            qDebug() << "Pour tester, scannez une carte RFID";
            
            return true;
        } else {
            QString errorMsg = m_serialPort->errorString();
            QSerialPort::SerialPortError error = m_serialPort->error();
            qDebug() << "Échec sur" << currentPort << ":" << errorMsg << "(code:" << error << ")";
            
            // Si c'est le dernier port, émettre l'erreur avec message spécifique
            if (currentPort == portsToTry.last()) {
                QString errorText = QString("Impossible d'ouvrir le port %1: %2\n\n"
                                       "Le port est peut-être utilisé par:\n"
                                       "- L'interface 'Gestion Objets Électroniques' (ArduinoSerial)\n"
                                       "- Arduino IDE (Moniteur Série)\n"
                                       "- Un autre programme\n\n"
                                       "Solutions:\n"
                                       "1. Fermez l'interface 'Gestion Objets Électroniques' si elle est ouverte\n"
                                       "2. Fermez Arduino IDE complètement\n"
                                       "3. Fermez tous les Moniteurs Série\n"
                                       "4. Redémarrez l'application Qt")
                               .arg(currentPort)
                               .arg(errorMsg);
                emit errorOccurred(errorText);
            }
        }
    }
    
    m_isConnected = false;
    emit connectionStatusChanged(false);
    return false;
}

void SerialPortManager::disconnectFromArduino()
{
    // Arrêter le timer
    if (m_checkTimer) {
        m_checkTimer->stop();
    }
    
    if (m_serialPort) {
        if (m_serialPort->isOpen()) {
            m_serialPort->clear();
            m_serialPort->close();
            QThread::msleep(200); // Attendre que le port soit libéré
        }
        m_isConnected = false;
        m_currentPort.clear();
        emit connectionStatusChanged(false);
        qDebug() << "Déconnecté de l'Arduino";
    }
}

void SerialPortManager::checkForData()
{
    // Vérifier périodiquement s'il y a des données disponibles
    if (m_serialPort && m_serialPort->isOpen() && m_serialPort->bytesAvailable() > 0) {
        qDebug() << "Timer detecte des donnees disponibles:" << m_serialPort->bytesAvailable() << "bytes";
        readData();
    }
}

bool SerialPortManager::isConnected() const
{
    return m_isConnected && m_serialPort->isOpen();
}

QStringList SerialPortManager::getAvailablePorts() const
{
    QStringList ports;
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    
    for (const QSerialPortInfo &portInfo : serialPortInfos) {
        QString portName = portInfo.portName();
        // Filtrer les ports série pertinents (généralement COM3, COM4, etc. sur Windows)
        if (!portName.isEmpty()) {
            ports << portName;
        }
    }
    
    return ports;
}

bool SerialPortManager::isPortAvailable(const QString &portName) const
{
    // Vérifier si le port existe dans la liste des ports disponibles
    QStringList availablePorts = getAvailablePorts();
    return availablePorts.contains(portName);
}

bool SerialPortManager::waitForPortAvailable(const QString &portName, int maxWaitMs) const
{
    // Version simplifiée - juste vérifier que le port existe
    Q_UNUSED(maxWaitMs);
    return isPortAvailable(portName);
}

QString SerialPortManager::getCurrentPort() const
{
    return m_currentPort;
}

void SerialPortManager::readData()
{
    qDebug() << "========================================";
    qDebug() << "*** readData() APPELE ***";
    qDebug() << "Port ouvert:" << m_serialPort->isOpen();
    qDebug() << "Bytes disponibles:" << m_serialPort->bytesAvailable();
    
    // Vérifier s'il y a des données disponibles
    if (m_serialPort->bytesAvailable() == 0) {
        qDebug() << "Aucune donnee disponible";
        qDebug() << "========================================";
        return;
    }
    
    // Lire toutes les données disponibles
    QByteArray data = m_serialPort->readAll();
    
    qDebug() << "Donnees lues - Taille:" << data.size();
    
    if (data.isEmpty()) {
        qDebug() << "Donnees vides apres readAll()";
        qDebug() << "========================================";
        return;
    }

    QString message = QString::fromUtf8(data).trimmed();
    
    if (message.isEmpty()) {
        qDebug() << "Message vide apres trim";
        qDebug() << "========================================";
        return;
    }

    qDebug() << "========================================";
    qDebug() << "*** DONNEES RECUES DE L'ARDUINO ***";
    qDebug() << "Taille des donnees:" << data.size();
    qDebug() << "Donnees brutes (hex):" << data.toHex();
    qDebug() << "Donnees brutes (ascii):" << data;
    qDebug() << "Message (string):" << message;
    qDebug() << "========================================";
    
    // Traiter chaque ligne
    QStringList lines = message.split('\n', Qt::SkipEmptyParts);
    qDebug() << "Nombre de lignes:" << lines.size();
    
    for (int i = 0; i < lines.size(); i++) {
        QString trimmedLine = lines[i].trimmed();
        if (!trimmedLine.isEmpty()) {
            qDebug() << "Ligne" << i << ":" << trimmedLine;
            parseArduinoMessage(trimmedLine);
        }
    }
    qDebug() << "========================================";
}

void SerialPortManager::parseArduinoMessage(const QString &message)
{
    // Format attendu: ACCESS_GRANTED|EMPLOYEE_NAME|STATUS
    // ou: ACCESS_DENIED|EMPLOYEE_NAME|STATUS
    
    qDebug() << "========================================";
    qDebug() << "*** PARSING MESSAGE ***";
    qDebug() << "Message complet:" << message;
    qDebug() << "Longueur du message:" << message.length();
    
    // Chercher ACCESS_GRANTED ou ACCESS_DENIED même s'il y a du texte avant
    int grantedPos = message.indexOf("ACCESS_GRANTED|");
    int deniedPos = message.indexOf("ACCESS_DENIED|");
    
    qDebug() << "Position ACCESS_GRANTED|:" << grantedPos;
    qDebug() << "Position ACCESS_DENIED|:" << deniedPos;
    
    if (grantedPos >= 0) {
        QString relevantPart = message.mid(grantedPos);
        QStringList parts = relevantPart.split("|");
        qDebug() << ">>> ACCESS_GRANTED trouve <<<";
        qDebug() << "Parties:" << parts;
        qDebug() << "Nombre de parties:" << parts.size();
        
        if (parts.size() >= 3) {
            QString employeeName = parts[1].trimmed();
            QString status = parts[2].trimmed();
            qDebug() << "*** EMISSION SIGNAL accessGranted ***";
            qDebug() << "Employee:" << employeeName;
            qDebug() << "Status:" << status;
            emit accessGranted(employeeName, status);
            qDebug() << "Signal accessGranted EMIS avec succes";
        } else {
            qDebug() << "!!! ERREUR: Pas assez de parties dans ACCESS_GRANTED !!!";
            qDebug() << "Parties trouvees:" << parts;
        }
    } else if (deniedPos >= 0) {
        QString relevantPart = message.mid(deniedPos);
        QStringList parts = relevantPart.split("|");
        qDebug() << ">>> ACCESS_DENIED trouve <<<";
        qDebug() << "Parties:" << parts;
        qDebug() << "Nombre de parties:" << parts.size();
        
        if (parts.size() >= 3) {
            QString employeeName = parts[1].trimmed();
            QString status = parts[2].trimmed();
            qDebug() << "*** EMISSION SIGNAL accessDenied ***";
            qDebug() << "Employee:" << employeeName;
            qDebug() << "Status:" << status;
            emit accessDenied(employeeName, status);
            qDebug() << "Signal accessDenied EMIS avec succes";
        } else {
            qDebug() << "!!! ERREUR: Pas assez de parties dans ACCESS_DENIED !!!";
            qDebug() << "Parties trouvees:" << parts;
        }
    } else {
        qDebug() << "!!! Message ne contient PAS ACCESS_GRANTED| ou ACCESS_DENIED| !!!";
        qDebug() << "Message recherche:" << message;
    }
    qDebug() << "========================================";
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qDebug() << "Erreur série:" << m_serialPort->errorString();
        emit errorOccurred(m_serialPort->errorString());
        disconnectFromArduino();
    }
}

