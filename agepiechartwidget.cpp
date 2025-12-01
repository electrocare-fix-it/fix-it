#include "agepiechartwidget.h"
#include <QPainter>
#include <QTimer>
#include <QFontMetrics>
#include <cmath>
#include <algorithm>

AgePieChartWidget::AgePieChartWidget(const QMap<QString, int>& ageData, int total, QWidget *parent)
    : QWidget(parent), m_ageData(ageData), m_total(total), m_animationStep(0)
{
    setMinimumSize(500, 400);
    setAttribute(Qt::WA_TranslucentBackground);
    
    for (const QString& key : m_ageData.keys()) {
        m_animatedAngles[key] = 0.0;
    }
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &AgePieChartWidget::updateAnimation);
    
    startAnimation();
}

void AgePieChartWidget::setData(const QMap<QString, int>& ageData, int total)
{
    m_ageData = ageData;
    m_total = total;
    
    for (const QString& key : m_ageData.keys()) {
        if (!m_animatedAngles.contains(key)) {
            m_animatedAngles[key] = 0.0;
        }
    }
    
    update();
}

QColor AgePieChartWidget::getColor(int index) const
{
    QList<QColor> colors = {
        QColor(255, 140, 0),
        QColor(70, 130, 180),
        QColor(34, 139, 34),
        QColor(220, 20, 60),
        QColor(255, 165, 0),
        QColor(148, 0, 211),
        QColor(255, 20, 147),
        QColor(0, 191, 255),
        QColor(255, 69, 0),
        QColor(50, 205, 50)
    };
    return colors[index % colors.size()];
}

QColor AgePieChartWidget::getColorForCategory(const QString& category) const
{
    if (category == "Moins de 20 ans") {
        return QColor(70, 130, 180);
    }
    
    QList<QString> sortedKeys = m_ageData.keys();
    std::sort(sortedKeys.begin(), sortedKeys.end());
    
    QList<QColor> colors = {
        QColor(255, 140, 0),
        QColor(34, 139, 34),
        QColor(220, 20, 60),
        QColor(255, 165, 0),
        QColor(148, 0, 211),
        QColor(255, 20, 147),
        QColor(0, 191, 255),
        QColor(255, 69, 0),
        QColor(50, 205, 50)
    };
    
    int colorIndex = 0;
    for (const QString& key : sortedKeys) {
        if (key == category) {
            return colors[colorIndex % colors.size()];
        }
        if (key != "Moins de 20 ans") {
            colorIndex++;
        }
    }
    
    return colors[0];
}

void AgePieChartWidget::updateAnimation()
{
    m_animationStep = (m_animationStep + 1) % 120;
    
    bool needsUpdate = false;
    for (const QString& key : m_ageData.keys()) {
        double targetAngle = m_total > 0 ? (m_ageData[key] * 360.0) / m_total : 0.0;
        double currentAngle = m_animatedAngles[key];
        double diff = targetAngle - currentAngle;
        
        if (qAbs(diff) > 0.5) {
            m_animatedAngles[key] = currentAngle + diff * 0.15;
            needsUpdate = true;
        } else {
            m_animatedAngles[key] = targetAngle;
        }
    }
    
    if (needsUpdate) {
        update();
    }
}

void AgePieChartWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int margin = 40;
    int legendWidth = 240;
    int chartSize = qMin(width() - legendWidth - 3 * margin, height() - 2 * margin - 40);
    chartSize = qMax(chartSize, 250);
    
    int centerX = margin + chartSize / 2;
    int centerY = margin + chartSize / 2 + 20;
    QRect chartRect(centerX - chartSize / 2, centerY - chartSize / 2, chartSize, chartSize);
    
    if (m_total == 0 || m_ageData.isEmpty()) {
        QFont font("Arial", 14);
        painter.setFont(font);
        painter.setPen(QPen(QColor(100, 100, 100)));
        painter.drawText(QRect(0, 0, width(), height()), Qt::AlignCenter, "Aucune donnée disponible");
        return;
    }
    
    QList<QString> sortedKeys = m_ageData.keys();
    std::sort(sortedKeys.begin(), sortedKeys.end());
    
    double startAngle = 0;
    int colorIndex = 0;
    
    int pulseAlpha = 220 + static_cast<int>(35 * qSin(m_animationStep * M_PI / 60.0));
    
    for (const QString& key : sortedKeys) {
        double angle = m_animatedAngles[key];
        
        if (angle > 0.1) {
            QColor color = getColorForCategory(key);
            color.setAlpha(pulseAlpha);
            
            QPen pen(Qt::white, 3);
            pen.setJoinStyle(Qt::RoundJoin);
            painter.setPen(pen);
            painter.setBrush(QBrush(color));
            painter.drawPie(chartRect, startAngle * 16, angle * 16);
            
            startAngle += angle;
        }
        
        colorIndex++;
    }
    
    int centerRadius = chartSize / 4;
    QRect centerRect(centerX - centerRadius, centerY - centerRadius, centerRadius * 2, centerRadius * 2);
    
    painter.setPen(QPen(QColor(240, 240, 240), 2));
    painter.setBrush(QBrush(QColor(255, 255, 255)));
    painter.drawEllipse(centerRect);
    
    QFont totalFont("Arial", 18, QFont::Bold);
    painter.setFont(totalFont);
    painter.setPen(QPen(QColor(0, 0, 139)));
    
    QString totalText = QString::number(m_total);
    QFontMetrics totalFm(totalFont);
    QRect totalTextRect = totalFm.boundingRect(totalText);
    
    painter.drawText(centerX - totalTextRect.width() / 2, 
                    centerY - totalTextRect.height() / 2 + totalTextRect.height() / 4, 
                    totalText);
    
    QFont labelFont("Arial", 11, QFont::Normal);
    painter.setFont(labelFont);
    painter.setPen(QPen(QColor(100, 100, 100)));
    
    QString labelText = "Total";
    QFontMetrics labelFm(labelFont);
    QRect labelTextRect = labelFm.boundingRect(labelText);
    
    painter.drawText(centerX - labelTextRect.width() / 2, 
                    centerY + totalTextRect.height() / 2 + 5, 
                    labelText);
    
    int legendX = margin + chartSize + margin;
    int legendY = margin + 30;
    int yPos = legendY;
    
    QFont legendTitleFont("Arial", 13, QFont::Bold);
    painter.setFont(legendTitleFont);
    painter.setPen(QPen(QColor(13, 71, 161)));
    painter.drawText(legendX, yPos, "Tranches d'âge");
    yPos += 30;
    
    QFont legendFont("Arial", 11);
    painter.setFont(legendFont);
    
    colorIndex = 0;
    for (const QString& key : sortedKeys) {
        int value = m_ageData[key];
        if (value == 0) continue;
        
        QColor color = getColorForCategory(key);
        color.setAlpha(255);
        
        QRect colorRect(legendX, yPos, 20, 20);
        painter.setBrush(QBrush(color));
        painter.setPen(QPen(QColor(50, 50, 50), 1.5));
        painter.drawRoundedRect(colorRect, 3, 3);
        
        double percent = m_total > 0 ? (value * 100.0) / m_total : 0.0;
        QString legendText = QString("%1").arg(key);
        QString valueText = QString("%2 (%3%)").arg(value).arg(percent, 0, 'f', 1);
        
        painter.setPen(QPen(QColor(33, 33, 33)));
        QFontMetrics fm(legendFont);
        painter.drawText(legendX + 28, yPos + 14, legendText);
        
        QFont valueFont("Arial", 10, QFont::Bold);
        painter.setFont(valueFont);
        painter.setPen(QPen(QColor(13, 71, 161)));
        painter.drawText(legendX + 28, yPos + 28, valueText);
        painter.setFont(legendFont);
        
        yPos += 38;
        colorIndex++;
    }
}

void AgePieChartWidget::startAnimation()
{
    m_timer->start(50);
}

