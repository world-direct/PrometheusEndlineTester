#ifndef WORLDDIRECT_TESTEXPLORER_H
#define WORLDDIRECT_TESTEXPLORER_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QLabel;
class QTreeView;

QT_END_NAMESPACE

namespace worlddirect {

  class NamePlateWidget;
  class DcPlateWidget;
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
    void startTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void failedTestCase(const QString& hostTestRun, const QString& hostTest, const QString& hostTestCase);
    void passedHostTest(const QString& hostTestRun, const QString& hostTest);
    void failedHostTest(const QString& hostTestRun, const QString& hostTest);
    void passedHostTestRun(const QString& hostTestRun);
    void failedHostTestRun(const QString& hostTestRun);

    void typeReceived(const QString& type);
    void hardwareVersionReceived(const QString& hwVer);
    void endpointNameReceived(const QString& epName);
    void iccIdReceived(const QString& iccId);

    void printNameplate();

  private:
    QWidget* m_testSetup;
    NamePlateWidget* m_namePlate;
    DcPlateWidget* m_dcPlate;
    QLabel* m_testCaseSumary;
    QTreeView* m_testCases;


  };

} // namespace worlddirect

#endif // WORLDDIRECT_TESTEXPLORER_H
