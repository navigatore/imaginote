#include "keyboardlayouthelpdialog.h"
#include "ui_keyboardlayouthelpdialog.h"

KeyboardLayoutHelpDialog::KeyboardLayoutHelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeyboardLayoutHelpDialog)
{
    ui->setupUi(this);
}

KeyboardLayoutHelpDialog::~KeyboardLayoutHelpDialog()
{
    delete ui;
}
