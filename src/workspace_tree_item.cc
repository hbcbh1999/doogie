#include "workspace_tree_item.h"
#include "page_tree.h"
#include "util.h"

namespace doogie {

WorkspaceTreeItem::WorkspaceTreeItem(const Workspace& workspace)
    : QTreeWidgetItem(PageTree::kWorkspaceItemType), workspace_(workspace) {
  setFlags(Qt::ItemIsEditable | Qt::ItemIsDragEnabled |
           Qt::ItemIsDropEnabled | Qt::ItemIsEnabled);
  setText(0, workspace.FriendlyName());
}

void WorkspaceTreeItem::AfterAdded() {
  auto menu_button = new QToolButton;
  menu_button->setIcon(
        Util::CachedIcon(":/res/images/fontawesome/bars.png"));
  menu_button->setAutoRaise(true);
  treeWidget()->connect(menu_button, &QToolButton::clicked, [=]() {
    auto tree = static_cast<PageTree*>(treeWidget());
    QMenu menu;
    tree->ApplyWorkspaceMenu(&menu, workspace_, this);
    menu.exec(menu_button->mapToGlobal(menu_button->rect().bottomLeft()));
  });
  treeWidget()->setItemWidget(this,
                              PageTreeItem::kCloseButtonColumn,
                              menu_button);

  // Gotta call it on my children too sadly
  for (int i = 0; i < childCount(); i++) {
    static_cast<PageTreeItem*>(child(i))->AfterAdded();
  }
}

void WorkspaceTreeItem::TextChanged() {
  if (text(0) != workspace_.Name()) {
    if (Workspace::NameInUse(text(0))) {
      QMessageBox::critical(nullptr,
                            "Invalid Name",
                            "Name already in use by another workspace");
      treeWidget()->editItem(this);
      return;
    }
    workspace_.SetName(text(0));
    workspace_.Save();
  }
}

void WorkspaceTreeItem::ChildCloseCancelled() {
  close_on_empty_not_cancelled_ = false;
  send_close_event_not_cancelled_ = true;
}

void WorkspaceTreeItem::ChildCloseCompleted() {
  if (close_on_empty_not_cancelled_ && childCount() == 0) {
    if (send_close_event_not_cancelled_) {
      auto tree = static_cast<PageTree*>(treeWidget());
      emit tree->WorkspaceClosed(workspace_.Id());
    }
    delete this;
  }
}

}  // namespace doogie