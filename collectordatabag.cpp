#include "collectordatabag.h"
#include "command_for_bag.h"
#include <QCoreApplication>
#include <limits>

CollectorDataBag::CollectorDataBag(UdpChange *pUdp, CommandLine *pComLine, QObject *parent) : QObject(parent),
    pUdp_(pUdp), pComLine_(pComLine)
{
    connect(pComLine, SIGNAL(receivCommand(QString)), this, SLOT(parsUserCommand(QString)));
    connect(pUdp, SIGNAL(rec_data_ans(QByteArray)), this, SLOT(parsAnswerBag(QByteArray)));
    bag = new Bag(this);
    for(int i = 0; i < NUM_MADS_IN_BAG; i++) {
        mads[i] = new Mad(i +1, this);
        connect(mads[i], SIGNAL(command(QByteArray)), pUdp_, SLOT(send_data(QByteArray)));
        connect(mads[i], SIGNAL(print(QString)), pComLine_, SLOT(printOnScreen(QString)));
    }
    connect(pUdp, SIGNAL(rec_data_power(QByteArray)), this, SLOT(parsDataPower(QByteArray)));
    connect(pUdp, SIGNAL(rec_data_info(QByteArray)), this, SLOT(parsInfo(QByteArray)));
    connect(bag, SIGNAL(command(QByteArray)), pUdp_, SLOT(send_data(QByteArray)));
    connect(bag, SIGNAL(print(QString)), pComLine_, SLOT(printOnScreen(QString)));
}

void CollectorDataBag::parsUserCommand(QString strCom)
{
    QStringList listCom = strCom.split(" ", QString::SkipEmptyParts);
    if(listCom.size() == 0)
        return;
    QString strDest = listCom.at(0);
    if(strDest.startsWith("mad"))
        execComMads(listCom);
    else if(strDest == "bag")
        execComBag(listCom);
    else
        execGeneralCom(listCom);
}

void CollectorDataBag::parsAnswerBag(QByteArray ans)
{
    const H_pack_ans* pHeadAns = (const  H_pack_ans*)ans.constData();
    switch(pHeadAns->id) {
    case CONFIGURE:
        if(pHeadAns->status == OK)
            pComLine_->printOnScreen("Команда выполнена успешно");
        else
            pComLine_->printOnScreen("Попытка выполнить команду окончилась неудачей");
        if(ans.size() >= static_cast<int>(sizeof(H_pack_ans) + sizeof(Configure))) {
            Configure* pConf = (Configure*)(ans.constData() + sizeof(H_pack_ans));
            analyseConfig(*pConf);
        }
        break;
    case STATUS:
        if(pHeadAns->status == OK)
            pComLine_->printOnScreen("Команда выполнена успешно");
        else
            pComLine_->printOnScreen("Попытка выполнить команду окончилась неудачей");
        if(ans.size() >= static_cast<int>(sizeof(H_pack_ans) + sizeof(Status))) {
            Status* pStatus = (Status*)(ans.constData() + sizeof(H_pack_ans));
            analyseStatus(*pStatus);
        }
        break;
    }
}

void CollectorDataBag::parsDataPower(QByteArray buf)
{
    const Sample* pS = (const  Sample*)buf.constData();
    QString str("Текущее состояние системы питания акустического стринга\n300V: %1В, 48V: %2В, I1: %3А, I2: %4А, I3: %5А");
    pComLine_->printOnScreen(
                str.arg(pS->u300v).arg(double(pS->u48v)/1000).arg(double(pS->iMad[0])/1000).arg(double(pS->iMad[1])/1000).arg(double(pS->iMad[2])/1000));
}

