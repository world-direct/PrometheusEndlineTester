#ifndef WORLDDIRECT_TESTEXPLORER_H
#define WORLDDIRECT_TESTEXPLORER_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QLabel;
class QTreeView;
class QStandardItem;

QT_END_NAMESPACE

namespace worlddirect {

  class NamePlateWidget;
  class AbstractTest;

  class TestExplorer : public QWidget
  {
    Q_OBJECT
  public:
    explicit TestExplorer(QWidget *parent = nullptr);
    TestExplorer&  operator= ( const  TestExplorer& ) = delete;
    TestExplorer&  operator= ( const  TestExplorer&& ) = delete;

    TestExplorer ( const  TestExplorer& ) = delete;
    TestExplorer ( const  TestExplorer&& ) = delete;

    virtual ~TestExplorer() = default;

  public slots:
    void newHostTestRun(const QString& hostTestRun);
    void newHostTest(const QString& hostTestRun, const QString& hostTest);
    void newTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);

  private:
   QStandardItem* findorCreateHtRun(const QString& hostTestRun);
   QStandardItem* findorCreateHostTest(const QString& hostTestRun, const QString& hostTest);

  private:
    QWidget* m_testSetup;
    NamePlateWidget* m_namePlate;
    QLabel* m_testCaseSumary;
    QTreeView* m_testCases;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_TESTEXPLORER_H
