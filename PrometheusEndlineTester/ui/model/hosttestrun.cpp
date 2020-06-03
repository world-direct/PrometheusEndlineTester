#include "hosttestrun.h"
#include "hosttestcommon.h"

namespace worlddirect {

    HostTestRun::HostTestRun()
        :QStandardItem()
    {

    }

    HostTestRun::HostTestRun(const QString &text)
        :QStandardItem(QIcon(":/PrometheusEndlineTester/unicode/stopwatch.svg"), text)
    {

    }

    HostTestRun::HostTestRun(const QIcon &icon, const QString &text)
        :QStandardItem(icon, text)
    {

    }

    void HostTestRun::setStarted()
    {
        setIcon(QIcon(":/PrometheusEndlineTester/unicode/black_rightpointing_small_triangle.svg"));
    }

    void HostTestRun::setPassed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/check_mark.svg"));
    }

    void HostTestRun::setFailed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/ballot_x.svg"));
    }

    void HostTestRun::setTimedout()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/stopwatch.svg"));
    }

} // namespace worlddirect
