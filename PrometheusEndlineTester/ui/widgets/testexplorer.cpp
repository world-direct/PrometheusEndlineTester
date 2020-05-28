#include "testexplorer.h"

#include <QBoxLayout>
#include <QLabel>
#include <QStandardItemModel>
#include <QTreeView>

#include "nameplatewidget.h"

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
    this->setMaximumSize(300,this->maximumHeight());

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

    m_layout->addWidget(m_testCases);

    this->setLayout(m_layout);
  }

  void TestExplorer::newHostTestRun(const QString &hostTestRun)
  {
    auto old_model = dynamic_cast<QStandardItemModel*>(m_testCases->model());
    auto new_model = new QStandardItemModel();
    m_testCases->setModel(new_model);
    if(old_model != Q_NULLPTR){
        delete  old_model;
      }
    auto htrun = new QStandardItem(hostTestRun);
    new_model->appendRow(htrun);
    m_testCases->expandAll();
  }

  void TestExplorer::newHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    QStandardItem* htrun = findorCreateHtRun(hostTestRun);
    htrun->appendRow(new QStandardItem(hostTest));
    m_testCases->expandAll();
  }

  void TestExplorer::newTestCase(const QString &hostTestRun, const QString &hostTest, const QString &hostTestCase)
  {
    QStandardItem* hosttest = findorCreateHostTest(hostTestRun, hostTest);
    hosttest->appendRow(new QStandardItem(hostTestCase));
    m_testCases->expandAll();
  }

  QStandardItem *TestExplorer::findorCreateHtRun(const QString &hostTestRun)
  {
    auto model = dynamic_cast<QStandardItemModel*>(m_testCases->model());
    auto items = model->findItems(hostTestRun);

    QStandardItem* htrun = Q_NULLPTR;
    if(items.isEmpty()){
        htrun = new QStandardItem(hostTestRun);
        model->appendRow(htrun);
      }else{
        htrun = items.first();
      }
    return htrun;
  }

  // sorry for the ugly code
  QStandardItem *TestExplorer::findorCreateHostTest(const QString &hostTestRun, const QString &hostTest)
  {
    QStandardItem* htrun = findorCreateHtRun(hostTestRun);

    QStandardItem* hosttest = Q_NULLPTR;
    for(auto i = 0; i<htrun->rowCount(); i++){
        auto child = htrun->child(i);
        if(child->text() == hostTest){
            hosttest = child;
            break;
          }
      }

    if(hosttest == Q_NULLPTR){
        hosttest = new QStandardItem(hostTest);
        htrun->appendRow(hosttest);
      }

    return hosttest;

  }

} // namespace worlddirect
