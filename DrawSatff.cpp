#include "mainwindow.h"
#include <QPainter>
#include <QGraphicsPathItem>
#include <QApplication>

#include "midi/globals.h"


#include<unistd.h> //??????????????????????????????????????????????????????

#define F5S 78
#define E4F 64
#define A3S 58
#define G2S 44

void MainWindow::DrawTuple(int pos_screen, int pos_tune){

    int i, j, tmp;
    char note[2]="";
    int from, to;
    int oth_offset, oth_delta;

// right hand

    for (i = 0; i < Nmax && tune_data[pos_tune].hand[RI_H].note[i]; ++i){
        tmp = tune_data[pos_tune].hand[RI_H].note[i];
        if(!i){oth_offset = 0; oth_delta = 0;}
        else{
            if(std::abs(note_ofset[tmp % 12] - note_ofset[tune_data[pos_tune].hand[RI_H].note[i-1] % 12]) < 2)
                if(oth_offset == other_note_offset){
                    oth_offset = 0; oth_delta = 0;
                } else {
                    oth_offset = other_note_offset; oth_delta = 0.3 * staff_font_z;
            } else {
                oth_offset = 0; oth_delta = 0;
            }
        }
        note[0] = tmp % 12 + noteC_symbol + ((tmp < E4F || tmp > F5S) ? ledjer_offset[tmp % 24] : 0) + oth_offset;
        paint->drawText(pos_screen + oth_delta,
            note_zero_r - tmp / 12 * staff_base_h7 - note_ofset[tmp % 12] * staff_base_h2, note);
    }

    note[0] = ledjer_symbol;

    for (i = 0; i < Nmax - 1 && tune_data[pos_tune].hand[RI_H].note[i]; ++i){
        from = tune_data[pos_tune].hand[RI_H].note[i];
        to = tune_data[pos_tune].hand[RI_H].note[i+1];
        if( to == 0){
            to = (E4F + F5S) / 2;
            if (to < from) std::swap(to, from);
        }

        while (ledjer_offset[from % 24]) ++from;
        while (ledjer_offset[to % 24]) --to;
        for(j = from; j <= to; ++j){
            if( ledjer0[j % 24] && (j < E4F || j > F5S)){
                paint->drawText(pos_screen,
                note_zero_r - j / 12 * staff_base_h7 - note_ofset[j % 12] * staff_base_h2, note);
            }
        }
    }

// left hand

    for (i = 0; i < Nmax && tune_data[pos_tune].hand[LE_H].note[i]; ++i){
        tmp = tune_data[pos_tune].hand[LE_H].note[i];
        if(!i){oth_offset = 0; oth_delta = 0;}
        else{
            if(std::abs(note_ofset[tmp % 12] - note_ofset[tune_data[pos_tune].hand[LE_H].note[i-1] % 12]) < 2)
                if(oth_offset == other_note_offset){
                    oth_offset = 0; oth_delta = 0;
                } else {
                    oth_offset = other_note_offset; oth_delta = 0.3 * staff_font_z;
            } else {
                oth_offset = 0; oth_delta = 0;
            }
        }
        note[0] = tmp % 12 + noteC_symbol + ((tmp < G2S || tmp > A3S) ? ledjer_offset[tmp % 24] : 0) + oth_offset;
        paint->drawText(pos_screen + oth_delta,
            note_zero_l - tmp / 12 * staff_base_h7 - note_ofset[tmp % 12] * staff_base_h2, note);
    }

    note[0] = ledjer_symbol;

    for (i = 0; i < Nmax-1 && tune_data[pos_tune].hand[LE_H].note[i]; ++i){
        from = tune_data[pos_tune].hand[LE_H].note[i];
        to = tune_data[pos_tune].hand[LE_H].note[i+1];
        if( to == 0){
            to = (G2S + A3S) / 2;
                if (to < from) std::swap(to, from);
        }

        while (ledjer_offset[from % 24]) ++from;
        while (ledjer_offset[to % 24]) --to;
        for(j = from; j <= to; ++j){
            if( ledjer0[j % 24] && (j < G2S || j > A3S)){
                paint->drawText(pos_screen,
                    note_zero_l - j / 12 * staff_base_h7 - note_ofset[j % 12] * staff_base_h2, note);
            }
        }
    }
}

void MainWindow::DrawStaff(){

    int i, j;
    int num = (width()-staff_pading_w) / staff_base_w;
    staffPixmap->fill(Qt::white);

    for(i=0; i<5; ++i){
        paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h, width() - staff_pading_w,  staff_pading_h + i * staff_base_h);
        paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h + staff_step, width() - staff_pading_w,  staff_pading_h + i * staff_base_h + staff_step);
     }

    for (j = 0; j < tune_length ; j++){
        DrawTuple(staff_pading_w + (j % num) * staff_base_w, j) ;
        if(!(j % num) && j){
            staffPixmapItem->setPixmap(*staffPixmap);
            QApplication::processEvents();
            sleep(1);
            staffPixmap->fill(Qt::white);
            for(i=0; i<5; ++i){
                paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h, width() - staff_pading_w,  staff_pading_h + i * staff_base_h);
                paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h + staff_step, width() - staff_pading_w,  staff_pading_h + i * staff_base_h + staff_step);
             }

        }
    }

    staffPixmapItem->setPixmap(*staffPixmap);
    QApplication::processEvents();

}


