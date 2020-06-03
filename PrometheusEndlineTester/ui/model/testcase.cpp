#include "testcase.h"

namespace worlddirect {

    TestCase::TestCase()
        :QStandardItem()
    {

    }

    TestCase::TestCase(const QString &text)
        :QStandardItem(QIcon(":/PrometheusEndlineTester/unicode/white_hourglass.svg"), text)
    {

    }

    TestCase::TestCase(const QIcon &icon, const QString &text)
        :QStandardItem(icon, text)
    {

    }

    void TestCase::setStarted()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/black_rightpointing_small_triangle.svg"));
    }

    void TestCase::setPassed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/check_mark.svg"));
    }

    void TestCase::setFailed()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/ballot_x.svg"));
    }

    void TestCase::setTimedout()
    {
      setIcon(QIcon(":/PrometheusEndlineTester/unicode/stopwatch.svg"));
    }

} // namespace worlddirect
