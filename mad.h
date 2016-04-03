#ifndef MAD_H
#define MAD_H
#include <QtGlobal>
#include <QObject>
#include <QString>

class Mad: public QObject
{
    Q_OBJECT
public:
    const qint32 DISABLE_MAD = -1;    //МАД изначально находится в выключенном состоянии
private:
    quint32 num_;   //номер
    bool isOn_ = false;  //включен/выключен
    qint32 seqNum_  =  DISABLE_MAD; //порядковый номер включения МАД
    quint32 timeAfterPrevActivate_ = 10;	//время в мс, которое пройдёт после предыдущего включения
    quint16 max_ = 65535;	//верхняя граница безопасного значения тока потребления
    quint16 time_overh_ = 10;	//количество отсчётов, которое ток потребления может находится за границей max, прежде чем сработает сигнал тревоги
public:
    Mad(quint16 num, QObject *parent = 0);
    virtual ~Mad();
public slots:
    void execPowerOn(void);     //выполнить включение
    void ackPowerOn(void);      //подтверждение включения
    void execPowerOff(void);    //выполнить выключение
    void ackPowerOff(void);     //подтвержение выключения
    void ackSetMaxValueCurrent(quint16 val);    //подтведить установку нового значения верхней границы тока потребления МАД
    void ackSetTimeOverh(quint16 num); //подтвердить установку нового значения количества отсчётов нахождения тока в небезопасном режиме
    void ackSetSeqNumPowerOn(qint32 seq);   //подтвердить установку нового значения порядка включения МАД
    void ackSetTimeAfterPrevActivate(quint32 time); //подтвердить установку нового значения времени в мс, которое пройдёт после предыдущего включения
    void execSetSecConfigureCurrentMad(quint16 maxCurrent, quint16 timeOverh);  //установить конфигурацию безопасности тока потребления МАД
signals:
    void command(QByteArray com);
    void print(QString str);
};

#endif // MAD_H
