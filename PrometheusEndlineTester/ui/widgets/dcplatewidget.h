#ifndef WORLDDIRECT_DCPLATEWIDGET_H
#define WORLDDIRECT_DCPLATEWIDGET_H

#include <QWidget>

namespace worlddirect {

  class DcPlateWidget : public QWidget
  {
    Q_OBJECT
  public:
    explicit DcPlateWidget(QWidget *parent = nullptr);
    DcPlateWidget&  operator= ( const  DcPlateWidget& ) = delete;
    DcPlateWidget&  operator= ( const  DcPlateWidget&& ) = delete;

    DcPlateWidget ( const  DcPlateWidget& ) = delete;
    DcPlateWidget ( const  DcPlateWidget&& ) = delete;

    virtual ~DcPlateWidget() = default;

  public slots:
    void printNameplate();

  };

} // namespace worlddirect

#endif // WORLDDIRECT_DCPLATEWIDGET_H
