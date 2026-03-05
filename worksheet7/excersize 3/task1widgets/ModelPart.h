/**     @file ModelPart.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#ifndef VIEWER_MODELPART_H
#define VIEWER_MODELPART_H

#include <QString>
#include <QList>
#include <QVariant>

#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkMapper.h>


class ModelPart {
public:
    /** Constructor
     * @param data is a List (array) of strings for each property of this item
     * @param parent is the parent of this item
     */
    ModelPart(const QList<QVariant>& data, ModelPart* parent = nullptr);

    /** Destructor */
    ~ModelPart();

    /** Add a child to this item */
    void appendChild(ModelPart* item);

    /** Return child at position 'row' below this item */
    ModelPart* child(int row);

    /** Return number of children */
    int childCount() const;

    /** Get number of data items (columns) */
    int columnCount() const;

    /** Return the data item at a particular column */
    QVariant data(int column) const;

    /** Set column data */
    void set(int column, const QVariant& value);

    /** Get pointer to parent item */
    ModelPart* parentItem();

    /** Get row index of item relative to parent */
    int row() const;

    /** Set colour (0-255 RGB) */
    void setColour(const unsigned char R, const unsigned char G, const unsigned char B);

    unsigned char getColourR();
    unsigned char getColourG();
    unsigned char getColourB();

    /** Set name */
    void setName(const QString& name);
    QString getName() const;

    /** Set visibility */
    void setVisible(bool visible);
    bool Visible() const;

    /** Load STL file */
    void loadSTL(QString fileName);

    /** Return actor for GUI rendering */
    vtkSmartPointer<vtkActor> getActor();

    /** Return new actor for VR rendering */
    //vtkActor* getNewActor();

private:
    QList<ModelPart*>       m_childItems;
    QList<QVariant>         m_itemData;
    ModelPart* m_parentItem;

    /* Typical properties */
    bool            isVisible;
    unsigned char   colourB;
    unsigned char   colourG;
    unsigned char   colourR;
    QString         name;

    /* VTK properties used to load/render the model */
    vtkSmartPointer<vtkSTLReader>   reader;
    vtkSmartPointer<vtkMapper>      mapper;
    vtkSmartPointer<vtkActor>       actor;
    vtkColor3<unsigned char>        colour;
};

#endif