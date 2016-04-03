#include "bag.h"
#include "command_for_bag.h"
Bag::Bag(QObject *parent): QObject(parent)
{

}

void Bag::execSetPeriodTransDataMonitPower(qint64 time)
{
    QByteArray blob;
    quint32 idCom= SET_PERIOD_TRANS_DATA_MONIT_POWER;
    if(time  > std::numeric_limits<qint32>::max()) {
        emit print("Период передачи мониторинговой информации по питанию слишком большой");
        return;
    }
    qint32 period = time;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&period, sizeof(period));
    emit command(blob);
}

void Bag::execDisableTransDataMonitPower()
{
    execSetPeriodTransDataMonitPower(DISABLE_TRANS_DATA);
}

void Bag::execGetSampleDataMonitPower()
{
    execSetPeriodTransDataMonitPower(SINGLE_TRANS_DATA);
}

void Bag::ack48VMoveToActive(quint32 status)
{
    is48vActive_ = status;
}

void Bag::ack300VMoveToActive(quint32 status)
{
    is300vActive_ = status;
}

void Bag::ackSetMaxValueVolt48V(quint16 val)
{
    maxValue48V_ = val;
}

void Bag::ackSetMinValueVolt48V(quint16 val)
{
    minValue48V_ = val;
}

void Bag::ackSetTimeOverh48V(quint16 val)
{
    timeOverh48V_ = val;
}

void Bag::ackSetMaxValueVolt300V(quint16 val)
{
    maxValue300V_ = val;
}

void Bag::ackSetMinValueVolt300V(quint16 val)
{
    minValue300V_ = val;
}

void Bag::ackSetTimeOverh300V(quint16 val)
{
    timeOverh300V_ = val;
}

void Bag::ackEnableAutoOffPowerMad(quint32 is_enable)
{
    isEnableAutoOffPowerMad_ = is_enable;
}

void Bag::ackEnableMonitorOrientation(quint32 is_enable)
{
    isEnableMonitorOrientation_ = is_enable;
}

void Bag::ackSetPeriodTransDataMonitPower(qint32 time)
{
    periodTransDataMonitPower_ = time;
}

void Bag::execStartMonitDataPower()
{
    QByteArray blob;
    quint32 idCom= START_MONIT_DATA_POW;
    blob.append((const char *)&idCom, sizeof(idCom));
    emit command(blob);
}

void Bag::execSetSecConfigure48v(quint16 max, quint16 min, quint16 time_overh)
{
    QByteArray blob;
    quint32 idCom= SET_SEC_CONFIG_48V;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&max, sizeof(max))
            .append((const char *)&min, sizeof(min)).append((const char *)&time_overh, sizeof(time_overh));
    emit command(blob);

}

void Bag::execSetSecConfigure300v(quint16 max, quint16 min, quint16 time_overh)
{
    QByteArray blob;
    quint32 idCom= SET_SEC_CONFIG_300V;
    blob.append((const char *)&idCom, sizeof(idCom)).append((const char *)&max, sizeof(max))
            .append((const char *)&min, sizeof(min)).append((const char *)&time_overh, sizeof(time_overh));
    emit command(blob);
}

void Bag::execSaveConfigureInEEPROM()
{
    QByteArray blob;
    quint32 idCom= SAVE_CONFIG_IN_EEPROM;
    blob.append((const char *)&idCom, sizeof(idCom));
    emit command(blob);
}

