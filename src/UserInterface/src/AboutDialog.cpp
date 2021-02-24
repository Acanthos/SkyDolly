#include <QDialog>

#include "../../Kernel/src/Version.h"
#include "AboutDialog.h"
#include "ui_AboutDialog.h"

// PUBLIC

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    initUi();
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

// PRIVATE

void AboutDialog::initUi()
{
    ui->aboutLabel->setText(tr("%1\nVersion %2\n\nMIT License").arg(Version::getApplicationName(), Version::getApplicationVersion()));
}
