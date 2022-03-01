#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QMutex>

class ClientThread : public QThread
{
    Q_OBJECT

public:
    explicit ClientThread(QObject* parent = nullptr);
    ClientThread ( const  ClientThread& ) = delete;
    ClientThread ( const  ClientThread&& ) = delete;

    ClientThread&  operator= ( const  ClientThread& ) = delete;
    ClientThread&  operator= ( const  ClientThread&& ) = delete;

    virtual ~ClientThread() = default;

public slots:
    void startClient();

signals:
    void request(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

private:
    QString m_portName;
    int m_baudRate;
    int m_dataBits;
    int m_parity;
    int m_stopBits;
    int m_flowControl;
//    QString m_response;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    bool m_quit = false;
};

#endif // CLIENTTHREAD_H
