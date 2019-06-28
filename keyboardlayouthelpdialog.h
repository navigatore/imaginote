#pragma once
#include <QDialog>

namespace Ui {
class KeyboardLayoutHelpDialog;
}

class KeyboardLayoutHelpDialog : public QDialog {
  Q_OBJECT

 public:
  explicit KeyboardLayoutHelpDialog(QWidget *parent = nullptr);
  ~KeyboardLayoutHelpDialog();

 private:
  Ui::KeyboardLayoutHelpDialog *ui;
};
