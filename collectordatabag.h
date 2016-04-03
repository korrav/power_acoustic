#ifndef COLLECTORDATABAG_H
#define COLLECTORDATABAG_H
#include <QObject>
#include "udpchange.h"
#include "commandline.h"
#include "mad.h"
#include "bag.h"

#define NUM_MADS_IN_BAG 3
class CollectorDataBag : public QObject
{
    Q_OBJECT
    UdpChange* pUdp_;
    CommandLine* pComLine_;
    Mad* mads[NUM_MADS_IN_BAG];
    Bag* bag;
public:
    explicit CollectorDataBag(UdpChange* pUdp, CommandLine* pComLine, QObject *parent = 0);

signals:

public slots:
    void parsUserCommand(QString strCom);
    void parsAnswerBag(QByteArray ans);
    void parsDataPower(QByteArray buf);
    void parsInfo(QByteArray buf);
private:
    void execComMads(const QStringList& listCom);  //выполнить команды, введённые пользователем и предназначенные для одного из МАД
    void execComBag(const QStringList& listCom);  //выполнить команды, введённые пользователем и предназначенные для БЭГ
    void execGeneralCom(const QStringList& listCom);    //выполнить команды, введённые пользователем и предназначенные для приложения
    void analyseStatus(const Status& st);   //разбор информации статуса приложения БЭГ
    void analyseConfig(const Configure& conf);
};

#endif // COLLECTORDATABAG_H
