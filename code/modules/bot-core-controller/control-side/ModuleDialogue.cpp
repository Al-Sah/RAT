#include "ModuleDialogue.h"
#include "ui_ModuleDialogue.h"

ModuleDialogue::ModuleDialogue(BotCoreController* modulePtr, QWidget *parent) : QDialog(parent), ui(new Ui::ModuleDialogue) {
    ui->setupUi(this);
}

ModuleDialogue::~ModuleDialogue()
{
    delete ui;
}
