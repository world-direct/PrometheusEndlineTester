#include "nameplatewidget.h"
#include "qrwidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPageSetupDialog>

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
      m_ceMark(new QLabel()),
      m_qrCode(new QRWidget()),
      m_weeeMark(new QLabel()),
      m_acLabel(new QLabel())
  {
    setMinimumWidth(560);
    setMaximumWidth(560);
    setMinimumHeight(168);
    setMaximumHeight(168);

    m_organizationLabel->setAlignment(Qt::AlignCenter);
    auto font = m_organizationLabel->font();
    font.setBold(true);
    m_organizationLabel->setFont(font);

    font = m_typeLabel->font();
    font.setPointSize(font.pointSize()+1);
    font.setBold(true);
    m_typeLabel->setFont(font);

    m_typeIndicator->setAlignment(Qt::AlignRight);
    m_typeIndicator->setFont(font);

    m_hwVerLabel->setFont(font);

    m_hwVerIndicator->setAlignment(Qt::AlignRight);
    m_hwVerIndicator->setFont(font);

    m_endpointLabel->setFont(font);

    m_endpointIndicator->setAlignment(Qt::AlignRight);
    m_endpointIndicator->setFont(font);

    m_iccLabel->setFont(font);

    m_iccIndicator->setAlignment(Qt::AlignRight);
    m_iccIndicator->setFont(font);

    int logo_size = minimumHeight()/15.0*7.0;

    QPixmap ceMarkPix(":/PrometheusEndlineTester/images/ce_mark.svg");
    m_ceMark->setPixmap(ceMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));

    QPixmap weeeMarkPix(":/PrometheusEndlineTester/images/WEEE_mark.svg");
    m_weeeMark->setPixmap(weeeMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));

    int qr_size = 156;

    m_qrCode->setMinimumWidth(qr_size);
    m_qrCode->setMinimumHeight(qr_size);

    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Raised);

    auto line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Raised);

    QPixmap acLabelPix(":/PrometheusEndlineTester/images/LabelAC.png");
    m_acLabel->setPixmap(acLabelPix.scaled(329, 80, Qt::KeepAspectRatio));

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);

    m_layout->addWidget(m_qrCode, 0,0,15,2,Qt::AlignCenter);

    m_layout->addWidget(m_organizationLabel,0,2,1,2,Qt::AlignCenter);

    m_layout->addWidget(line,1,2,1,2);

    m_layout->addWidget(m_typeLabel,2,2,1,1);
    m_layout->addWidget(m_typeIndicator,2,3,1,1,Qt::AlignRight);
    m_layout->addWidget(m_hwVerLabel,3,2,1,1);
    m_layout->addWidget(m_hwVerIndicator,3,3,1,1, Qt::AlignRight);
    m_layout->addWidget(m_endpointLabel,4,2,1,1);
    m_layout->addWidget(m_endpointIndicator,4,3,1,1,Qt::AlignRight);
    m_layout->addWidget(m_iccLabel,5,2,1,1);
    m_layout->addWidget(m_iccIndicator,5,3,1,1,Qt::AlignRight);

    m_layout->addWidget(line2,6,2,1,2);

    m_layout->addWidget(m_ceMark,0,4,7,1,Qt::AlignCenter);
    m_layout->addWidget(m_weeeMark, 8,4,7,1,Qt::AlignCenter);

    m_layout->addWidget(m_acLabel, 7, 2, 8, 2, Qt::AlignCenter);

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
    //    printer.setOutputFormat(QPrinter::PdfFormat);
    //    printer.setOutputFileName(m_endpointIndicator->text()+".pdf");
    //    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
    //    printer.setFullPage(false);
    //    printer.setPageSizeMM(QSizeF(100.0 , 30.0));
    //    printer.setColorMode(QPrinter::GrayScale);

    //    QPainter painter(&printer);

    //    double w = double(this->width());
    //    double h = double(this->height());

    //    double xscale = printer.pageRect().width() / w;
    //    double yscale = printer.pageRect().height() / h;
    //    double scale = qMin(xscale, yscale);
    //    painter.translate(printer.paperRect().center());
    //    painter.scale(scale, scale);
    //    painter.translate(-1 * w / 2, -1 * h/ 2);
    //    this->render(&painter);

    //    QPageSetupDialog pagesetdialog(&printer, this);
    //    if(pagesetdialog.exec() == QDialog::Accepted){
    //      }

    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);

        double w = double(this->width());
        double h = double(this->height());

        double xscale = printer.pageRect().width() / w;
        double yscale = printer.pageRect().height() / h;
        double scale = qMin(xscale, yscale);
        painter.translate(printer.paperRect().center());
        painter.scale(scale, scale);
        painter.translate(-1 * w / 2, -1 * h/ 2);
        printer.setPageMargins(2, 2, 2, 2, QPrinter::Millimeter);
        this->render(&painter);
      }

  }

} // namespace worlddirect
