#ifndef MODULEDIALOGUE_H
#define MODULEDIALOGUE_H

#include <QDialog>

namespace Ui {
class ModuleDialogue;
}
class BotCoreController;

class ModuleDialogue : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleDialogue(BotCoreController* modulePtr, QWidget *parent = nullptr);
    ~ModuleDialogue();

private:
    Ui::ModuleDialogue *ui;
};

#endif // MODULEDIALOGUE_H
