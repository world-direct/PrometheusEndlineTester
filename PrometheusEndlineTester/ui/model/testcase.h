#ifndef WORLDDIRECT_TESTCASE_H
#define WORLDDIRECT_TESTCASE_H

#include <QStandardItem>

namespace worlddirect {

  class TestCase : public QStandardItem
  {
  public:
      TestCase();
      explicit TestCase(const QString &text);
      TestCase(const QIcon &icon, const QString &text);

      TestCase&  operator= ( const  TestCase& ) = delete;
      TestCase&  operator= ( const  TestCase&& ) = delete;

      TestCase ( const  TestCase& ) = delete;
      TestCase ( const  TestCase&& ) = delete;

      virtual ~TestCase() = default;

  public:
      void setStarted();
      void setPassed();
      void setFailed();
      void setTimedout();

  };

} // namespace worlddirect

#endif // WORLDDIRECT_TESTCASE_H
