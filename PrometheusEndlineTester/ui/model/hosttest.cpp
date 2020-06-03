#include "hosttest.h"

namespace worlddirect {

    HostTest::HostTest()
        :QStandardItem()
    {

    }

    HostTest::HostTest(const QString &text)
        :QStandardItem(QIcon(":/PrometheusEndlineTester/unicode/stopwatch.svg"), text)
    {

    }

    HostTest::HostTest(const QIcon &icon, const QString &text)
        :QStandardItem(icon, text)
    {

    }

    void HostTest::setStarted()
    {
        setIcon(QIcon(":/PrometheusEndlineTester/unicode/black_rightpointing_small_triangle.svg"));
    }

    void HostTest::setPassed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/check_mark.svg"));
    }

    void HostTest::setFailed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/ballot_x.svg"));
    }

    void HostTest::setTimedout()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/stopwatch.svg"));
    }

} // namespace worlddirect
