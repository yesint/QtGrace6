#ifndef KEYPRESSANDMOUSEPRESSEATER_H
#define KEYPRESSANDMOUSEPRESSEATER_H

#include <QObject>

class KeyAndMousePressFilter : public QObject
{
    Q_OBJECT

public:
    KeyAndMousePressFilter();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};


#endif // KEYPRESSANDMOUSEPRESSEATER_H
