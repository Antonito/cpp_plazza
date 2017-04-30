#include <iostream>
#include <QFileDialog>
#include "orderlauncher.h"

OrderLauncher::OrderLauncher(QWidget *parent) :
    QGroupBox(tr("Create an order"), parent),
    m_browse(new QPushButton(tr("Add files"), this)),
    m_reset(new QPushButton(tr("Reset"), this)),
    m_radios(new QGroupBox(this)),
    m_ip(new QRadioButton(tr("IP adress"), this)),
    m_mail(new QRadioButton(tr("Mail address"), this)),
    m_phone(new QRadioButton(tr("Phone number"), this)),
    m_search(new QPushButton(tr("Search"), this)),
    m_layout(new QHBoxLayout(this)),
    m_buttonLayout(new QVBoxLayout(this)),
    m_filesLayout(new QVBoxLayout(this)),
    m_filesButtonLayout(new QHBoxLayout(this)),
    m_list(new QListWidget())
{
    m_buttonLayout->addWidget(m_ip);
    m_buttonLayout->addWidget(m_mail);
    m_buttonLayout->addWidget(m_phone);
    m_buttonLayout->addWidget(m_search);

    m_radios->setLayout(m_buttonLayout);

    m_filesButtonLayout->addWidget(m_browse);
    m_filesButtonLayout->addWidget(m_reset);

    m_filesLayout->addLayout(m_filesButtonLayout);
    m_filesLayout->addWidget(m_list);

    m_layout->addLayout(m_filesLayout);
    m_layout->addWidget(m_radios);
    this->setLayout(m_layout);

    m_radios->setMinimumWidth(400);

    m_ip->setChecked(true);

    QObject::connect(m_search, SIGNAL(clicked(bool)), this, SLOT(launch()));
    QObject::connect(m_browse, SIGNAL(clicked(bool)), this, SLOT(browseFile()));
    QObject::connect(m_reset, SIGNAL(clicked(bool)), this, SLOT(resetFile()));
}

OrderLauncher::~OrderLauncher()
{
}

void OrderLauncher::launch()
{
    QString mode;

    if (m_files.count() == 0)
    {
        return;
    }

    if (m_ip->isChecked())
    {
        mode = tr("IP_ADDRESS");
    }
    else if (m_mail->isChecked())
    {
        mode = tr("EMAIL_ADDRESS");
    }
    else
    {
        mode = tr("PHONE_NUMBER");
    }

    QString command;

    for (QString const &s : m_files)
    {
        command += s + ' ';
    }

    command += mode;

    this->launchOrder(command);
    m_files.clear();
    m_list->clear();
}

void OrderLauncher::browseFile()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Select Files"),".",tr("All Files (*)"));

    m_files += files;
    m_list->addItems(files);
}

void OrderLauncher::resetFile()
{
    m_files.clear();
    m_list->clear();
}
