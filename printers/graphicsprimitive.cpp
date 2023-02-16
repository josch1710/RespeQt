#include "logdisplaydialog.h"
#include "common/atariprinter.h"
#include "graphicsprimitive.h"

#include <QTransform>
#include <QGraphicsObject>

namespace Printers
{
    void _GraphicsPrimitive::executeQueue(QGraphicsScene *scene)
    {
        if (scene == nullptr)
            return;
        connect(scene, &QGraphicsScene::changed, this, &_GraphicsPrimitive::changed);
        for(auto item: mGraphicsItems)
        {
            scene->addItem(item);
        }
        // Now the items are owned by scene
    }

    void _GraphicsPrimitive::addItem(QGraphicsItem *item)
    {
        item->setTransform(mTransform);
        mGraphicsItems.push_back(item);
    }

    void _GraphicsPrimitive::clearScene()
    {
        for(auto item: mGraphicsItems)
        {
            // Only delete if item is not owned by a scene
            if (item->scene() == nullptr)
                delete item;
        }
        mGraphicsItems.clear();
    }

    void _GraphicsPrimitive::setTransform(QTransform transform)
    {
        mTransform = transform;
    }
}

