#ifndef STATEINDICATOR_H
#define STATEINDICATOR_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QPropertyAnimation>

class StateIndicator : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int pulseSize READ getPulseSize WRITE setPulseSize)

public:
    explicit StateIndicator(const QString& state, QWidget *parent = nullptr);
    void setState(const QString& state);
    QString getState() const;
    int getPulseSize() const { return m_pulseSize; }
    void setPulseSize(int size);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_state;
    QTimer* m_timer;
    int m_pulseSize;
    int m_animationStep;
    QColor getStateColor(const QString& state) const;
    void startAnimation();
};

#endif // STATEINDICATOR_H

