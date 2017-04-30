#ifndef ORDERLAUNCHER_H
#define ORDERLAUNCHER_H

#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>

class OrderLauncher : public QGroupBox
{
    Q_OBJECT

public:
    OrderLauncher(QWidget *parent = nullptr);
    virtual ~OrderLauncher();

private:
    QPushButton *m_browse;
    QPushButton *m_reset;
    QGroupBox *m_radios;
    QRadioButton *m_ip;
    QRadioButton *m_mail;
    QRadioButton *m_phone;
    QPushButton *m_search;
    QHBoxLayout *m_layout;
    QVBoxLayout *m_buttonLayout;
    QVBoxLayout *m_filesLayout;
    QHBoxLayout *m_filesButtonLayout;
    QListWidget *m_list;

    QStringList m_files;

signals:
    void launchOrder(QString);

public slots:
    void launch();
    void browseFile();
    void resetFile();
};

#endif // ORDERLAUNCHER_H
