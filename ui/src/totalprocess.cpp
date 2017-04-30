#include <sstream>
#include "totalprocess.h"

TotalProcess::TotalProcess(QWidget *parent) :
    QGroupBox(tr("Global"), parent),
    m_layout(new QVBoxLayout(this)),
    m_nbProcess(new QLabel(this)),
    m_busyProcess(new QLabel(this))
{
    m_layout->addWidget(m_nbProcess);
    m_layout->addWidget(m_busyProcess);
    this->setLayout(m_layout);
    this->setMaximumHeight(100);
}

TotalProcess::~TotalProcess()
{
}

void TotalProcess::updateData(const std::vector<Response> &resp)
{
    std::size_t count = 0;
    std::size_t total = resp.size();

    // Count the number of busy process
    for (Response const &r : resp)
    {
        if (r.getBusy() > 0)
        {
            ++count;
        }
    }

    // Strinstream to store and compute the resulting string
    std::stringstream ss;

    ss << "Number of process : " << total;

    m_nbProcess->setText(tr(ss.str().c_str()));

    // Clear the stringstream
    ss.str("");

    ss << "Number of busy process : " << count;

    m_busyProcess->setText(tr(ss.str().c_str()));
}
