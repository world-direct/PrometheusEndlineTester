#ifndef WORLDDIRECT_NAMEPLATEWIDGET_H
#define WORLDDIRECT_NAMEPLATEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QLabel;
class QGridLayout;

QT_END_NAMESPACE

namespace worlddirect {

  class QRWidget;
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
    void setType(const QString& type);
    void setHardwareVersion(const QString& hwVer);
    void setEndpointName(const QString& ep);
    void setIccId(const QString& icc);
    void printNameplate();


  private:
    QGridLayout* m_layout;
    QLabel* m_organizationLabel;
    QLabel* m_typeLabel;
    QLabel* m_typeIndicator;
    QLabel* m_hwVerLabel;
    QLabel* m_hwVerIndicator;
    QLabel* m_endpointLabel;
    QLabel* m_endpointIndicator;
    QLabel* m_iccLabel;
    QLabel* m_iccIndicator;
    QLabel* m_ceMark;
    QRWidget* m_qrCode;
    QLabel* m_weeeMark;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_NAMEPLATEWIDGET_H
