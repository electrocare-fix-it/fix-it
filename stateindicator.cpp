#include "stateindicator.h"
#include <QPainter>
#include <QTimer>
#include <cmath>

StateIndicator::StateIndicator(const QString& state, QWidget *parent)
    : QWidget(parent), m_state(state), m_pulseSize(0), m_animationStep(0)
{
    setFixedSize(20, 20);
    setAttribute(Qt::WA_TranslucentBackground);
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        m_animationStep = (m_animationStep + 1) % 60;
        update();
    });
    
    startAnimation();
}

void StateIndicator::setState(const QString& state)
{
    if (m_state != state) {
        m_state = state;
        update();
    }
}

QString StateIndicator::getState() const
{
    return m_state;
}

void StateIndicator::setPulseSize(int size)
{
    m_pulseSize = size;
    update();
}

void StateIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QColor baseColor = getStateColor(m_state);
    
    int centerX = width() / 2;
    int centerY = height() / 2;
    int baseRadius = 6;
    
    double pulseFactor = 1.0 + 0.3 * qSin(m_animationStep * M_PI / 30.0);
    int currentRadius = static_cast<int>(baseRadius * pulseFactor);
    
    QColor circleColor = baseColor;
    circleColor.setAlpha(200 + static_cast<int>(55 * qSin(m_animationStep * M_PI / 30.0)));
    
    painter.setBrush(QBrush(circleColor));
    painter.setPen(QPen(baseColor.darker(120), 1));
    painter.drawEllipse(centerX - currentRadius, centerY - currentRadius, 
                       currentRadius * 2, currentRadius * 2);
    
    QColor innerColor = baseColor.lighter(150);
    innerColor.setAlpha(180);
    painter.setBrush(QBrush(innerColor));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(centerX - baseRadius/2, centerY - baseRadius/2, 
                       baseRadius, baseRadius);
}

QColor StateIndicator::getStateColor(const QString& state) const
{
    QString stateLower = state.toLower();
    
    if (stateLower.contains("en cours") || stateLower.contains("en réparation")) {
        return QColor(255, 165, 0);
    } else if (stateLower.contains("terminé") || stateLower.contains("réparé") || stateLower.contains("prêt")) {
        return QColor(34, 139, 34);
    } else if (stateLower.contains("en attente") || stateLower.contains("en attente")) {
        return QColor(255, 140, 0);
    } else if (stateLower.contains("annulé") || stateLower.contains("annule")) {
        return QColor(220, 20, 60);
    } else {
        return QColor(70, 130, 180);
    }
}

void StateIndicator::startAnimation()
{
    m_timer->start(50);
}

