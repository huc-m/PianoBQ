#include "myqgraphicsview.h"
#include "mainwindow.h"

myQGraphicsView::myQGraphicsView( QWidget *parent ) : QGraphicsView( parent )
{

}

void myQGraphicsView::mouseReleaseEvent( QMouseEvent *event ) {

    static_cast<MainWindow*>(this->parent()->parent())->tuneToBegin();
    QGraphicsView::mouseReleaseEvent( event );
}