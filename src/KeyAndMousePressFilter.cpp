#include "KeyAndMousePressFilter.h"
#include <QEvent>

KeyAndMousePressFilter::KeyAndMousePressFilter()
{

}

bool KeyAndMousePressFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress ||
            event->type() == QEvent::MouseButtonPress ||
            event->type() == QEvent::MouseButtonDblClick ||
            event->type() == QEvent::MouseButtonRelease ||
            event->type() == QEvent::MouseMove ||
            event->type() == QEvent::MouseTrackingChange
            ) {
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
