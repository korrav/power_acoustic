#include <QCoreApplication>
#include <QCommandLineParser>
#include "collectordatabag.h"
#define PORT_SC_DEFAULT 32000
#define IP_BAG_DEFAULT  "192.168.203.36"
#define PORT_BAG_DEFAULT 32000
#define PERIOD_UPD_STATUS_INFO 600
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream streamOut(stdout, QIODevice::WriteOnly);
    streamOut << "Privet" << endl;
    //qint16 portSc = PORT_SC_DEFAULT;  //порт,через который работает программа
    //QString ipBag = IP_BAG_DEFAULT;  //адрес БЭГ
    //quint16 portBag = PORT_BAG_DEFAULT;    //порт БЭГ

    QCommandLineParser parser;
    //Установка опций
    parser.setApplicationDescription("Данная программа предназаначенна для управления блоком электроники акустической гирлянды.\n");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption optBagIp(QStringList() << "b" << "bag_ip","ip адрес БЭГ", "bag ip", IP_BAG_DEFAULT);
    parser.addOption(optBagIp);
    QCommandLineOption optBagPort(QStringList() << "p" << "bag_port","порт БЭГ", "bag port", QString().setNum(PORT_BAG_DEFAULT));
    parser.addOption(optBagPort);
    QCommandLineOption optScPort(QStringList() << "s" << "sc_port","порт программы оператора",
                                 "sc port", QString().setNum(PORT_SC_DEFAULT));
    parser.addOption(optScPort);
    QCommandLineOption optPeriodUpdStatusInfoOnScreen(QStringList() << "t" << "time",  "период обновления статусной информации на экране",
                                                      "period", QString().setNum(PERIOD_UPD_STATUS_INFO));
    parser.addOption(optPeriodUpdStatusInfoOnScreen);
    parser.process(a);
    //Проверка введённых пользователем опций
    QString ipBag  = parser.value(optBagIp);  //адрес БЭГ
    quint16 portBag  = parser.value(optBagPort).toUShort();  //порт БЭГ
    quint16 portSc  = parser.value(optScPort).toUShort();  //порт,через который работает программа

    UdpChange udpCh(portSc, ipBag, portBag);
    CommandLine comLine;
    CollectorDataBag collector(&udpCh, &comLine);
    return a.exec();
}
