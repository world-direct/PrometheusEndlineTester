#ifndef MBEDHOSTTESTRUNNER_H
#define MBEDHOSTTESTRUNNER_H

#include<QSerialPort>
#include<QString>
#include<QByteArray>
#include <QTimer>


namespace worlddirect {


  class MbedHostTestRunner : public QSerialPort
  {
    Q_OBJECT
  public:
    explicit MbedHostTestRunner(QObject *parent = Q_NULLPTR);
    MbedHostTestRunner&  operator= ( const  MbedHostTestRunner& ) = delete;
    MbedHostTestRunner&  operator= ( const  MbedHostTestRunner&& ) = delete;

    MbedHostTestRunner ( const  MbedHostTestRunner& ) = delete;
    MbedHostTestRunner ( const  MbedHostTestRunner&& ) = delete;

    virtual ~MbedHostTestRunner();

  signals:
    void serialOpened();
    void printData(const QByteArray& dt);
    void newLineReceived(const QByteArray &dt);
    void kvReceived(const QByteArray& key, const QList<QByteArray>& val);
    void msgReceived(const QByteArray& msg);

    void setupReceived();
    void endReceived(bool success);
    void exitReceived(int code);
    void syncReceived(const QUuid &uuid);

    void timeoutReceived(int sek);
    void hostTestNameReceived(const QString& hostTestName);
    void hostTestVersionReceived(const QString& version);
    void testCaseCountReceived(int cnt);
    void testCaseSummaryReceived(int passed, int failed);

    void testCaseNameReceived(const QString& testCaseName);
    void testCaseStartReeived(const QString& testCaseName);
    void testCaseFinishReceived(const QString& testCaseName, int passed, int failed);

    void typeReceived(const QString& epName);
    void hardwareVersionReceived(const QString& epName);
    void endpointNameReceived(const QString& epName);
    void iccIdReceived(const QString& iccId);

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

    void successMessage(const QString& msg);
    void errorMessage(const QString& msg);

    void firmwareStarted();

  public slots:
    void openSerial();
    void serialSendSync();
    void serialSendSyncUuid(const QUuid &uuid);

    void sendPSK(const QString& key);

  private slots:
    void readSerialData();
    void parseNewLine(const QByteArray &dt);
    void syncTimedOut();

  private:
    qint64 sendKv(const QByteArray &key, const QByteArray &val);
    qint64 sendString(const QByteArray& str);
    qint64 sendSync(const QUuid &uuid);
    qint64 sendSync(const QByteArray &uuid);

    void parseMsg(const QByteArray& line);
    void parseKv(const QByteArray& line);
    void checkForWellKnownKv(const QByteArray& key, const QList<QByteArray>& val);

    void newSyncReceived(const QUuid &uuid);
    void newHostTestReceived(const QString& hostTestName);
    void newTestCaseReceived(const QString& testCaseName);

    qint64 writePreamble();
    qint64 writeString(const QByteArray &str);
    qint64 writeSeperator();
    qint64 writePostamble();
    qint64 writeEndl();

    QString errorCode2Msg()const;

  private:
    QString m_testRun;
    QString m_hostTest;
    QTimer m_syncTimeout;

  };

}

#endif // MBEDHOSTTESTRUNNER_H
