#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QGraphicsView>

class myQGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    myQGraphicsView(QWidget *parent = NULL );

protected:
    void mouseReleaseEvent( QMouseEvent *event ) override;
};

#endif // MYQGRAPHICSVIEW_H
