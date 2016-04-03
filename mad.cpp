#include "mad.h"
#include "command_for_bag.h"
Mad::Mad(quint16 num, QObject *parent): QObject(parent), num_(num)
{

}

Mad::~Mad()
{

}

void Mad::execPowerOn()
{
    QByteArray blob;
    quint32 idCom= POWERON;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&num_, sizeof(num_));
    emit command(blob);
}

void Mad::ackPowerOn()
{
    isOn_ = true;
}

void Mad::execPowerOff()
{
    QByteArray blob;
    quint32 idCom= POWEROFF;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&num_, sizeof(num_));
    emit command(blob);
}

void Mad::ackPowerOff()
{
    isOn_ = false;
}

void Mad::ackSetMaxValueCurrent(quint16 val)
{
    max_ = val;
}

void Mad::ackSetTimeOverh(quint16 num)
{
    time_overh_ = num;
}

void Mad::ackSetSeqNumPowerOn(qint32 seq)
{
    seqNum_ = seq;
}

void Mad::ackSetTimeAfterPrevActivate(quint32 time)
{
    timeAfterPrevActivate_ = time;
}

void Mad::execSetSecConfigureCurrentMad(quint16 maxCurrent, quint16 timeOverh)
{
    QByteArray blob;
    quint32 idCom= SET_SEC_CONFIG_CURRENT_MAD;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&num_, sizeof(num_)).
            append((const char *)&maxCurrent, sizeof(maxCurrent)).append((const char *)&timeOverh, sizeof(timeOverh));
    emit command(blob);
}
