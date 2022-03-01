#ifndef NAMEPLATEWIDGET_H
#define NAMEPLATEWIDGET_H

#include <QWidget>

namespace worlddirect {


class NamePlateWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit NamePlateWidget(QWidget *parent = nullptr);
    NamePlateWidget&  operator= ( const  NamePlateWidget& ) = delete;
    NamePlateWidget&  operator= ( const  NamePlateWidget&& ) = delete;

    NamePlateWidget ( const  NamePlateWidget& ) = delete;
    NamePlateWidget ( const  NamePlateWidget&& ) = delete;

    virtual ~NamePlateWidget() = default;

  public slots:
    virtual void setType(const QString& type) = 0;
    virtual void setHardwareVersion(const QString& hwVer) = 0;
    virtual void setEndpointName(const QString& ep) = 0;
    virtual void setIccId(const QString& icc) = 0;
    virtual void printNameplate() = 0;

}; // class NameplateWidget

} // namespace worlddirect

#endif // NAMEPLATEWIDGET_H
