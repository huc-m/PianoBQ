#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "midi/globals.h"

#include <QPainter>
#include <QGraphicsPathItem>

char note_name[2] = {'\0'};
static int pointerXold;

void MainWindow::DrawTuple(int pos_screen, int pos_tune){
    if( showPartOnly )
        if(pos_tune < cur_start || pos_tune >= cur_finish) return;

    int hand, hand_d, note;
    int back_note, back_note_offset;
    int old_note;
    int i, ledjer;
    int margin;
    int interval;

    for( hand = LE_H, hand_d = LE_D; hand <= RI_H; ++hand, ++hand_d) {
         back_note = back_note_offset = old_note = 0;
        for( i = 0; i < tuple_nums[pos_tune][hand_d]; ++i) {
            note = tune_draw[pos_tune].hand[hand].note[i];
//compute if the note is back
            if( old_note > 0 )
                if( back_note_offset > 0) {back_note = back_note_offset = 0; }
                else
                    if( note - old_note < 4){
                        if( ( interval = note_ofset[ note % 12 ] - note_ofset[ old_note % 12 ] ) < 0 ) interval =+ 7;
                        if ( interval < 2) {
                            back_note = other_note_offset; back_note_offset = 0.5 * staff_font_z;
                        }
                    }
//compute ledjers
            if( note < staff_low[hand]) {
                margin = staff_low[hand];
                if( i + 1 < tuple_nums[pos_tune][hand_d] )
                    if( tune_draw[pos_tune].hand[hand].note[i + 1] < staff_low[hand] ) margin = tune_draw[pos_tune].hand[hand].note[i + 1];
//margin in screen coordinates
                margin = note_zero[hand] - margin / 12 * staff_base_h7 - note_ofset[margin % 12] * staff_base_h2;
//position of the first ledjer
                ledjer = note_zero[hand] - note / 12 * staff_base_h7 - note_ofset[note % 12] * staff_base_h2 - ledjer0[note % 24] * staff_base_h2;
                for( note_name[0] = ledjer_symbol; ledjer > margin; ledjer -= staff_base_h) paint->drawText(pos_screen, ledjer, note_name);
            } else  if( note > staff_high[hand] ) {
                margin = staff_high[hand];
                if( i + 1 < tuple_nums[pos_tune][hand_d] )
                    if( tune_draw[pos_tune].hand[hand].note[i+1] > staff_low[hand] ) margin = tune_draw[pos_tune].hand[hand].note[i + 1];
//margin in screen coordinates
                margin = note_zero[hand] - margin / 12 * staff_base_h7 - note_ofset[margin % 12] * staff_base_h2;
//position of the first ledjer
                ledjer = note_zero[hand] - note / 12 * staff_base_h7 - note_ofset[note % 12] * staff_base_h2 + ledjer0[note % 24] * staff_base_h2;
                for( note_name[0] = ledjer_symbol; ledjer < margin; ledjer += staff_base_h) paint->drawText(pos_screen, ledjer, note_name);
             }
            note_name[0] = note % 12 + noteC_symbol + ((note < staff_low[hand] || note > staff_high[hand]) ? ledjer_offset[note % 24] : 0) + back_note;
            paint->drawText(pos_screen + back_note_offset, note_zero[hand] - note / 12 * staff_base_h7 - note_ofset[note % 12] * staff_base_h2, note_name);
            old_note = note;
        }
    }
}

void MainWindow::paintEvent( [[maybe_unused]] QPaintEvent *event) {
    int i;
    numVisibleNotes = (width()-staff_pading_w) / staff_base_w;
    int pointerX = (cur_position % numVisibleNotes) * staff_base_w + staff_pading_w;
    int pointerY0 = staff_pading_h / 2;
    int pointerY1 = staff_step +  1.5  * staff_pading_h + 4 * staff_base_h ;
    int cbegin = cur_position / numVisibleNotes * numVisibleNotes;
    int cend = cbegin + numVisibleNotes;
    if(cend > tune_length) cend = tune_length;

    if(cbegin != begin){
        if(ui->graphicsView->size() != staff_area_size){
            delete paint;
            delete staffPixmap;
            staff_area_size = ui->graphicsView->size();
            staffPixmap = new QPixmap(staff_area_size);
            ui->graphicsView->setContentsMargins( 0, 0, 0, 0 );
            ui->graphicsView->setSceneRect( staffPixmap->rect());
            paint = new QPainter(staffPixmap);
            paint->setFont(QFont("PianoBQ",staff_font_z));
        }
        begin = cbegin;
        pointerXold = -1;
        staffPixmap->fill(Qt::white);
        paint->setPen(QPen(Qt::black, staff_line_w));
        for(i=0; i<5; ++i){
            paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h, width() - staff_pading_w,  staff_pading_h + i * staff_base_h);
            paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h + staff_step, width() - staff_pading_w,  staff_pading_h + i * staff_base_h + staff_step);
        }
        for(i = cbegin; i < cend; ++i)
            DrawTuple(staff_pading_w + (i - cbegin) * staff_base_w, i) ;
    }
    paint->setCompositionMode( QPainter::CompositionMode_Xor);
    paint->setPen(QPen(Qt::white, staff_line_w));
    paint->drawLine(pointerXold, pointerY0, pointerXold, pointerY1);
    paint->drawLine(pointerX, pointerY0, pointerX, pointerY1);
    staffPixmapItem->setPixmap(*staffPixmap);
    pointerXold = pointerX;
}