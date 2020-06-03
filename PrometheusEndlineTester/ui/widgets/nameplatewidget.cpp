#include "nameplatewidget.h"
#include "qrwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QPrinter>
#include <QPainter>

namespace worlddirect {

  NamePlateWidget::NamePlateWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QGridLayout(this)),
      m_organizationLabel(new QLabel(qApp->organizationName(),this)),
      m_typeLabel(new QLabel(tr("Type:"), this)),
      m_typeIndicator(new QLabel(this)),
      m_hwVerLabel(new QLabel(tr("Version:"), this)),
      m_hwVerIndicator(new QLabel(this)),
      m_endpointLabel(new QLabel(tr("Endpoint-Name:"),this)),
      m_endpointIndicator(new QLabel(this)),
      m_iccLabel(new QLabel(tr("ICC:"),this)),
      m_iccIndicator(new QLabel(this)),
      m_ceMark(new QLabel(this)),
      m_qrCode(new QRWidget(this)),
      m_weeeMark(new QLabel(this))
  {
    setMinimumWidth(297);
    setMaximumWidth(297);
    setMinimumHeight(210);
    setMaximumHeight(210);

    auto font = m_organizationLabel->font();
    font.setBold(true);
    m_organizationLabel->setFont(font);

    m_typeIndicator->setAlignment(Qt::AlignRight);

    m_hwVerIndicator->setAlignment(Qt::AlignRight);

    m_endpointIndicator->setAlignment(Qt::AlignRight);

    m_iccIndicator->setAlignment(Qt::AlignRight);

    int logo_size = minimumWidth()/3;

    QPixmap ceMarkPix(":/PrometheusEndlineTester/images/ce_mark.svg");
    m_ceMark->setPixmap(ceMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));

    QPixmap weeeMarkPix(":/PrometheusEndlineTester/images/WEEE_mark.svg");
    m_weeeMark->setPixmap(weeeMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));
    //m_weeeMark->setMinimumWidth(logo_size);
    //m_weeeMark->setMinimumHeight(logo_size);

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
    m_layout->addWidget(m_typeIndicator,2,3,1,3,Qt::AlignRight);
    m_layout->addWidget(m_hwVerLabel,3,0,1,3);
    m_layout->addWidget(m_hwVerIndicator,3,3,1,3, Qt::AlignRight);
    m_layout->addWidget(m_endpointLabel,4,0,1,3);
    m_layout->addWidget(m_endpointIndicator,4,3,1,3,Qt::AlignRight);
    m_layout->addWidget(m_iccLabel,5,0,1,3);
    m_layout->addWidget(m_iccIndicator,5,3,1,3,Qt::AlignRight);

    m_layout->addWidget(line2,6,0,1,6);

    m_layout->addWidget(m_ceMark,7,0,1,2,Qt::AlignCenter);
    m_layout->addWidget(m_qrCode, 7,2,1,2,Qt::AlignCenter);
    m_layout->addWidget(m_weeeMark, 7,4,1,2,Qt::AlignCenter);

    this->setLayout(m_layout);

  }

  void NamePlateWidget::setType(const QString &type)
  {
    m_typeIndicator->setText(type);
  }

  void NamePlateWidget::setHardwareVersion(const QString &hwVer)
  {
    m_hwVerIndicator->setText(hwVer);
  }

  void NamePlateWidget::setEndpointName(const QString &ep)
  {
    m_endpointIndicator->setText(ep);
    m_qrCode->setQRData(ep);
  }

  void NamePlateWidget::setIccId(const QString &icc)
  {
    m_iccIndicator->setText(icc);
  }

  void NamePlateWidget::printNameplate()
  {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(m_endpointIndicator->text()+".pdf");
    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    printer.setFullPage(false);
    printer.setPageSize(QPrinter::A8);
    printer.setColorMode(QPrinter::GrayScale);
    printer.setOrientation(QPrinter::Landscape);

    QPainter painter(&printer);

    double w = double(this->width());
    double h = double(this->height());

    double xscale = printer.pageRect().width() / w;
    double yscale = printer.pageRect().height() / h;
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().center());
    painter.scale(scale, scale);
    painter.translate(-1 * w / 2, -1 * h/ 2);
    this->render(&painter);

  }

} // namespace worlddirect
