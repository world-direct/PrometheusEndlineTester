#include "gatewaynameplatewidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QCoreApplication>
#include <QPrinter>
#include <QPainter>
#include <QPrintDialog>
#include <QPageSetupDialog>

#include "qrwidget.h"

namespace worlddirect {

GatewayNamePlateWidget::GatewayNamePlateWidget(QWidget *parent)
    : NamePlateWidget(parent),
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
      m_weeeMark(new QLabel())
{
//    setMinimumWidth(560);
//    setMaximumWidth(560);
    setMinimumWidth(316);
    setMaximumWidth(316);
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
//    int logo_size = 156;

    QPixmap ceMarkPix(":/PrometheusEndlineTester/images/ce_mark.svg");
    m_ceMark->setPixmap(ceMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));

    QPixmap weeeMarkPix(":/PrometheusEndlineTester/images/WEEE_mark.svg");
    m_weeeMark->setPixmap(weeeMarkPix.scaled(logo_size-10,logo_size-10,Qt::KeepAspectRatio));

//    int qr_size = 156;
    int qr_size = logo_size;

    m_qrCode->setMinimumWidth(qr_size);
    m_qrCode->setMinimumHeight(qr_size);

    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Raised);

    auto line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Raised);

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);

    //m_layout->addWidget(m_qrCode, 0,0,15,2,Qt::AlignCenter);

    {
        int c = 0;
        //                                       r  c  rs cs
        m_layout->addWidget(m_organizationLabel, 0, c, 1, 3, Qt::AlignCenter);

        m_layout->addWidget(line,                1, c, 1, 3);

        m_layout->addWidget(m_typeLabel,         2, c, 1, 2);
        m_layout->addWidget(m_hwVerLabel,        3, c, 1, 2);
        m_layout->addWidget(m_endpointLabel,     4, c, 1, 2);
        m_layout->addWidget(m_iccLabel,          5, c, 1, 2);
        m_layout->addWidget(line2,               6, c, 1, 3);

        c++;

        m_layout->addWidget(m_typeIndicator,     2, c, 1, 2, Qt::AlignRight);
        m_layout->addWidget(m_hwVerIndicator,    3, c, 1, 2, Qt::AlignRight);
        m_layout->addWidget(m_endpointIndicator, 4, c, 1, 2, Qt::AlignRight);
        m_layout->addWidget(m_iccIndicator,      5, c, 1, 2, Qt::AlignRight);
    }

//    m_layout->addWidget(m_ceMark,0,4,7,1,Qt::AlignCenter);
//    m_layout->addWidget(m_weeeMark, 8,4,7,1,Qt::AlignCenter);


    m_layout->addWidget(m_qrCode,                8, 0, 1, 1, Qt::AlignLeft);
    m_layout->addWidget(m_ceMark,                8, 1, 1, 1, Qt::AlignCenter);
    m_layout->addWidget(m_weeeMark,              8, 2, 1, 1, Qt::AlignRight);

    this->setLayout(m_layout);

}

void GatewayNamePlateWidget::setType(const QString &type)
{
    m_typeIndicator->setText(type);
}

void GatewayNamePlateWidget::setHardwareVersion(const QString &hwVer)
{
    m_hwVerIndicator->setText(hwVer);
}

void GatewayNamePlateWidget::setEndpointName(const QString &ep)
{
    m_endpointIndicator->setText(ep);
    m_qrCode->setQRData(ep);
}

void GatewayNamePlateWidget::setIccId(const QString &icc)
{
    m_iccIndicator->setText(icc);
}

void GatewayNamePlateWidget::printNameplate()
{
    QPrinter printer(QPrinter::HighResolution);
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
