#ifndef WORLDDIRECT_HOSTTESTS_H
#define WORLDDIRECT_HOSTTESTS_H

#include <QMap>
#include <QObject>
#include <QProcess>
#include <QString>

namespace worlddirect {

class HostTests : public QObject
{
    Q_OBJECT
public:
    explicit HostTests(QObject *parent = nullptr);
    HostTests&  operator= ( const  HostTests& ) = delete;
    HostTests&  operator= ( const  HostTests&& ) = delete;
    HostTests ( const  HostTests& ) = delete;
    HostTests ( const  HostTests&& ) = delete;

    virtual ~HostTests() = default;

public:
    void killall();

public slots:
    void newTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);

private:
    QMap<QString, QProcess*> m_host_tests;

};

} // namespace worlddirect

#endif // WORLDDIRECT_HOSTTESTS_H
