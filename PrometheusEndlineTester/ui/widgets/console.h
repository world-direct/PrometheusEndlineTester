#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

namespace worlddirect {

  class Console : public QPlainTextEdit
  {
    Q_OBJECT
  public:
    explicit Console(QWidget *parent = Q_NULLPTR);
    Console&  operator= ( const  Console& ) = delete;
    Console&  operator= ( const  Console&& ) = delete;

    Console ( const  Console& ) = delete;
    Console ( const  Console&& ) = delete;

    virtual ~Console() = default;

  signals:
    void getData(const QByteArray &data);

  public slots:
    void printData(const QByteArray &dt);

  protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *e) Q_DECL_OVERRIDE;
  };

}

#endif // CONSOLE_H
