#include <fluidsynth.h>

#include "midi/constants.h"
#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include "configuration/mainconfig.h"

static fluid_settings_t* fluid_settings;
static fluid_synth_t* fluid_synth;
static fluid_midi_driver_t* read_keyboard_driver;
static fluid_audio_driver_t* fluid_audio_driver;

static int synth_channel_out = 0;

int cur_position;
int cur_start;
int cur_finish;
static tune_def cur_tuple;
static int cur_numOn;
static tune_def cur_notes_wrong;
static int cur_num_wrong;

static MainWindow *mainwindow;
static int hand;

static int read_keyboard(void *data, fluid_midi_event_t *event){

    fluid_synth_handle_midi_event(fluid_synth, event);

    int note_key = fluid_midi_event_get_key(event);
    int i, k;

    switch (fluid_midi_event_get_type(event)) {
        case MIDI_NOTE_ON:
// check if this is correct note
            for( i=0; i < tuple_nums[cur_position][NEW_N]; ++i)
                if( note_key == tune_newNotes[cur_position].noteOn[i]) {
                    cur_tuple.noteOn[i] = note_key;
                    if( ++i > cur_numOn) cur_numOn = i;
check:
                    if( cur_tuple.all == tune_newNotes[cur_position].all && cur_num_wrong == 0) {
                        if(++cur_position >= cur_finish) {
//repeat from beginning
                            reset_keyboard_fluid( cur_start );
                        } else {
//reset noteOn
                            cur_numOn = 0;
                            cur_tuple.all = 0;
                        }
// update graph
                        mainwindow->cur_pos = cur_position;
                        mainwindow->update();
                    }
                    return FLUID_OK;
                }
// check if this is admissible note
            for( k = LE_H; k<= RI_H; ++k )
                for( i = 0; i < tuple_nums[cur_position][k]; ++i)
                    if( note_key == tune_data[cur_position].hand[k].note[i] ) return FLUID_OK;
// note is wrong
            fluid_synth_noteoff(fluid_synth, synth_channel_out, note_key);
            if( cur_num_wrong < N_MAX - 1 )
                cur_notes_wrong.noteOn[cur_num_wrong++] = note_key;
            return FLUID_OK;

        case MIDI_NOTE_OFF:
// remove wrong note
            for( i=0; i < cur_num_wrong; ++i)
                if( note_key == cur_notes_wrong.noteOn[i]) {
                    cur_notes_wrong.noteOn[i] = cur_notes_wrong.noteOn[--cur_num_wrong];
                    goto check;
                }
//remove right note
            for( i = 0; i < cur_numOn; ++i)
                if( note_key == cur_tuple.noteOn[i]) {
                    cur_tuple.noteOn[i] = 0;
                    if( i + 1 == cur_numOn) --cur_numOn;
                    return FLUID_OK;
                }
            return FLUID_OK;
    }
    return FLUID_OK;
}


