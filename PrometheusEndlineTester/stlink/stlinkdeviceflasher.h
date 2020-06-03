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
    explicit StLinkDeviceFlasher(QObject* parent=nullptr);
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher&& ) = delete;

    StLinkDeviceFlasher ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher ( const  StLinkDeviceFlasher&& ) = delete;

  signals:
    void printData(const QByteArray &dt);
    void newLineReceived(const QByteArray &dt);

    void exitSuccess();
    void exitFailure();

    void targetConnected();
    void targetProgrammed();
    void targetReset();

  public slots:
    void connectToTarget();
    void programTarget();
    void resetTarget();

  private slots:
    void readProcessData();
    void errorOccurredSlot(QProcess::ProcessError error);
    void finishedSlot(int exitCode, QProcess::ExitStatus exitStatus);

  private :
    Mode m_mode;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_STLINKDEVICEFLASHER_H