void CollectorDataBag::parsInfo(QByteArray buf)
{
    const qint32* info = (const qint32*)buf.constData();
    switch(*info) {
    case INF_VOLT_48V_TO_SEC_MODE:
        pComLine_->printOnScreen("INFO: напряжение на выводе 48В перешло в безопасный режим");
        break;
    case INF_VOLT_48V_TO_UNSEC_MODE:
        pComLine_->printOnScreen("INFO: напряжение на выводе 48В перешло в небезопасный режим");
        break;
    case INF_VOLT_300V_TO_SEC_MODE:
        pComLine_->printOnScreen("INFO: напряжение на выводе 300В перешло в безопасный режим");
        break;
    case INF_VOLT_300V_TO_UNSEC_MODE:
        pComLine_->printOnScreen("INFO: напряжение на выводе 300В перешло в небезопасный режим");
        break;
    case INF_MONIT_F3_NOT_TIME:
        pComLine_->printOnScreen("INFO: мониторинговая программа питания в F3 не успевает обрабатывать поток данных");
        break;
    case INF_MONIT_F4_NOT_TIME:
        pComLine_->printOnScreen("INFO: мониторинговая программа питания в F4 не успевает обрабатывать поток данных");
        break;
    case INF_OVR_ADC:
        pComLine_->printOnScreen("INFO: DMA в F3 не успевает забирать данные из входного буфера АЦП");
        break;
    case INF_SPI_TRANS_NOT_TIME:
        pComLine_->printOnScreen("INFO: передача данных  в SPI порт в F3 не успевает вовремя закончится при текущей скорости потока");
        break;
    case INF_CUR_TO_OVERHEAD:
    {
        const qint32* pNumMad = info + 1;
        pComLine_->printOnScreen(QString("У МАД %1 наблюдается перегрузка тока потребления").arg(*pNumMad));
        break;
    }
    case INF_CUR_TO_NORMAL:
    {
        const qint32* pNumMad = info + 1;
        pComLine_->printOnScreen(QString("У МАД %1 наблюдается тока потребления в нормальном состоянии").arg(*pNumMad));
        break;
    }
    case INF_SET_TEST_PIN_PA3:
        pComLine_->printOnScreen("INFO: установлен тестовый вывод PA3 в F4");
        break;
    case INF_SET_TEST_PIN_EX13:
        pComLine_->printOnScreen("INFO: установлен тестовый вывод 13 в разъёме расширения в F4");
        break;
    case INF_SET_TEST_PIN_EX15:
        pComLine_->printOnScreen("INFO: установлен тестовый вывод 15 в разъёме расширения в F4");
        break;
    case INF_SET_TEST_PIN_EX16:
        pComLine_->printOnScreen("INFO: установлен тестовый вывод 16 в разъёме расширения в F4");
        break;
    }
}

void CollectorDataBag::execComMads(const QStringList& listCom)
{
    QString strDest = listCom.at(0);
    bool isNum =false;
    quint16 num = strDest.mid(3).toUShort(&isNum);
    if(isNum && num > 0 && num <= NUM_MADS_IN_BAG) {
        /*команды для Мад*/
        if(listCom.size() < 2)
            return;
        QString command = listCom.at(1);
        if(command == PWON) //включение МАД
            mads[num - 1]->execPowerOn();
        else if(command == PWOFF)   //выключение МАД
            mads[num - 1]->execPowerOff();
        else if(command == SET_SECUR_CONFIG_CURRENT_MAD) {
            bool statusCur, statusTime;
            if(listCom.size() < 4)
                return;
            quint16 maxCur = listCom.at(2).toUShort(&statusCur);
            quint32 fact;
            QString strTimeOverh = listCom.at(3);
            if(listCom.at(3).endsWith('m')) {
                fact = 1000;
                strTimeOverh.chop(1);
            } else if(strTimeOverh.endsWith('u')) {
                fact = 1;
                strTimeOverh.chop(1);
            }  else
                fact = 1;
            quint16 timeOverh = listCom.at(3).toUShort(&statusTime);
            if(!statusCur || !statusTime) {
                pComLine_->printOnScreen(QString("Неверно заданный аргумент у команды %1").arg(command));
                return;
            }
            if(fact * timeOverh > std::numeric_limits<quint16>::max()) {
                pComLine_->printOnScreen(QString("Время нахождения тока в небезопасном режиме до сигнала тревоги не должно превышать 65 мс"));
            }
            mads[num]->execSetSecConfigureCurrentMad(maxCur, timeOverh * fact);
        } else
            pComLine_->printOnScreen(QString("Неверно заданная команда: %1").arg(command));
    } else
        pComLine_->printOnScreen(QString("Неверно заданный адресат команды: %1").arg(strDest));
}

