#ifndef WORLDDIRECT_DCPLATEWIDGET_H
#define WORLDDIRECT_DCPLATEWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE

class QLabel;
class QGridLayout;

QT_END_NAMESPACE

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

  private:
    QGridLayout* m_layout;
    QLabel* m_dcLabel;
  };

} // namespace worlddirect

#endif // WORLDDIRECT_DCPLATEWIDGET_H
