#ifndef AGEPIECHARTWIDGET_H
#define AGEPIECHARTWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QString>
#include <QMap>

class AgePieChartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AgePieChartWidget(const QMap<QString, int>& ageData, int total, QWidget *parent = nullptr);
    void setData(const QMap<QString, int>& ageData, int total);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QMap<QString, int> m_ageData;
    int m_total;
    QTimer* m_timer;
    int m_animationStep;
    QMap<QString, double> m_animatedAngles;
    void startAnimation();
    QColor getColor(int index) const;
    QColor getColorForCategory(const QString& category) const;
    void updateAnimation();
};

#endif // AGEPIECHARTWIDGET_H

