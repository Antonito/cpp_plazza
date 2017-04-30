#ifndef WPROCESS_H
#define WPROCESS_H

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <memory>
#include "Response.hpp"

class WProcess : public QGroupBox
{
public:
    explicit WProcess(std::size_t n);
    virtual ~WProcess();

    void updateId(std::size_t n);
    void updateData(Response const &r);

    static std::size_t threadPerProcess;
private:
    QLabel *m_busy;
    QLabel *m_waiting;
    QVBoxLayout *m_layout;
};

#endif // WPROCESS_H
