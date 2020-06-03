#ifndef WORLDDIRECT_HOSTTESTRUN_H
#define WORLDDIRECT_HOSTTESTRUN_H

#include <QStandardItem>

namespace worlddirect {

  class HostTestRun : public QStandardItem
  {
  public:
    HostTestRun();
    explicit HostTestRun(const QString &text);
    HostTestRun(const QIcon &icon, const QString &text);

    HostTestRun&  operator= ( const  HostTestRun& ) = delete;
    HostTestRun&  operator= ( const  HostTestRun&& ) = delete;

    HostTestRun ( const  HostTestRun& ) = delete;
    HostTestRun ( const  HostTestRun&& ) = delete;

    virtual ~HostTestRun() = default;

  public:
    void setStarted();
    void setPassed();
    void setFailed();
    void setTimedout();

  };

} // namespace worlddirect

#endif // WORLDDIRECT_HOSTTESTRUN_H
