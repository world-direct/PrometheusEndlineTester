#include "nameplate.h"

#include <QPainter>
#include <QPrinter>
#include <QWidget>

namespace worlddirect {

  NamePlate::NamePlate(QObject *parent)
    : QObject(parent),
      m_serialNumber(),
      m_endpointName(),
      m_iccId()
  {

  }

  void NamePlate::printNamePlate()
  {
    QWidget w;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("output.pdf");
    printer.setPageMargins(5, 5, 5, 5, QPrinter::Millimeter);
    printer.setFullPage(false);

    QPainter painter(&printer);

    double xscale = printer.pageRect().width() / double(w.width());
    double yscale = printer.pageRect().height() / double(w.height());
    double scale = qMin(xscale, yscale);
    painter.translate(printer.paperRect().center());
    painter.scale(scale, scale);
    painter.translate(-w.width()/ 2, -w.height()/ 2);
    w.render(&painter);
  }

  void NamePlate::sendNamePlateInfo()
  {

  }

} // namespace worlddirect
