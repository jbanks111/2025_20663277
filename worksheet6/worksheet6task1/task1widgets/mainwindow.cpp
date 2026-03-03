#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::handleSubtractButton);

    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    /* Create/allocate the ModelList */
    this->partList = new ModelPartList("PartsList");

    /* Link it to the tree view in the GUI */
    ui->treeView->setModel(this->partList);

    ui->treeView->addAction(ui->actionItem_Options);

    /* Manually create a model tree = there are much better and more flexible ways of doing this,
       e.g. with nested functions. This is just a quick example as a starting point. */
    ModelPart* rootItem = this->partList->getRootItem();

    /* Add 3 top level items */
    for (int i = 0; i < 3; i++) {

        /* Create strings for both data columns */
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        /* Create child item */
        ModelPart* childItem = new ModelPart({ name, visible });

        /* Append to tree top-level */
        rootItem->appendChild(childItem);

        /* Add 5 sub-items */
        for (int j = 0; j < 5; j++) {

            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name, visible });

            /* Append to parent */
            childItem->appendChild(childChildItem);
        }
    }
    connect(ui->treeView,
        &QTreeView::clicked,
        this,
        &MainWindow::handleTreeClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton()
{
    emit statusUpdateMessage("Add button was clicked", 0);
}

void MainWindow::handleSubtractButton()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
        return;

    // Get selected ModelPart
    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart)
        return;

    // Open dialog
    optionDialog dialog(this);

    // Load existing values into the dialog
    dialog.setName(selectedPart->data(0).toString());
    dialog.setColourR(selectedPart->getColourR());
    dialog.setColourG(selectedPart->getColourG());
    dialog.setColourB(selectedPart->getColourB());
    dialog.setPartVisible(selectedPart->Visible());

    // Run dialog
    if (dialog.exec() == QDialog::Accepted) {

        // Update ModelPart with new values
        selectedPart->setName(dialog.getName());
        selectedPart->setColour(dialog.getColourR(),
            dialog.getColourG(),
            dialog.getColourB());
        selectedPart->setVisible(dialog.getPartVisible());

        emit statusUpdateMessage("Item updated", 0);

        // -----------------------------
        // Notify the model/view that the data has changed
        // -----------------------------
        QVector<int> roles; // empty, or could use Qt::DisplayRole
        roles << Qt::DisplayRole;

        // Tell the model that column 0 (name) changed
        this->partList->dataChanged(index, index, roles);

        // Refresh the view just in case
        ui->treeView->viewport()->update();
    }
    else {
        emit statusUpdateMessage("Dialog rejected", 0);
    }
}

void MainWindow::handleTreeClicked(const QModelIndex& index)
{
    /* Get a pointer to the item from the index */
    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
        return;

    /* Retrieve the name string from column 0 */
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(
        QString("The selected item is: ") + text,
        0
    );
}
void MainWindow::on_actionOpen_File_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid())
    {
        emit statusUpdateMessage("No item selected", 0);
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
    );

    if (fileName.isEmpty())
        return;  // user cancelled

    // Extract just the file name (no path)
    QFileInfo fileInfo(fileName);
    QString shortName = fileInfo.fileName();

    // Get selected ModelPart
    ModelPart* selectedPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart)
        return;

    // Set name in model
    selectedPart->setName(shortName);

    // Tell view the data changed
    QVector<int> roles;
    roles << Qt::DisplayRole;

    QModelIndex nameIndex = index.sibling(index.row(), 0);
    partList->dataChanged(nameIndex, nameIndex, roles);

    emit statusUpdateMessage("File loaded and name updated", 0);
}
void MainWindow::on_actionItem_Options_triggered()
{
    emit statusUpdateMessage("Context menu action triggered", 0);
    handleSubtractButton();
}
