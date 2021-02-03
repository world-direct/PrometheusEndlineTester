#ifndef WORLDDIRECT_STLINKDEVICEFLASHER_H
#define WORLDDIRECT_STLINKDEVICEFLASHER_H

#include <QProcess>
#include <QString>


namespace worlddirect {

  class StLinkDeviceFlasher : public QProcess
  {
    Q_OBJECT
  private:
    enum class Mode{NONE, CONNECT, PROGRAM, RESET};

  public:
    enum class BinaryType{NONE, TEST, FIRMWARE};

  public:
    explicit StLinkDeviceFlasher(QObject* parent=nullptr);
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher&& ) = delete;

    StLinkDeviceFlasher ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher ( const  StLinkDeviceFlasher&& ) = delete;

  signals:
    void printData(const QByteArray &dt);
    void newLineReceived(const QByteArray &dt);

    void successMessage(const QString& msg);
    void errorMessage(const QString& msg);

    void targetConnected();
    void targetProgrammed();
    void targetReset();

  public slots:
    //void newHostTestRun(const QString& hostTestRun);

    void connectToTarget();
    void programTargetTest();
    void resetTarget();
    void programTargetFirmware();
    void programTarget(BinaryType type);
    void clearWriteProtection();

  private slots:
    void readProcessData();
    void errorOccurredSlot(QProcess::ProcessError error);
    void finishedSlot(int exitCode, QProcess::ExitStatus exitStatus);

  private:
    QString mode2String(Mode md);
    QString pathToCmd()const;

  private :
    Mode m_mode;
    QString m_testRun;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_STLINKDEVICEFLASHER_H
