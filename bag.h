#ifndef BAG_H
#define BAG_H

#include <QObject>

enum {
    DISABLE_TRANS_DATA = -1,	//запрещено передавать данные оператору
    SINGLE_TRANS_DATA = 0,	//произвести единичную передачу данных
};

class Bag: public QObject
{
    Q_OBJECT
    quint32 is48vActive_;    //напряжение 48В находится в рамках нормальных значений
    quint32 is300vActive_;   //напряжение 300В находится в рамках нормальных значений
    quint16 maxValue48V_;   //верхний предел безопасного режима вывода 48В
    quint16 minValue48V_;   //нижний предел безопасного режима 48В
    quint16 timeOverh48V_;    //время нахождения вывода 48В в небезопасном режиме(кол. отсчётов) до того, как сработает сигнал тревоги
    quint16 maxValue300V_;   //верхний предел безопасного режима вывода 300В
    quint16 minValue300V_;   //нижний предел безопасного режима 300В
    quint16 timeOverh300V_;    //время нахождения вывода 300В в небезопасном режиме(кол. отсчётов) до того, как сработает сигнал тревоги
    quint32 isEnableAutoOffPowerMad_;	//разрешение автовыключения канала МАД при сигнализировании о его перегрузке (DEACTIVE/ACTIVE)
    quint32 isEnableMonitorOrientation_;	//разрешение вести мониторинг ориентации в пространстве (DEACTIVE/ACTIVE)
    qint32 periodTransDataMonitPower_;  //период передачи мониторинговой информации по питанию (в 10 мкс)
public:
    Bag(QObject *parent = 0);
public slots:
    void execSetPeriodTransDataMonitPower(qint64 time); //установить период передачи мониторинговой информации по питанию (мкс)
    void execDisableTransDataMonitPower(void);  //запретить передачу мониторинговой инфорации по питанию
    void execGetSampleDataMonitPower(void);  //получить один отсчёт мониторинговой инфорации по питанию
    void ack48VMoveToActive(quint32 status);    //переход вывода 48В в активное состояние( или неактивное)
    void ack300VMoveToActive(quint32 status);    //переход вывода 300В в активное состояние( или неактивное)
    void ackSetMaxValueVolt48V(quint16 val);   //подтвержение установки нового значения верхней границы напряжения вывода 48В в небезопасном режиме
    void ackSetMinValueVolt48V(quint16 val);   //подтвержение установки нового значения нижней границы напряжения вывода 48В в небезопасном режиме
    void ackSetTimeOverh48V(quint16 val);   //подтвержение установки нового значения количества отсчётов нахождения вывода 48В в небезопасном режиме
    void ackSetMaxValueVolt300V(quint16 val);   //подтвержение установки нового значения верхней границы напряжения вывода 300В в небезопасном режиме
    void ackSetMinValueVolt300V(quint16 val);   //подтвержение установки нового значения нижней границы напряжения вывода 300В в небезопасном режиме
    void ackSetTimeOverh300V(quint16 val);   //подтвержение установки нового значения количества отсчётов нахождения вывода 300В в небезопасном режиме
    void ackEnableAutoOffPowerMad(quint32 is_enable);   //подтверждение изменения разрешения на автовыключения канала МАД при сигнализировании о его перегрузке (DEACTIVE/ACTIVE)
    void ackEnableMonitorOrientation(quint32 is_enable);    //подтверждение изменения разрешения на ведение мониторинга ориентации в пространстве (DEACTIVE/ACTIVE)
    void ackSetPeriodTransDataMonitPower(qint32 time); //подтверждение установки периода передачи мониторинговой информации по питанию (мкс)
    void execStartMonitDataPower(void); //запустить оцифровку мониторинговой информации питания
    void execSetSecConfigure48v(quint16 max, quint16 min, quint16 time_overh);  //установить конфигурацию безопасности напряжения на выводе 48В
    void execSetSecConfigure300v(quint16 max, quint16 min, quint16 time_overh);  //установить конфигурацию безопасности напряжения на выводе 300В
    void execSaveConfigureInEEPROM(void);   //активизировать процедуру сохранения конфигурационной информации в EEPROM
signals:
    void command(QByteArray com);
    void print(QString str);
};

#endif // BAG_H
