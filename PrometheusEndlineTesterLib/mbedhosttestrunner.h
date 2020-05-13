#ifndef MBEDHOSTTESTRUNNER_H
#define MBEDHOSTTESTRUNNER_H

#include<QObject>
#include<QString>
#include<QByteArray>

#include "PrometheusEndlineTesterLib_global.h"

QT_BEGIN_NAMESPACE
class QSerialPort;
QT_END_NAMESPACE

namespace worlddirect {

    class PROMETHEUSENDLINETESTERLIB_EXPORT MbedHostTestRunner : public QObject
    {
    Q_OBJECT
    public:
        explicit MbedHostTestRunner(QObject *parent = nullptr);
        MbedHostTestRunner&  operator= ( const  MbedHostTestRunner& ) = delete;
        MbedHostTestRunner&  operator= ( const  MbedHostTestRunner&& ) = delete;

        MbedHostTestRunner ( const  MbedHostTestRunner& ) = delete;
        MbedHostTestRunner ( const  MbedHostTestRunner&& ) = delete;

        virtual ~MbedHostTestRunner();

        bool isConnected()const;

        int connect(QString port, int baudRate, int dataBits, int parity, int stopBits, int flowControl);
        int disconnect();
        qint64 sendKv(const QByteArray &key, const QByteArray &val);
        qint64 sendString(QByteArray str);
        qint64 sendSync(const QUuid &uuid);
        qint64 sendSync(const QByteArray &uuid);

    private:
        qint64 writePreamble();
        qint64 writeString(const QByteArray &str);
        qint64 writeSeperator();
        qint64 writePostamble();

    private:
        QSerialPort *m_serial;

    };

}

#endif // MBEDHOSTTESTRUNNER_H
