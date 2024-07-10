#include "myqgraphicsview.h"
#include "mainwindow.h"

#include "midi/midi_with_fluidsynth.h"

myQGraphicsView::myQGraphicsView( QWidget *parent ) : QGraphicsView( parent )
{

}

void myQGraphicsView::mouseReleaseEvent( QMouseEvent *event ) {

    fluid_play( false );
    static_cast<MainWindow*>(this->parent()->parent())->tuneToBegin();
    QGraphicsView::mouseReleaseEvent( event );
}