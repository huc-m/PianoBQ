#include "myqgraphicsview.h"
#include "mainwindow.h"

#include "midi/midi_with_fluidsynth.h"

myQGraphicsView::myQGraphicsView( QWidget *parent ) : QGraphicsView( parent )
{

}

void myQGraphicsView::mouseReleaseEvent( QMouseEvent *event ) {

    if( mainwindow->tunePlayAction->text() == "Stop" ) fluid_play( false );
    mainwindow->tuneToBegin();

    QGraphicsView::mouseReleaseEvent( event );
}