static int read_keyboard_OneHand(void *data, fluid_midi_event_t *event){
// LE_D, RI_D

    fluid_synth_handle_midi_event(fluid_synth, event);

    int note_key = fluid_midi_event_get_key(event);
    int i;

    switch (fluid_midi_event_get_type(event)) {
        case MIDI_NOTE_ON:
// check if this is correct note
            for( i=0; i < tuple_nums[cur_position][hand + HtoD]; ++i)
                if( note_key == tune_draw[cur_position].hand[hand].note[i]) {
                    cur_tuple.noteOn[i] = note_key;
                    if( ++i > cur_numOn) cur_numOn = i;
check:
                    if( cur_tuple.all == tune_draw[cur_position].hand[hand].hand && cur_num_wrong == 0) {
                        if( ++cur_position >= cur_finish ) {
//repeat from beginning
                            reset_keyboard_fluid( cur_start );
                        } else {
//reset noteOn
                            while ( tuple_nums[cur_position][hand + HtoD] == 0)
                                if( cur_position < cur_finish ) ++cur_position;
                                else break;
                            cur_numOn = 0;
                            cur_tuple.all = 0;
                        }
// update graph
                        mainwindow->cur_pos = cur_position;
                        mainwindow->update();
                    }
                    return FLUID_OK;
                }
// check if this is admissible note
                for( i = 0; i < tuple_nums[cur_position][hand]; ++i)
                    if( note_key == tune_data[cur_position].hand[hand].note[i] ) return FLUID_OK;
// note is wrong
            fluid_synth_noteoff(fluid_synth, synth_channel_out, note_key);
            if( cur_num_wrong < N_MAX - 1 )
                cur_notes_wrong.noteOn[cur_num_wrong++] = note_key;
            return FLUID_OK;

        case MIDI_NOTE_OFF:
// remove wrong note
            for( i=0; i < cur_num_wrong; ++i)
                if( note_key == cur_notes_wrong.noteOn[i]) {
                    cur_notes_wrong.noteOn[i] = cur_notes_wrong.noteOn[--cur_num_wrong];
                    goto check;
                }
//remove right note
            for( i = 0; i < cur_numOn; ++i)
                if( note_key == cur_tuple.noteOn[i]) {
                    cur_tuple.noteOn[i] = 0;
                    if( i + 1 == cur_numOn) --cur_numOn;
                    return FLUID_OK;
                }
            return FLUID_OK;
    }
    return FLUID_OK;
}



void read_midi_keyboard_with_libfluidsynth(MainWindow* mainWin){
    mainwindow = mainWin;

    reset_keyboard_fluid( cur_start );

    fluid_settings = new_fluid_settings();
    FluidSettingsKeyboard( fluid_settings );

    fluid_synth = new_fluid_synth(fluid_settings);
    fluid_synth_sfload(fluid_synth, mainWin->sf2FileName.toStdString().c_str() ,1);
    fluid_audio_driver = new_fluid_audio_driver(fluid_settings, fluid_synth);
    read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard, NULL);
 }

void delete_keyboard_fluid(){
    delete_fluid_midi_driver(read_keyboard_driver);
    delete_fluid_audio_driver(fluid_audio_driver);
    delete_fluid_synth(fluid_synth);
    delete_fluid_settings(fluid_settings);
}

void reset_keyboard_fluid(int from_pos) {
    if( from_pos < 0 ) {cur_start = 0; cur_finish = tune_length;  from_pos = 0; }
    cur_position = from_pos;
    cur_num_wrong = 0;
    cur_notes_wrong.all = 0;
    cur_tuple.all = 0; cur_numOn = 0;
 }

void set_hand( int key) {
    delete_fluid_midi_driver(read_keyboard_driver);
    switch ( key ) {
        case LE_H:
            hand = LE_H;
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard_OneHand, NULL);
            mainwindow->handLeftOnlyAction->setChecked( true );
            if( tuple_nums[cur_position][LE_D] == 0 ){
                while ( tuple_nums[cur_position][LE_D] == 0 )
                    if( cur_position < cur_finish ) ++cur_position;
                    else break;
                mainwindow->cur_pos = cur_position;
                mainwindow->update();
            }
        break;
        case RI_H:
            hand = RI_H;
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard_OneHand, NULL);
            mainwindow->handRightOnlyAction->setChecked( true );
            if( tuple_nums[cur_position][RI_D] == 0 ){
                while ( tuple_nums[cur_position][RI_D] == 0 )
                    if( cur_position < cur_finish ) ++cur_position;
                    else break;
                mainwindow->cur_pos = cur_position;
                mainwindow->update();
            }
        break;
        case ALL_H:
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard, NULL);
            mainwindow->handAllHandsAction->setChecked( true );
        break;
        case NO_H:
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, fluid_synth_handle_midi_event, fluid_synth);
            mainwindow->handNoHandsAction->setChecked( true );
        break;
    }
}