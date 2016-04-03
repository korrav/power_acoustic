#include "commandline.h"
#include "stdio.h"
#include <QTime>
CommandLine::CommandLine(QObject *parent) : QObject(parent), streamIn(stdin),
    streamOut(stdout, QIODevice::WriteOnly)
{
   pSocNotif =  new QSocketNotifier(0, QSocketNotifier::Read, this);
   connect(pSocNotif, SIGNAL(activated(int)), this, SLOT(readCommand()));
   pSocNotif->setEnabled(true);
}

void CommandLine::readCommand()
{
    QString strCommand = streamIn.readLine();
    emit receivCommand(strCommand);
}

void CommandLine::printOnScreen(QString str)
{
    streamOut << QTime::currentTime().toString("hh::mm::ss") << "\n";
    streamOut << str << "\n\n" <<endl;
}
