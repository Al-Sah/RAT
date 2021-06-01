#include "serverinteractiongroupbox.h"
#include "ui_serverinteractiongroupbox.h"

#include "ServerInteraction.h"

ServerInteractionGroupBox::ServerInteractionGroupBox(ServerInteraction *si_ptr, QWidget *parent) : QGroupBox(parent), ui(new Ui::ServerInteractionGroupBox) {
    this->si_ptr = si_ptr;
    ui->setupUi(this);

    connect(ui->updateTargetsButton, SIGNAL(clicked(bool)), si_ptr, SLOT(do_targets_update()));
    connect(ui->botsList, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(showTargetInfo(QListWidgetItem *)));
    connect(ui->targetModulesList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), si_ptr, SLOT(showTargetModule(QListWidgetItem *)));
    connect(this, SIGNAL(setActiveTerget(QString)), si_ptr, SLOT(setActiveTerget(QString)));
}


ServerInteractionGroupBox::~ServerInteractionGroupBox(){
    delete ui;
}


void ServerInteractionGroupBox::handleUpdatedTargets(QList<TargetInfo> bots, QList<TargetInfo> users){
    this->bots = bots;
    this->users = users;

    this->ui->botsList->clear();

    for(auto bot: bots){
        this->ui->botsList->addItem(new QListWidgetItem(bot.id));
    }

    for(auto user: users){
        this->ui->botsList->addItem(new QListWidgetItem(user.id));
    }
}

void ServerInteractionGroupBox::showTargetInfo(QListWidgetItem *item){
    emit setActiveTerget(item->text());
    for(auto bot: bots){
        if(bot.id == item->text()){
           updateTargetUI(bot);
        }
    }
}


void ServerInteractionGroupBox::updateTargetUI(TargetInfo& info){
    ui->targetModulesList->clear();

    ui->label_adress_val->setText(info.characteristics["adress"]);
    ui->label_targetId_val->setText(info.id);

    for(auto module: info.modules){
        this->ui->targetModulesList->addItem(new QListWidgetItem(module));
    }
}

