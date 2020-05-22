#ifndef NODEEDITDIALOG_H
#define NODEEDITDIALOG_H

#include <QDialogButtonBox>
#include <QMessageBox>
#include <QTabWidget>
#include <QVBoxLayout>

#include "nodeeditgeneraltab.h"
#include "nodeedittransporttab.h"

namespace Ui {
class NodeEditDialog;
}

class NodeEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NodeEditDialog(QWidget *parent = 0);
    ~NodeEditDialog();

private:
    Ui::NodeEditDialog *ui;
    QTabWidget *tabWidget;
    NodeEditGeneralTab *generalTab;
    NodeEditTransportTab *transportTab;
    QDialogButtonBox *buttonBox;

private slots:
    void accept();
    void reject();
};

#endif // NODEEDITDIALOG_H