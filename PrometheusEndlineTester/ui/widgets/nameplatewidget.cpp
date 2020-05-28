#include "nameplatewidget.h"
#include "qrwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>

namespace worlddirect {

  NamePlateWidget::NamePlateWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QGridLayout(this)),
      m_organizationLabel(new QLabel(qApp->organizationName(),this)),
      m_typeLabel(new QLabel(tr("Type:"), this)),
      m_typeIndicator(new QLabel(this)),
      m_endpointLabel(new QLabel(tr("Endpoint-Name:"),this)),
      m_endpointIndicator(new QLabel(this)),
      m_iccLabel(new QLabel(tr("ICC:"),this)),
      m_iccIndicator(new QLabel(this)),
      m_ceMark(new QLabel(this)),
      m_qrCode(new QRWidget(this)),
      m_weeeMark(new QLabel(this))
  {
    setMinimumWidth(300);
    setMaximumWidth(300);
    setMinimumHeight(150);
    setMaximumHeight(150);

    auto font = m_organizationLabel->font();
    font.setBold(true);
    m_organizationLabel->setFont(font);

    int logo_size = 60;

    QPixmap ceMarkPix(":/images/ce_mark.svg");
    m_ceMark->setPixmap(ceMarkPix.scaled(logo_size,logo_size,Qt::KeepAspectRatio));

    QPixmap weeeMarkPix(":/images/WEEE_mark.svg");
    m_weeeMark->setPixmap(weeeMarkPix.scaled(logo_size,logo_size,Qt::KeepAspectRatio));
    m_weeeMark->setMinimumWidth(logo_size);
    m_weeeMark->setMinimumHeight(logo_size);

    m_qrCode->setMinimumWidth(logo_size);
    m_qrCode->setMinimumHeight(logo_size);

    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Raised);

    auto line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Raised);

    m_layout->setMargin(5);
    m_layout->setHorizontalSpacing(2);
    m_layout->setVerticalSpacing(2);
    m_layout->addWidget(m_organizationLabel,0,0,1,6,Qt::AlignCenter);

    m_layout->addWidget(line,1,0,1,6);

    m_layout->addWidget(m_typeLabel,2,0,1,3);
    m_layout->addWidget(m_typeIndicator,2,1,1,3,Qt::AlignRight);
    m_layout->addWidget(m_endpointLabel,3,0,1,3);
    m_layout->addWidget(m_endpointIndicator,3,1,1,3,Qt::AlignRight);
    m_layout->addWidget(m_iccLabel,4,0,1,3);
    m_layout->addWidget(m_iccIndicator,4,1,1,3,Qt::AlignRight);

    m_layout->addWidget(line2,5,0,1,6);

    m_layout->addWidget(m_ceMark,6,0,1,2,Qt::AlignCenter);
    m_layout->addWidget(m_qrCode, 6,2,1,2,Qt::AlignCenter);
    m_layout->addWidget(m_weeeMark, 6,4,1,2,Qt::AlignCenter);

    this->setLayout(m_layout);

  }

  void NamePlateWidget::setType(const QString &type)
  {
    m_typeIndicator->setText(type);
  }

  void NamePlateWidget::setEndpointName(const QString &ep)
  {
    m_endpointLabel->setText(ep);
    m_qrCode->setQRData(ep);
  }

  void NamePlateWidget::setIccId(const QString &icc)
  {
    m_iccIndicator->setText(icc);
  }

} // namespace worlddirect
