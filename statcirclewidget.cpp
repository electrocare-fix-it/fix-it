#include "statcirclewidget.h"
#include <QPainter>
#include <QTimer>
#include <QFontMetrics>
#include <cmath>

StatCircleWidget::StatCircleWidget(int value, int total, const QString& label, const QString& category1, const QString& category2, QWidget *parent)
    : QWidget(parent), m_value(value), m_total(total), m_label(label), 
      m_category1(category1), m_category2(category2), m_animationStep(0), m_animatedAngle(0.0)
{
    setMinimumSize(300, 350);
    setAttribute(Qt::WA_TranslucentBackground);
    
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        m_animationStep = (m_animationStep + 1) % 120;
        
        double targetAngle = m_total > 0 ? (m_value * 360.0) / m_total : 0.0;
        double diff = targetAngle - m_animatedAngle;
        if (qAbs(diff) > 1.0) {
            m_animatedAngle += diff * 0.1;
        } else {
            m_animatedAngle = targetAngle;
        }
        
        update();
    });
    
    startAnimation();
}

void StatCircleWidget::setValue(int value)
{
    if (m_value != value) {
        m_value = value;
        update();
    }
}

void StatCircleWidget::setTotal(int total)
{
    if (m_total != total) {
        m_total = total;
        update();
    }
}

QColor StatCircleWidget::getColor(int index) const
{
    QList<QColor> colors = {
        QColor(255, 140, 0),
        QColor(70, 130, 180),
        QColor(34, 139, 34),
        QColor(220, 20, 60),
        QColor(255, 165, 0),
        QColor(148, 0, 211),
        QColor(255, 20, 147),
        QColor(0, 191, 255)
    };
    return colors[index % colors.size()];
}

void StatCircleWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int margin = 20;
    int legendWidth = 180;
    int chartSize = qMin(width() - legendWidth - 3 * margin, height() - 2 * margin);
    
    int centerX = margin + chartSize / 2;
    int centerY = margin + chartSize / 2;
    QRect chartRect(centerX - chartSize / 2, centerY - chartSize / 2, chartSize, chartSize);
    
    if (m_total == 0) {
        QFont font("Arial", 12);
        painter.setFont(font);
        painter.setPen(QPen(QColor(100, 100, 100)));
        painter.drawText(chartRect, Qt::AlignCenter, "Aucune donnée");
        return;
    }
    
    if (m_value == m_total && m_category2 == "Aucun") {
        QColor fullColor = getColor(0);
        int pulseAlpha = 200 + static_cast<int>(55 * qSin(m_animationStep * M_PI / 60.0));
        fullColor.setAlpha(pulseAlpha);
        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(QBrush(fullColor));
        painter.drawEllipse(chartRect);
        
        QFont valueFont("Arial", 24, QFont::Bold);
        painter.setFont(valueFont);
        painter.setPen(QPen(QColor(0, 0, 139), 2));
        QString valueText = QString::number(m_value);
        QFontMetrics fm(valueFont);
        QRect textRect = fm.boundingRect(valueText);
        painter.drawText(chartRect.center().x() - textRect.width() / 2, 
                        chartRect.center().y() + textRect.height() / 4, 
                        valueText);
    } else {
        double angle1 = m_animatedAngle;
        double angle2 = 360.0 - m_animatedAngle;
        
        QColor color1 = getColor(0);
        QColor color2 = getColor(1);
        
        int pulseAlpha = 200 + static_cast<int>(55 * qSin(m_animationStep * M_PI / 60.0));
        color1.setAlpha(pulseAlpha);
        color2.setAlpha(180);
        
        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(QBrush(color1));
        painter.drawPie(chartRect, 0, angle1 * 16);
        
        painter.setBrush(QBrush(color2));
        painter.drawPie(chartRect, angle1 * 16, angle2 * 16);
    }
    
    if (m_value != m_total || m_category2 != "Aucun") {
        int legendX = margin + chartSize + margin;
        int legendY = margin;
        int yPos = legendY;
        
        QFont legendFont("Arial", 10);
        painter.setFont(legendFont);
        
        QColor color1 = getColor(0);
        QColor color2 = getColor(1);
        
        QRect colorRect1(legendX, yPos, 15, 15);
        painter.setBrush(QBrush(color1));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(colorRect1);
        
        double percent1 = m_total > 0 ? (m_value * 100.0) / m_total : 0.0;
        QString legendText1 = QString("%1: %2 (%3%)").arg(m_category1).arg(m_value).arg(percent1, 0, 'f', 1);
        painter.setPen(QPen(Qt::black));
        painter.drawText(legendX + 20, yPos + 12, legendText1);
        
        yPos += 25;
        
        QRect colorRect2(legendX, yPos, 15, 15);
        painter.setBrush(QBrush(color2));
        painter.setPen(QPen(Qt::black, 1));
        painter.drawRect(colorRect2);
        
        int value2 = m_total - m_value;
        double percent2 = m_total > 0 ? (value2 * 100.0) / m_total : 0.0;
        QString legendText2 = QString("%1: %2 (%3%)").arg(m_category2).arg(value2).arg(percent2, 0, 'f', 1);
        painter.setPen(QPen(Qt::black));
        painter.drawText(legendX + 20, yPos + 12, legendText2);
    }
    
    QFont titleFont("Arial", 12, QFont::Bold);
    painter.setFont(titleFont);
    painter.setPen(QPen(QColor(13, 71, 161)));
    QFontMetrics titleFm(titleFont);
    QRect titleRect(0, height() - 30, width(), 25);
    painter.drawText(titleRect, Qt::AlignCenter, m_label);
}

void StatCircleWidget::startAnimation()
{
    m_timer->start(50);
}
