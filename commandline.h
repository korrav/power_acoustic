#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QObject>
#include <QSocketNotifier>
#include <QTextStream>

class CommandLine : public QObject
{
    Q_OBJECT

    QSocketNotifier* pSocNotif;
    QTextStream streamIn;
    QTextStream streamOut;
public:
    explicit CommandLine(QObject *parent = 0);

signals:
    void receivCommand(QString strCommand);
public slots:
    void printOnScreen(QString str);    //вывести сообщение на экран
private slots:
    void readCommand(void);
};

#endif // COMMANDLINE_H
