#include "window.h"
#include <QApplication>
#include <sstream>
#include <QTimer>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <unistd.h>
#include <QLabel>
#include "Response.hpp"

bool getResponse(std::vector<Response> &r)
{
  static std::size_t s = 0;

  if (s == 0)
    {
      s = rand() % 19 + 1;
      s = 13;
    }

  r.clear();
  if (r.size() == 0)
    {
      for (std::size_t i = 0; i < s; ++i)
    {
      r.emplace_back(rand() % 2 ? true : false, rand() % 5, rand() % 2);
    }
    }
  return (true);
}

Window::Window(std::int32_t threadNb, QWidget *parent) :
    QWidget(parent),
    m_process(new ProcessDisplay(this)),
    m_total(new TotalProcess(this)),
    m_order(new OrderLauncher(this)),
    m_layout(new QVBoxLayout(this)),
    m_timer(new QTimer(this)),
    m_timer2(new QTimer(this)),
    m_plazza(threadNb)
{
    // Set the window size
    setFixedSize(1280, 720);

    // Set the timer to call update() each x milliseconds
    constexpr int x = 200;

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    m_timer->start(x);

    QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(updatePlazza()));
    m_timer->start(1);

    QObject::connect(m_order, SIGNAL(launchOrder(QString)), this, SLOT(orderLaunched(QString)));

    m_layout->addWidget(m_total);
    m_layout->addWidget(m_process);
    m_layout->addWidget(m_order);
    setLayout(m_layout);

    this->setStyleSheet("QGroupBox {"
                        "   border: 1px solid lightgray;"
                        "   border-radius: 5px;"
                        "   margin-top: 0.5em;"
                        "   background: #05000000;"
                        "}"
                        "QGroupBox::title {"
                        "   subcontrol-origin: margin;"
                        "   left: 10px;"
                        "   padding: 0 3px 0 3px;"
                        "}"
                        "QScrollArea {"
                        "    border: 0px;"
                        "}"
                        "QScrollBar:vertical {"
                        "   border: 0px;"
                        "   background:transparent;"
                        "   width:10px;    "
                        "   margin: 0px 0px 0px 0px;"
                        "}"
                        "QScrollBar::handle:vertical {"
                        "   background: gray;"
                        "   min-height: 0px;"
                        "   border-radius: 5px;"
                        "}"
                        "QScrollBar::add-line:vertical {"
                        "   height: 0px;"
                        "   subcontrol-position: bottom;"
                        "   subcontrol-origin: margin;"
                        "}"
                        "QScrollBar::sub-line:vertical {"
                        "   height: 0 px;"
                        "   subcontrol-position: top;"
                        "   subcontrol-origin: margin;"
                        "}");
}

Window::~Window()
{
}

void Window::updatePlazza()
{
    m_plazza.update();
}

void Window::update()
{
    std::vector<Response> resp = m_plazza.getResponse();

    m_process->updateData(resp);
    m_total->updateData(resp);
}

void Window::orderLaunched(QString s)
{
    m_plazza.pushOrder(s.toStdString());
}
