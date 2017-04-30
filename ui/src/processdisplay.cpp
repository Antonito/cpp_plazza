#include "processdisplay.h"

ProcessDisplay::ProcessDisplay(QWidget *parent) :
    QGroupBox(tr("Process"), parent),
    m_layout(new QGridLayout(this)),
    m_area(new QScrollArea(this)),
    m_dummy(new QWidget(this)),
    m_blayout(new QVBoxLayout(this))
{
    for (std::size_t i = 0; i < 10; ++i)
    {
        if (i < 5)
        {
            m_layout->setColumnMinimumWidth(i, 1210 / 5);
        }
        m_layout->setRowMinimumHeight(i, 500 / 5 - 15);
    }
    m_dummy->setLayout(m_layout);
    m_area->setWidget(m_dummy);
    m_blayout->addWidget(m_area);
    this->setLayout(m_blayout);
    m_dummy->setStyleSheet("background: #05000000;");
}

ProcessDisplay::~ProcessDisplay()
{
    //asm("int $3;");
}

void ProcessDisplay::updateData(const std::vector<Response> &resp)
{
    std::size_t n = 0;

    for (Response const &r : resp)
    {
        if (n == m_process.size())
        {
            m_process.emplace_back(new WProcess(n));
            m_layout->addWidget(m_process.back().get(), n / 5, n % 5);
        }
        m_process[n]->updateData(r);
        ++n;
    }

    std::size_t oldSize = m_process.size();
    m_process.resize(n);
    for (std::size_t i = n; i < oldSize; ++i)
    {
        m_layout->takeAt(n);
    }
    //m_box->setLayout(m_layout.get());
    //this->takeWidget();
    //this->setWidget(m_box.get());
}
