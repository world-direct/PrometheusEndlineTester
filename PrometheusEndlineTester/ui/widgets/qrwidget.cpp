#include "qrwidget.h"
#include <QPainter>
#include <QDebug>

#include <QrCode.hpp>

namespace worlddirect {


  QRWidget::QRWidget(QWidget *parent) :
    QWidget(parent),
    m_data()//Note: The encoding fails with empty string so I just default to something else. Use the setQRData() call to change this.
  {

  }

  void QRWidget::setQRData(const QString &dta){
    this->m_data=dta;
    update();
  }

  void QRWidget::paintEvent(QPaintEvent *)
  {
    QPainter painter(this);
    //QColor fg("black");
    auto fg = palette().foreground().color();
    //QColor bg("white");
    auto bg = palette().background().color();
    //QSize sz =this->size();
    auto mindim= qMin(width(), height());
    QSize sz(mindim, mindim);

    paintQR(painter, sz, m_data, fg, bg);

  }

  void QRWidget::paintQR(QPainter &painter, const QSize sz, const QString &dta, QColor fg, QColor bg)
  {

    painter.setBrush(bg);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0,0,sz.width(),sz.height());

    if(m_data.isEmpty()){
        return;
      }
    // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(dta.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);
    const int s=qr.getSize()>0?qr.getSize():1;
    const double w=sz.width();
    const double h=sz.height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);
    // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
    // It expects background to be prepared already (in white or whatever is preferred).
    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);
    for(int y=0; y<s; y++) {
        for(int x=0; x<s; x++) {
            const int color=qr.getModule(x, y);  // 0 for white, 1 for black
            if(0!=color) {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
              }
          }
      }
  }

} // namespace worlddirect
