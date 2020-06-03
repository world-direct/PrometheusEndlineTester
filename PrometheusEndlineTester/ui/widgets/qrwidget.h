#ifndef WORLDDIRECT_QRWIDGET_H
#define WORLDDIRECT_QRWIDGET_H

#include <QWidget>

namespace worlddirect {

    class QRWidget : public QWidget{
        Q_OBJECT
    public:
        explicit QRWidget(QWidget *parent = 0);
        QRWidget&  operator= ( const  QRWidget& ) = delete;
        QRWidget&  operator= ( const  QRWidget&& ) = delete;

        QRWidget ( const  QRWidget& ) = delete;
        QRWidget ( const  QRWidget&& ) = delete;

        virtual ~QRWidget() = default;

        void setQRData(const QString& dta);

        // QWidget interface
    protected:
        virtual void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;

    private:
        void paintQR(QPainter &painter, const QSize sz, const QString &dta, QColor fg, QColor bg);

    private:
        QString m_data;

    };

} // namespace worlddirect

#endif // WORLDDIRECT_QRWIDGET_H
