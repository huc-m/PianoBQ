#ifndef MYEVENT_H
#define MYEVENT_H

#include <QEvent>

class NewPos : public QEvent
{
public:
    NewPos(QEvent::Type type) : QEvent(type) {}
    int cur_pos;
};

#endif // MYEVENT_H
