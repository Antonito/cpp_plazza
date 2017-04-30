#ifndef PROCESSDISPLAY_H
#define PROCESSDISPLAY_H

#include <QGroupBox>
#include <QGridLayout>
#include <memory>
#include <vector>
#include <QScrollArea>
#include <QVBoxLayout>
#include "Response.hpp"
#include "wprocess.h"

class ProcessDisplay : public QGroupBox
{
public:
    ProcessDisplay(QWidget *parent = nullptr);
    virtual ~ProcessDisplay();

    void updateData(std::vector<Response> const &resp);
private:
    std::vector<std::unique_ptr<WProcess>> m_process;
    QGridLayout *m_layout;
    QScrollArea *m_area;
    QWidget *m_dummy;
    QVBoxLayout *m_blayout;
};

#endif // PROCESSDISPLAY_H
