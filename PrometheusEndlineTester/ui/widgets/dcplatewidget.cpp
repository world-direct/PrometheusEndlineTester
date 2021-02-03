#include "dcplatewidget.h"

#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QPrinter>

namespace worlddirect {

  DcPlateWidget::DcPlateWidget(QWidget *parent)
    : QWidget(parent)
  {

    setMinimumWidth(560);
    setMaximumWidth(560);
    setMinimumHeight(168);
    setMaximumHeight(168);

    auto m_layout = new QGridLayout();
    auto m_dcLabel = new QLabel();

    QPixmap acLabelPix(":/PrometheusEndlineTester/images/LabelDC.png");
    m_dcLabel->setPixmap(acLabelPix.scaled(168*3, 168, Qt::KeepAspectRatio));

    m_layout->setMargin(0);
    m_layout->setHorizontalSpacing(0);
    m_layout->setVerticalSpacing(0);

    m_layout->addWidget(m_dcLabel,0,0,1,1,Qt::AlignCenter);

    setLayout(m_layout);

  }

  void DcPlateWidget::printNameplate()
  {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("DcPlate.pdf");
    printer.setPageMargins(2, 2, 2, 2, QPrinter::Millimeter);
    printer.setFullPage(false);
    //printer.setPageSize(QPrinter::A8);
    printer.setPageSizeMM(QSizeF(100.0 , 30.0));
    printer.setColorMode(QPrinter::Color);
    //printer.setOrientation(QPrinter::Landscape);

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
