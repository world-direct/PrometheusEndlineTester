#ifndef WORLDDIRECT_TESTCASESUMARYMODEL_H
#define WORLDDIRECT_TESTCASESUMARYMODEL_H

#include <QStandardItemModel>

namespace worlddirect {
  class HostTest;
  class HostTestRun;
  class TestCase;

  class TestCaseSumaryModel : public QStandardItemModel
  {
    Q_OBJECT
  public:
    explicit TestCaseSumaryModel(QObject *parent = Q_NULLPTR);
    TestCaseSumaryModel(int rows, int columns, QObject *parent = Q_NULLPTR);

    TestCaseSumaryModel&  operator= ( const  TestCaseSumaryModel& ) = delete;
    TestCaseSumaryModel&  operator= ( const  TestCaseSumaryModel&& ) = delete;

    TestCaseSumaryModel ( const  TestCaseSumaryModel& ) = delete;
    TestCaseSumaryModel ( const  TestCaseSumaryModel&& ) = delete;

    virtual ~TestCaseSumaryModel() = default;

  public slots:
    void newHostTestRun(const QString& hostTestRun);
    void newHostTest(const QString& hostTestRun, const QString& hostTest);
    void newTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void startTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void failedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedHostTest(const QString& hostTestRun, const QString& hostTest);
    void failedHostTest(const QString& hostTestRun, const QString& hostTest);
    void passedHostTestRun(const QString& hostTestRun);
    void failedHostTestRun(const QString& hostTestRun);

  private:
    HostTestRun* findOrCreateHtRun(const QString& hostTestRunName);
    HostTest* findOrCreateHostTest(const QString& hostTestRunName, const QString& hostTestName);
    TestCase* findOrCreateTestCase(const QString& hostTestRunName, const QString& hostTestName, const QString& testCaseName);


  };

} // namespace worlddirect

#endif // WORLDDIRECT_TESTCASESUMARYMODEL_H
