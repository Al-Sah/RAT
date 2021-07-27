#ifndef CONNECTION_SETUP_DIALOG_H
#define CONNECTION_SETUP_DIALOG_H

#include <QDialog>

namespace Ui {
class connectionSetupDialog;
}

class ConnectionSetupDialog : public QDialog{
    Q_OBJECT

public:
    explicit ConnectionSetupDialog(QWidget *parent = nullptr);
    ~ConnectionSetupDialog();

signals:
    void infoEntered (const QString &text) const;

public slots:
    void saveAndQuit(QString &link,QString &pswd);

private:
    Ui::connectionSetupDialog *ui;
};

#endif // CONNECTION_SETUP_DIALOG_H