void CollectorDataBag::execComBag(const QStringList &listCom)
{
    if(listCom.size() < 2)
        return;
    QString command = listCom.at(1);
    if(command == SET_PERIOD_MONIT_POWER) { //установить период передачи мониторной информации по питанию
        if(listCom.size() != 3) {
            pComLine_->printOnScreen(QString("Неверно заданный аргумент у команды %1").arg(command));
            return;
        }
        QString arg = listCom.at(2);
        qint64 fact;
        if(arg.endsWith('m'))
            fact = 1000;
        else if(arg.endsWith('s'))
            fact = 1e6;
        else if(arg.endsWith('M'))
            fact = 60e6;
        else if(arg.endsWith('h'))
            fact = 3600e6;
        else if(arg.endsWith('u')) {
            fact = 1;
            arg.chop(1);
        }
        else
            fact = 1;
        if(fact != 1)
            arg.chop(1);
        bool isNum = false;
        qint64 num = arg.toInt(&isNum);
        if(!isNum) {
            pComLine_->printOnScreen(QString("Неверно заданный аргумент у команды %1").arg(command));
            return;
        }
        num *= fact;
        bag->execSetPeriodTransDataMonitPower(num);
    } else if(command == DISABLE_TRANS_MONIT_POWER) //запретить передачу мониторной информации по питанию
        bag->execDisableTransDataMonitPower();
    else if(command == GET_SAMPLE_MONIT_POWER)
        bag->execGetSampleDataMonitPower();
    else if(command == START_MONIT_POWER)
        bag->execStartMonitDataPower();
    else if(command == SET_SECUR_CONFIG_48V) {
        bool statusMax, statusMin, statusTime;
        if(listCom.size() < 5)
            return;
        quint16 max = listCom.at(2).toUShort(&statusMax);
        quint16 min = listCom.at(3).toUShort(&statusMin);
        quint32 fact;
        QString strTimeOverh = listCom.at(4);
        if(listCom.at(4).endsWith('m')) {
            fact = 1000;
            strTimeOverh.chop(1);
        } else if(strTimeOverh.endsWith('u')) {
            fact = 1;
            strTimeOverh.chop(1);
        }  else
            fact = 1;
        quint16 timeOverh = listCom.at(4).toUShort(&statusTime);
        if(!statusMax || !statusMin || !statusTime) {
            pComLine_->printOnScreen(QString("Неверно заданный аргумент у команды %1").arg(command));
            return;
        }
        if(fact * timeOverh > std::numeric_limits<quint16>::max()) {
            pComLine_->printOnScreen(QString("Время нахождения напряжения на выводе 48В в небезопасном режиме до сигнала тревоги не должно превышать 65 мс"));
        }
        bag->execSetSecConfigure48v(max, min, timeOverh * fact);
    }
    else if(command == SET_SECUR_CONFIG_300V){
        bool statusMax, statusMin, statusTime;
        if(listCom.size() < 5)
            return;
        quint16 max = listCom.at(2).toUShort(&statusMax);
        quint16 min = listCom.at(3).toUShort(&statusMin);
        quint32 fact;
        QString strTimeOverh = listCom.at(4);
        if(listCom.at(4).endsWith('m')) {
            fact = 1000;
            strTimeOverh.chop(1);
        } else if(strTimeOverh.endsWith('u')) {
            fact = 1;
            strTimeOverh.chop(1);
        }  else
            fact = 1;
        quint16 timeOverh = listCom.at(4).toUShort(&statusTime);
        if(!statusMax || !statusMin || !statusTime) {
            pComLine_->printOnScreen(QString("Неверно заданный аргумент у команды %1").arg(command));
            return;
        }
        if(fact * timeOverh > std::numeric_limits<quint16>::max()) {
            pComLine_->printOnScreen(QString("Время нахождения напряжения на выводе 300В в небезопасном режиме до сигнала тревоги не должно превышать 65 мс"));
        }
        bag->execSetSecConfigure300v(max, min, timeOverh * fact);
    }
    else if(command == SAVE_CONF_IN_EEPROM)
        bag->execSaveConfigureInEEPROM();
}

void CollectorDataBag::execGeneralCom(const QStringList &listCom)
{
    if(listCom.size() == 0)
        return;
    QString com = listCom.at(0);
    if(com == "exit") {
        QCoreApplication::quit();
    } else {
        pComLine_->printOnScreen("Неверно заданная команда");
    }
}

void CollectorDataBag::analyseStatus(const Status &st)
{
    for(int i = 0; i < 3; i++) {
        if(st.stPower.isMadOn[i] == ACTIVE)
            mads[i]->ackPowerOn();
        else
            mads[i]->ackPowerOff();
    }
    bag->ack48VMoveToActive(st.stPower.is48vActive);
    bag->ack300VMoveToActive(st.stPower.is300vActive);
    analyseConfig(st.conf);
}

void CollectorDataBag::analyseConfig(const Configure &conf)
{
    for(int i = 0; i < 3; i++) {
        mads[i]->ackSetMaxValueCurrent(conf.sec.mads[i].max);
        mads[i]->ackSetTimeOverh(conf.sec.mads[i].time_overh);
        mads[i]->ackSetSeqNumPowerOn(conf.order[i].seq);
        mads[i]->ackSetTimeAfterPrevActivate(conf.order[i].timeAfterPrevActivate);
    }
    bag->ackSetMaxValueVolt48V(conf.sec.v_48.max);
    bag->ackSetMinValueVolt48V(conf.sec.v_48.min);
    bag->ackSetTimeOverh48V(conf.sec.v_48.time_overh);
    bag->ackSetMaxValueVolt300V(conf.sec.v_300.max);
    bag->ackSetMinValueVolt300V(conf.sec.v_300.min);
    bag->ackSetTimeOverh300V(conf.sec.v_300.time_overh);
    bag->ackEnableAutoOffPowerMad(conf.isEnableAutoOffPowerMad);
    bag->ackEnableMonitorOrientation(conf.isEnableMonitorOrientation);
    bag->ackSetPeriodTransDataMonitPower(conf.seqNumSample);
}

