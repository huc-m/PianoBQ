#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QGraphicsPathItem>
#include <QApplication>

#include "midi/globals.h"
#include "myevents.h"


//#include<unistd.h> //??????????????????????????????????????????????????????

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

void MainWindow::DrawStaff(int pos){

    int i;
    int num = (width()-staff_pading_w) / staff_base_w;
    int pointerX = (pos % num) * staff_base_w + staff_pading_w - staff_base_w / 10;
    int cbegin = pos / num * num;
    int pointerY0 = staff_pading_h / 2;
    int pointerY1 = staff_step +  1.5  * staff_pading_h + 4 * staff_base_h ;
    int cend = cbegin + num;
    if(cend > tune_length) cend = tune_length;

    if(cbegin != begin){
        if(ui->graphicsView->size() != staff_area_size){
            staff_area_size = ui->graphicsView->size();
            delete paint;
            delete staffPixmap;
            staffPixmap = new QPixmap(staff_area_size);
            paint = new QPainter(staffPixmap);
            paint->setFont(QFont("PianoBQ",staff_font_z));
        }
        begin = cbegin;
        staffPixmap->fill(Qt::white);
        for(i=0; i<5; ++i){
            paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h, width() - staff_pading_w,  staff_pading_h + i * staff_base_h);
            paint->drawLine(staff_pading_w, staff_pading_h + i * staff_base_h + staff_step, width() - staff_pading_w,  staff_pading_h + i * staff_base_h + staff_step);
        }
        for(i = cbegin; i < cend; ++i)
            DrawTuple(staff_pading_w + (i - cbegin) * staff_base_w, i) ;
    }

    paint->setPen(QPen(Qt::white, staff_line_w));
    paint->drawLine(pointerX - staff_base_w, pointerY0, pointerX - staff_base_w, pointerY1);
    paint->setPen(QPen(Qt::black, staff_line_w));
    paint->drawLine(pointerX, pointerY0, pointerX, pointerY1);
    staffPixmapItem->setPixmap(*staffPixmap);
    QApplication::processEvents();

}

void MainWindow::customEvent(QEvent *event){
    DrawStaff(((NewPos*)event)->cur_pos);
}