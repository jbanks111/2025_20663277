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
#include <QVTKOpenGLNativeWidget.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkCamera.h>
#include <vtkNew.h>

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
    /* This needs adding to MainWindow constructor */

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->vtkWidget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer (this will change later to display a CAD model) */
    /* Will just copy and paste cylinder example from before */

    // This creates a polygonal cylinder model with eight circumferential facets
    // (i.e., in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry into the graphics
    // library. It may also do color mapping, if scalars or other attributes are defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(45.0);

    renderer->AddActor(cylinderActor);

    /* Reset Camera (probably needs to go in its own function that is called whenever
model is changed) */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
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
        tr("STL Files (*.stl)")
    );

    if (fileName.isEmpty())
        return;

    // Extract filename only (no path)
    QFileInfo fileInfo(fileName);
    QString shortName = fileInfo.fileName();

    // Get selected ModelPart (parent item)
    ModelPart* parentPart =
        static_cast<ModelPart*>(index.internalPointer());

    if (!parentPart)
        return;

    // Create new ModelPart item
    ModelPart* newPart = new ModelPart({ shortName, "true" });

    // Add as child of selected item
    parentPart->appendChild(newPart);

    // Ask the part to load the STL file
    newPart->loadSTL(fileName);

    // Notify the model/view that the tree changed
    partList->layoutChanged();

    emit statusUpdateMessage("STL loaded and item added", 0);
    
    newPart->loadSTL(fileName);

    updateRender();
}
void MainWindow::on_actionItem_Options_triggered()
{
    emit statusUpdateMessage("Context menu action triggered", 0);
    handleSubtractButton();
}
void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();

    updateRenderFromTree(partList->index(0, 0, QModelIndex()));

    renderWindow->Render();
}
void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid())
    {
        ModelPart* selectedPart =
            static_cast<ModelPart*>(index.internalPointer());

        if (selectedPart)
        {
            vtkSmartPointer<vtkActor> actor = selectedPart->getActor();

            if (actor)
                renderer->AddActor(actor);
        }
    }

    if (!partList->hasChildren(index) ||
        (index.flags() & Qt::ItemNeverHasChildren))
    {
        return;
    }

    int rows = partList->rowCount(index);

    for (int i = 0; i < rows; i++)
    {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}