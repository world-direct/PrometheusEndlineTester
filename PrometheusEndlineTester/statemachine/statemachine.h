#ifndef WORLDDIRECT_STATEMACHINE_H
#define WORLDDIRECT_STATEMACHINE_H

#include <QObject>
#include <QStateMachine>


namespace worlddirect {
  class MainWindow;
  class StLinkDeviceFlasher;
  class MbedHostTestRunner;
  class DeviceProvisioning;

  class StateMachine : public QStateMachine
  {
    Q_OBJECT
  public:
    explicit StateMachine(QObject* parent = Q_NULLPTR);
    StateMachine ( const  StateMachine& ) = delete;
    StateMachine ( const  StateMachine&& ) = delete;

    StateMachine&  operator= ( const  StateMachine& ) = delete;
    StateMachine&  operator= ( const  StateMachine&& ) = delete;

    virtual ~StateMachine() = default;

    void createStates();

  signals:
    void printData(const QByteArray &dt);

  private:
    MainWindow* m_ui;
    StLinkDeviceFlasher* m_stflash;
    MbedHostTestRunner *m_htrun;
    DeviceProvisioning * m_provisioning;
  };

} // namespace worlddirect

#endif // WORLDDIRECT_STATEMACHINE_H
