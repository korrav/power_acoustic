#include "udpchange.h"
#include <iostream>

void UdpChange::processTheDatagram(QByteArray &datagram)
{
    int* pIdData;
    const HeadPack* pHead = (const HeadPack*)datagram.constData();
    switch(pHead->endpoints.idBlock) {
    case DATA:
        pIdData =(int*)(datagram.constData() + sizeof(HeadPack));
        if(*pIdData == POWER)
            emit rec_data_power(datagram.remove(0, sizeof(HeadPack) + sizeof(int)));
        else
            if(*pIdData == POSITION)
                emit rec_data_pos(datagram.remove(0, sizeof(HeadPack) + sizeof(int)));
        break;
    case ANSWER:
        emit rec_data_ans(datagram.remove(0, sizeof(HeadPack)));
        break;
    case INFO:
        emit rec_data_info(datagram.remove(0, sizeof(HeadPack)));
        break;
    }
}

UdpChange::UdpChange(quint16 port_src, QString ip_dest, quint16 port_dst, QObject *parent) :
    QObject(parent), bagIp(ip_dest), bagPort(port_dst)
{
    pUdp = new QUdpSocket(this);
    pUdp->bind(QHostAddress::Any, port_src);
    connect(pUdp, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void UdpChange::readPendingDatagrams()
{
    while (pUdp->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(pUdp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        pUdp->readDatagram(datagram.data(), datagram.size(),
                           &sender, &senderPort);
        if(/*sender == bagIp &&*/ senderPort == bagPort)
            processTheDatagram(datagram);
    }
}

void UdpChange::send_data(QByteArray pBlob)
{
    HeadPack head;
    head.endpoints.idSrc = ID_SC;
    head.endpoints.idBlock = COMMAND;
    pUdp->writeDatagram(pBlob.prepend((const char*)&head, sizeof(head)), bagIp, bagPort);
}

