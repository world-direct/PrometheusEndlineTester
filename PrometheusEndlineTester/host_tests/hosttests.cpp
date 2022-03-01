#include "hosttests.h"

#include <QSettings>
#include "PrometheusEndlineTester_global.h"

namespace worlddirect {

HostTests::HostTests(QObject *parent)
    : QObject(parent),
      m_host_tests()
{

}

void HostTests::killall()
{
    // lock thead safty?
    for(auto it : m_host_tests){
        it->kill();
        delete it;
    }
    m_host_tests.clear();
    // unlock thead safty?
}

void HostTests::newTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
{
        Q_UNUSED(hostTestRun);
        Q_UNUSED(hostTest);
        QSettings settings(SETT_FILE_NAME, QSettings::IniFormat);

        auto cmd_key = QString("host_test_%1/cmd").arg(hostTestCase);
        auto args_key = QString("host_test_%1/args").arg(hostTestCase);
        auto pwd_key = QString("host_test_%1/pwd").arg(hostTestCase);

        auto cmd  = settings.value(cmd_key, "").toString();
        auto args = settings.value(args_key, "").toString();
        auto pwd = settings.value(pwd_key, "").toString();

        if(cmd.isEmpty()){
            return;
        }

        // lock thead safty?
        auto it = m_host_tests.find(hostTestCase);
        if(it == m_host_tests.end()){
            auto proc = new QProcess();
            proc->setProgram(cmd);

            if(pwd.isEmpty() == false){
               proc->setWorkingDirectory(pwd);
            }
            if(args.isEmpty() == false){
//                proc->setArguments(args);
            }
            it = m_host_tests.insert(hostTestCase, proc);
        }

        if(it.value()->state() == QProcess::Running){
            return;
        }

        it.value()->start();

}

} // namespace worlddirect
