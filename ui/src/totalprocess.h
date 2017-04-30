#ifndef TOTALPROCESS_H
#define TOTALPROCESS_H

#include <QGroupBox>
#include <QVBoxLayout>
#include <QLabel>
#include <vector>
#include "Response.hpp"

class TotalProcess : public QGroupBox
{
public:
    TotalProcess(QWidget *parent = nullptr);
    virtual ~TotalProcess();
    void updateData(std::vector<Response> const &resp);
private:
    QVBoxLayout *m_layout;
    QLabel *m_nbProcess;
    QLabel *m_busyProcess;
};

#endif // TOTALPROCESS_H
