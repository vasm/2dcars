//@vsabadazh

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <QDialog>

#include <QtGui/QtGui>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = 0);
    ~ConnectDialog();

    QString address();
    uint    port();

private:
    QLabel*      _addressLabel;
    QLineEdit*   _addressInput;
    QLabel*      _portLabel;
    QSpinBox*    _portInput;
    QPushButton* _okBtn;
    QPushButton* _cancelBtn;
    QGridLayout* _layout;

signals:

public slots:
    
};

#endif // CONNECTDIALOG_H
