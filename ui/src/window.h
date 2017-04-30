#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include "processdisplay.h"
#include "totalprocess.h"
#include "orderlauncher.h"
#include "Plazza.hpp"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(std::int32_t threadNb, QWidget *parent = 0);
    virtual ~Window();

private:
    ProcessDisplay *m_process;
    TotalProcess *m_total;
    OrderLauncher *m_order;
    QVBoxLayout *m_layout;
    QTimer *m_timer;
    QTimer *m_timer2;
    Plazza m_plazza;

signals:

public slots:
    void update();
    void updatePlazza();
    void orderLaunched(QString s);
};

#endif // WINDOW_H
