#ifndef UDPCHANGE_H
#define UDPCHANGE_H

#include <QObject>
#include <QUdpSocket>

struct SrcPack {
    qint32 idSrc;  //идентификатор источника
    qint32 idBlock;    //идентификатор блока данных
};

#define ID_SC 4     //идентификатор компьютера берегового центра
#define ID_BAG 5    //идентификатор БЭГ
#define IP_BAG "192.168.203.36" //ip БЭГ
#define PORT_BAG    32000 //порт БЭГ
#define PORT_CS    32000 //порт БЭГ
enum id_block {	//идентификаторы блоков данных
    DATA,
    COMMAND,
    ANSWER,
    INFO
};

struct MarkPack {
    qint32 id; //уникальный номер блока данных
    qint32 part;   //порядковый номер куска блока данных
    qint32 totalPart;  //общее количество частей в пакете
};

struct HeadPack {   //заголовок пакета
    SrcPack endpoints;
    MarkPack mark;
};

struct H_pack_ans { //структура, содержащая ответ на команду
    qint32 id; //идентификатор команды
    qint32 status; //результат выполнения команды (OK или NOT_OK)
};

enum status_ans {
    NOT_OK, OK
};

enum id_data {  //идентификаторы блоков данных
    POWER,  //данные по питанию
    POSITION,   //данные положения в пространстве
};

enum {
    DEACTIVE,
    ACTIVE
};

struct StatusOfPower {	//структура характеризующая текущее состояние системы питания акустического стринга
    quint32 isMadOn[3];
    quint32 is48vActive;
    quint32 is300vActive;
};

struct MCur {	//конфигурация безопасности тока МАД
    uint16_t max;	//верхняя граница безопасного значения тока потребления
    uint16_t time_overh;	//количество отсчётов, которое ток потребления может находится за границей max, прежде чем сработает сигнал тревоги
};

struct Volt {
    uint16_t max;	//верхняя граница безопасного значения напряжения питания
    uint16_t min;	//нижняя граница безопасного значения напряжения питания
    uint16_t time_overh;	//количество отсчётов, которое напряжение питания может находится за границей безопасного режима, прежде чем сработает сигнал тревоги
};

struct SecConfigure {	//конфигурация безопасности
    struct MCur mads[3];	//токи потребления МАД
    struct Volt v_48;	//напряжение 48В
    struct Volt v_300; //напряжение 300В
};

struct OrderMadActivate {	//порядок активации МАД
    qint32 seq;	//порядковый номер включения
    quint32 timeAfterPrevActivate;	//время в мс, которое пройдёт после предыдущего включения
};

struct Configure {	//конфигурация приложения
    struct SecConfigure sec;
    struct OrderMadActivate order[3];	//очерёдность и время ключения МАД
    quint32 isEnableAutoOffPowerMad;	//разрешение автовыключения канал МАД при сигнализировании о его перегрузке
    quint32 isEnableMonitorOrientation;	//разрешение вести мониторинг ориентации в пространстве
    qint32 seqNumSample;	//порядковый номер отсчёта, подлежащего записи в ethernet буфер
};

struct Status {	//текущее состояние приложения БЭГ
    struct StatusOfPower stPower;
    struct Configure conf;
};


struct Sample {			//структура, содержащая параметры одного отсчёта
    quint16 iMad[3];
    quint16 u48v;
    quint16 u300v;
};

/*СТРУКТУРЫ ПАКЕТОВ ДАННЫХ*/

//DATA
/*
 * 1)struct HeadPack
 * 2)id_data
 * 3)payload
 */

//INFO

/*
 * 1)struct HeadPack
 * 2)payload
 */

//ANSWER
/*
 * 1)struct HeadPack
 * 2)struct h_pack_ans
 * 3)payload
 */

//COMMAND
/*
 * 1)struct HeadPack
 * 2)int id_command
 * 3)arg1
 * ****************
 * 4)argN
 */

class UdpChange : public QObject
{
    Q_OBJECT
    QUdpSocket* pUdp;
    QHostAddress bagIp;
    qint16 bagPort;

    void processTheDatagram(QByteArray& datagram);  //обработка принятых датаграм
public:
    explicit UdpChange(quint16 port_src = PORT_CS, QString ip_dest = QString(IP_BAG), quint16 port_dst = PORT_BAG, QObject *parent = 0);

signals:
    void rec_data_power(QByteArray Blob); //приняты данные питания МАД
    void rec_data_pos(QByteArray Blob);   //приняты данные о положении в пространстве стринга
    void rec_data_ans(QByteArray Blob);   //приняты данные, содержащие ответ на команду
    void rec_data_info(QByteArray Blob);   //приняты данные, содержащие важную информацию
private slots:
    void readPendingDatagrams(void);
public slots:
    void send_data(QByteArray pBlob);
};

#endif // UDPCHANGE_H
