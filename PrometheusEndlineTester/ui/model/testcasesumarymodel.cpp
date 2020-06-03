#include "hosttest.h"
#include "hosttestrun.h"
#include "testcase.h"
#include "testcasesumarymodel.h"
#include "hosttestcommon.h"

namespace worlddirect {

  TestCaseSumaryModel::TestCaseSumaryModel(QObject *parent)
    : QStandardItemModel(parent)
  {

  }

  TestCaseSumaryModel::TestCaseSumaryModel(int rows, int columns, QObject *parent)
    :QStandardItemModel(rows, columns, parent)
  {

  }

  void TestCaseSumaryModel::newHostTestRun(const QString &hostTestRun)
  {
    clear();
    findOrCreateHtRun(hostTestRun);
  }

  void TestCaseSumaryModel::newHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    findOrCreateHostTest(hostTestRun, hostTest);
  }

  void TestCaseSumaryModel::newTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    findOrCreateTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void TestCaseSumaryModel::startTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto testcase = findOrCreateTestCase(hostTestRun, hostTest, hostTestCase);
    testcase->setStarted();

    auto hosttest = dynamic_cast<HostTest*>(testcase->parent());
    hosttest->setStarted();

    auto htrun = dynamic_cast<HostTestRun*>(hosttest->parent());
    htrun->setStarted();
  }

  void TestCaseSumaryModel::passedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto testcase = findOrCreateTestCase(hostTestRun, hostTest, hostTestCase);
    testcase->setPassed();
  }

  void TestCaseSumaryModel::failedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto testcase = findOrCreateTestCase(hostTestRun, hostTest, hostTestCase);
    testcase->setFailed();
  }

  void TestCaseSumaryModel::passedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    auto hosttest = findOrCreateHostTest(hostTestRun, hostTest);
    hosttest->setPassed();
  }

  void TestCaseSumaryModel::failedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    auto hosttest = findOrCreateHostTest(hostTestRun, hostTest);
    hosttest->setFailed();
  }

  void TestCaseSumaryModel::passedHostTestRun(const QString &hostTestRun)
  {
    auto htrun = findOrCreateHtRun(hostTestRun);
    htrun->setPassed();
  }

  void TestCaseSumaryModel::failedHostTestRun(const QString &hostTestRun)
  {
    auto htrun = findOrCreateHtRun(hostTestRun);
    htrun->setFailed();

  }

  HostTestRun *TestCaseSumaryModel::findOrCreateHtRun(const QString &hostTestRunName)
  {
    auto items = findItems(hostTestRunName,Qt::MatchContains,0);

    HostTestRun * htrun = Q_NULLPTR;
    if(! items.isEmpty()){
        htrun = dynamic_cast<HostTestRun*>(items.first());
      }

    if (htrun == Q_NULLPTR){
        htrun = new HostTestRun(hostTestRunName);
        appendRow(htrun);
      }

    return htrun;
  }

  HostTest *TestCaseSumaryModel::findOrCreateHostTest(const QString &hostTestRunName, const QString &hostTestName)
  {
    auto htrun = findOrCreateHtRun(hostTestRunName);

    HostTest* hosttest = Q_NULLPTR;
    for(auto i = 0; i<htrun->rowCount(); i++){
        auto child = htrun->child(i);
        if(child->text() == hostTestName){
            hosttest = dynamic_cast<HostTest*>(child);
            break;
          }
      }

    if(hosttest == Q_NULLPTR){
        hosttest = new HostTest(hostTestName);
        htrun->appendRow(hosttest);
      }

    return hosttest;
  }

  TestCase *TestCaseSumaryModel::findOrCreateTestCase(const QString &hostTestRunName, const QString &hostTestName, const QString &testCaseName)
  {
    auto hosttest = findOrCreateHostTest(hostTestRunName, hostTestName);

    TestCase* testcase = Q_NULLPTR;
    for(auto i = 0; i<hosttest->rowCount(); i++){
        auto child = hosttest->child(i);
        if(child->text() == testCaseName){
            testcase = dynamic_cast<TestCase*>(child);
            break;
          }
      }

    if(testcase == Q_NULLPTR){
        testcase = new TestCase(testCaseName);
        hosttest->appendRow(testcase);
      }

    return testcase;

  }

} // namespace worlddirect
