#ifndef DOOGIE_CEF_APP_HANDLER_H_
#define DOOGIE_CEF_APP_HANDLER_H_

#include <QtWidgets>

#include "cosmetic_blocker.h"
#include "cef/cef_base.h"

namespace doogie {

// Handler for overall app features.
class CefAppHandler :
    public QObject,
    public CefApp,
    public CefBrowserProcessHandler,
    public CefRenderProcessHandler {
  Q_OBJECT

 public:
  CefAppHandler();

  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }

  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override {
    return this;
  }

  // Render process handler overrides...
  bool OnBeforeNavigation(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      CefRenderProcessHandler::NavigationType navigation_type,
      bool is_redirect) override;

  void SetBeforeNavCallback(
      std::function<bool(CefRefPtr<CefBrowser>,
                         CefRefPtr<CefFrame>,
                         CefRefPtr<CefRequest>,
                         CefRenderProcessHandler::NavigationType,
                         bool)> before_nav_callback) {
    before_nav_callback_ = before_nav_callback;
  }

  void OnContextCreated(CefRefPtr<CefBrowser> browser,
                        CefRefPtr<CefFrame> frame,
                        CefRefPtr<CefV8Context> context) override;

  void OnWebKitInitialized() override;

 private:
  CosmeticBlocker blocker_;
  std::function<bool(CefRefPtr<CefBrowser>,
                     CefRefPtr<CefFrame>,
                     CefRefPtr<CefRequest>,
                     CefRenderProcessHandler::NavigationType,
                     bool)> before_nav_callback_;

  IMPLEMENT_REFCOUNTING(CefAppHandler)
};

}  // namespace doogie

#endif  // DOOGIE_CEF_APP_HANDLER_H_
