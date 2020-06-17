#include "testexplorer.h"

#include <QBoxLayout>
#include <QLabel>
#include <QTreeView>

#include "nameplatewidget.h"

#include <ui/model/testcasesumarymodel.h>

namespace worlddirect {

  TestExplorer::TestExplorer(QWidget *parent) : QWidget(parent),
    m_testSetup(new QWidget()),
    m_namePlate(new NamePlateWidget()),
    m_testCaseSumary(new QLabel()),
    m_testCases(new QTreeView())
  {
    auto m_layout = new QGridLayout(this);
    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);
    this->setMaximumSize(560,this->maximumHeight());

    m_testSetup->setAutoFillBackground(true);
    auto testSetupLayout = new QGridLayout();
    testSetupLayout->setMargin(0);
    testSetupLayout->setHorizontalSpacing(0);
    testSetupLayout->setVerticalSpacing(0);
    testSetupLayout->addWidget(new QLabel(tr("UUID:")),0,0);
    testSetupLayout->addWidget(new QLabel(tr("Version:")),1,0);
    testSetupLayout->addWidget(new QLabel(tr("Timeout:")),2,0);
    m_testSetup->setLayout(testSetupLayout);
    m_layout->addWidget(m_testSetup);

    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    m_layout->addWidget(line);

    m_layout->addWidget(m_namePlate);

    auto line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    m_layout->addWidget(line2);

    m_testCaseSumary->setAutoFillBackground(true);
    m_testCaseSumary->setObjectName("testCaseSumary");
    m_layout->addWidget(m_testCaseSumary);

    m_testCases->setModel(new TestCaseSumaryModel());
    m_layout->addWidget(m_testCases);

    this->setLayout(m_layout);
  }

  void TestExplorer::newHostTestRun(const QString &hostTestRun)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model ->newHostTestRun(hostTestRun);
    m_testCases->expandAll();
  }

  void TestExplorer::newHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->newHostTest(hostTestRun,hostTest);
    m_testCases->expandAll();
  }

  void TestExplorer::newTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->newTestCase(hostTestRun, hostTest, hostTestCase);
    m_testCases->expandAll();
  }

  void TestExplorer::startTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    m_testCaseSumary->setStyleSheet("QLabel { background-color : yellow; color : yellow; }");
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->startTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void TestExplorer::passedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->passedTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void TestExplorer::failedTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->failedTestCase(hostTestRun, hostTest, hostTestCase);
  }

  void TestExplorer::passedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->passedHostTest(hostTestRun, hostTest);
  }

  void TestExplorer::failedHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->failedHostTest(hostTestRun, hostTest);
  }

  void TestExplorer::passedHostTestRun(const QString &hostTestRun)
  {
    m_testCaseSumary->setStyleSheet("QLabel { background-color : green; color : green; }");
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->passedHostTestRun(hostTestRun);
  }

  void TestExplorer::failedHostTestRun(const QString &hostTestRun)
  {
    m_testCaseSumary->setStyleSheet("QLabel { background-color : red; color : red; }");
    auto model = dynamic_cast<TestCaseSumaryModel*>(m_testCases->model());
    model->failedHostTestRun(hostTestRun);
  }

  void TestExplorer::typeReceived(const QString &type)
  {
    m_namePlate->setType(type);
  }

  void TestExplorer::hardwareVersionReceived(const QString &hwVer)
  {
    m_namePlate->setHardwareVersion(hwVer);
  }

  void TestExplorer::endpointNameReceived(const QString &epName)
  {
    m_namePlate->setEndpointName(epName);
  }

  void TestExplorer::iccIdReceived(const QString &iccId)
  {
    m_namePlate->setIccId(iccId);
  }

  void TestExplorer::printNameplate()
  {
   m_namePlate->printNameplate();
  }

} // namespace worlddirect
