#include <sstream>
#include <iomanip>
#include "wprocess.h"

std::size_t WProcess::threadPerProcess = 0;

WProcess::WProcess(std::size_t n) :
    QGroupBox(),
    m_busy(new QLabel(this)),
    m_waiting(new QLabel(this)),
    m_layout(new QVBoxLayout(this))
{
    this->updateId(n);
    m_layout->addWidget(m_busy);
    m_layout->addWidget(m_waiting);
    this->setLayout(m_layout);
    m_busy->setStyleSheet("background: transparent;");
    m_waiting->setStyleSheet("background: transparent;");
    //this->setMinimumSize(200, 200);
}

WProcess::~WProcess()
{

}

void WProcess::updateId(std::size_t n)
{
    std::stringstream ss;

    ss << "Process " << n;
    this->setTitle(tr(ss.str().c_str()));
}

void WProcess::updateData(Response const &r)
{
    std::stringstream ss;
    double workload = 0.0;

    if (threadPerProcess != 0)
    {
        workload = static_cast<double>(r.getBusy()) / threadPerProcess * 100;
    }

    ss << "Workload : " << std::fixed << std::setprecision(2)
       << workload << "% (" << r.getBusy() << '/' << threadPerProcess << ')';

    m_busy->setText(tr(ss.str().c_str()));

    ss.str("");

    ss << "Waiting order : " << r.getWaiting();

    m_waiting->setText(tr(ss.str().c_str()));
}
