#include "console.h"

#include <QScrollBar>

namespace worlddirect {

  Console::Console(QWidget *parent) :
    QPlainTextEdit(parent)
  {
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
  }

  void Console::printData(const QByteArray &dt)
  {
    insertPlainText(dt);
    insertPlainText("\n");

    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
  }

  void Console::keyPressEvent(QKeyEvent *e)
  {
    switch (e->key()) {
      case Qt::Key_Backspace:
      case Qt::Key_Left:
      case Qt::Key_Right:
      case Qt::Key_Up:
      case Qt::Key_Down:
        break;
      case Qt::Key_Return:
      case Qt::Key_Enter:
        //emit getData("{{__sync;8ca5a20b-16fa-4bf2-99a0-49e5e02ca9b9}}\r\n");
        break;
      default:
        QPlainTextEdit::keyPressEvent(e);
        emit getData(e->text().toLocal8Bit());
      }
  }

  void Console::mousePressEvent(QMouseEvent *e)
  {
    Q_UNUSED(e)
    setFocus();
  }

  void Console::mouseDoubleClickEvent(QMouseEvent *e)
  {
    Q_UNUSED(e)
  }

  void Console::contextMenuEvent(QContextMenuEvent *e)
  {
    Q_UNUSED(e)
  }
}
