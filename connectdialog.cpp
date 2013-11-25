//@vsabadazh

#include "connectdialog.h"
#include <QGridLayout>
#include "gameserver.h"

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowTitle("Connect to server");

    _addressLabel=new QLabel(QString::fromUtf8("Address"));
    _addressInput=new QLineEdit();
    _addressInput->setText("127.0.0.1");
    _portLabel=new QLabel("Port");
    _portInput=new QSpinBox();
    _portInput->setMaximum(80000);
    _portInput->setValue(GameServer::port);
    _okBtn = new QPushButton("Connect");
    connect(_okBtn, SIGNAL(clicked()), SLOT(accept()));
    _cancelBtn = new QPushButton("Cancel");
    connect(_cancelBtn, SIGNAL(clicked()), SLOT(reject()));

    _layout = new QGridLayout(this);
    _layout->addWidget(_addressLabel, 0, 0);
    _layout->addWidget(_addressInput, 0, 1);
    _layout->addWidget(_portLabel, 1, 0);
    _layout->addWidget(_portInput, 1, 1);
    _layout->addWidget(_okBtn, 2, 1);
    _layout->addWidget(_cancelBtn, 2, 0);

    setFixedSize(300, 120);
}

ConnectDialog::~ConnectDialog()
{
}

QString ConnectDialog::address()
{
    return _addressInput->text();
}

uint ConnectDialog::port()
{
    return _portInput->value();
}
