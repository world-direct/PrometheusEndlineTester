#ifndef WORLDDIRECT_HOSTTEST_H
#define WORLDDIRECT_HOSTTEST_H

#include <QStandardItemModel>


namespace worlddirect {

  class HostTest : public QStandardItem
  {
  public:
    HostTest();
    explicit HostTest(const QString &text);
    HostTest(const QIcon &icon, const QString &text);

    HostTest&  operator= ( const  HostTest& ) = delete;
    HostTest&  operator= ( const  HostTest&& ) = delete;

    HostTest ( const  HostTest& ) = delete;
    HostTest ( const  HostTest&& ) = delete;

    virtual ~HostTest() = default;

  public:
    void setStarted();
    void setPassed();
    void setFailed();
    void setTimedout();

  };

} // namespace worlddirect

#endif // WORLDDIRECT_HOSTTEST_H
