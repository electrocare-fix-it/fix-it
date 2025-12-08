#ifndef STATCIRCLEWIDGET_H
#define STATCIRCLEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QString>
#include <QMap>

class StatCircleWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatCircleWidget(int value, int total, const QString& label, const QString& category1, const QString& category2, QWidget *parent = nullptr);
    void setValue(int value);
    void setTotal(int total);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_value;
    int m_total;
    QString m_label;
    QString m_category1;
    QString m_category2;
    QTimer* m_timer;
    int m_animationStep;
    double m_animatedAngle;
    void startAnimation();
    QColor getColor(int index) const;
};

#endif // STATCIRCLEWIDGET_H

