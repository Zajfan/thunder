#include "settingsbrowser.h"
#include "ui_settingsbrowser.h"

SettingsBrowser::SettingsBrowser(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::SettingsBrowser) {

    ui->setupUi(this);

    ui->commitButton->setProperty("green", true);

    connect(ui->commitButton, &QToolButton::clicked, this, &SettingsBrowser::commited);
    connect(ui->revertButton, &QToolButton::clicked, this, &SettingsBrowser::reverted);
}

SettingsBrowser::~SettingsBrowser() {
    delete ui;
}

void SettingsBrowser::setModel(QObject *model) {
    ui->settingsView->setObject(model);
    connect(model, SIGNAL(updated()), ui->settingsView, SLOT(onUpdated()));
}

void SettingsBrowser::changeEvent(QEvent *event) {
    if(event->type() == QEvent::LanguageChange) {
        ui->retranslateUi(this);
    }
}
