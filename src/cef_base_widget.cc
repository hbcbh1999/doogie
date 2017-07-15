#include "cef_base_widget.h"

namespace doogie {

CefBaseWidget::CefBaseWidget(Cef* cef, QWidget*  parent)
    : QWidget(parent), cef_(cef) {
  InitWindowInfo();
}

CefBaseWidget::~CefBaseWidget() {

}

const CefWindowInfo& CefBaseWidget::WindowInfo() {
  return window_info_;
}

void CefBaseWidget::moveEvent(QMoveEvent*) {
  this->UpdateSize();
}

void CefBaseWidget::resizeEvent(QResizeEvent*) {
  this->UpdateSize();
}

}  // namespace doogie
