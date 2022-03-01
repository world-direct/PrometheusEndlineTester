#ifndef WORLDDIRECT_PROMETHEUSNAMEPLATEWIDGET_H
#define WORLDDIRECT_PROMETHEUSNAMEPLATEWIDGET_H

// #include <QWidget>
#include "nameplatewidget.h"

QT_BEGIN_NAMESPACE

class QLabel;
class QGridLayout;

QT_END_NAMESPACE

namespace worlddirect {

  class QRWidget;
  class PrometheusNamePlateWidget : public NamePlateWidget
  {
    Q_OBJECT
  public:
    explicit PrometheusNamePlateWidget(QWidget *parent = nullptr);
    PrometheusNamePlateWidget&  operator= ( const  PrometheusNamePlateWidget& ) = delete;
    PrometheusNamePlateWidget&  operator= ( const  PrometheusNamePlateWidget&& ) = delete;

    PrometheusNamePlateWidget ( const  PrometheusNamePlateWidget& ) = delete;
    PrometheusNamePlateWidget ( const  PrometheusNamePlateWidget&& ) = delete;

    virtual ~PrometheusNamePlateWidget() = default;

  public slots:
    virtual void setType(const QString& type) override;
    virtual void setHardwareVersion(const QString& hwVer) override;
    virtual void setEndpointName(const QString& ep) override;
    virtual void setIccId(const QString& icc) override;
    virtual void printNameplate() override;

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
    QLabel* m_acLabel;

  };

} // namespace worlddirect

#endif // WORLDDIRECT_PROMETHEUSNAMEPLATEWIDGET_H
