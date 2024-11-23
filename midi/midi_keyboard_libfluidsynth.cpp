#include <fluidsynth.h>

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

#include "configuration/tuneconfig.h"
#include "mainwindow.h"

#ifdef QT_DEBUG
#include <QDebug>
#endif

static fluid_settings_t* fluid_settings;
static fluid_synth_t* fluid_synth;
static fluid_midi_driver_t* read_keyboard_driver;
static fluid_audio_driver_t* fluid_audio_driver;
static fluid_player_t *player = NULL;

static tune_def cur_tuple;
static tune_def cur_notes_wrong;
static int cur_num_wrong;

static int next_hand_pos;

int hand;

static void soundStartHandPos( int pos ){
// LE_D, RI_D
    next_hand_pos = pos;
    while ( tuple_nums[next_hand_pos][hand] == 0)
        if( next_hand_pos < cur_finish ) ++next_hand_pos;
        else return;
}

static int read_midi_file_one_track(void *, fluid_midi_event_t *event) {
// LE_D, RI_D
    if( fluid_midi_event_get_type( event ) == MIDI_NOTE_ON ) {
        if ( fluid_midi_event_get_velocity(event) > 0 ) {
            if(  midiTicks[cur_position] < fluid_player_get_current_tick( player ))
                if( cur_position < next_hand_pos ) {
                    ++cur_position;
                    mainwindow->update();
                }
        } else
            if( fluid_player_get_current_tick( player ) > midiTicks[next_hand_pos +1] )
            { fluid_player_stop( player ); fluid_player_join( player ); }

        if(!(( hand == LE_D && fluid_midi_event_get_channel( event ) == left_hand_channel ) ||
            ( hand == RI_D && fluid_midi_event_get_channel( event ) == right_hand_channel )))
            fluid_synth_handle_midi_event( fluid_synth, event );

    } else  fluid_synth_handle_midi_event( fluid_synth, event );

    return FLUID_OK;
}

static int read_keyboard_with_sound( void *, fluid_midi_event_t *event){
// LE_D, RI_D
    fluid_synth_handle_midi_event(fluid_synth, event);

    int note_key = fluid_midi_event_get_key(event);
    int i;

    switch (fluid_midi_event_get_type(event)) {
        case MIDI_NOTE_ON:
// check if this is correct note
            for( i=0; i < tuple_nums[cur_position][hand]; ++i)
                if( note_key == tune_draw[cur_position].hand[hand - HtoD].note[i]) {
                    cur_tuple.noteOn[i] = note_key;
check:
                    if( cur_tuple.all == tune_draw[cur_position].hand[hand - HtoD].hand && cur_num_wrong == 0) {
                        cur_tuple.all = 0;
                        if( cur_position >= cur_finish -1 ){
                            fluid_player_stop( player );
                            cur_position = cur_start;
                            soundStartHandPos( cur_position );
                            mainwindow->update();
                            fluid_player_join( player );
                            fluid_player_seek( player,  midiTicks[cur_position] );
                        } else soundStartHandPos( cur_position + 1 );
                        if (fluid_player_get_status( player ) != FLUID_PLAYER_PLAYING){
                            fluid_player_seek( player, midiTicks[cur_position]);
                            fluid_player_play( player );
                        }
                    }
                    return FLUID_OK;
                }
// check if this is admissible note
                for( i = 0; i < tuple_nums[cur_position][hand]; ++i)
                    if( note_key == tune_data[cur_position].hand[hand].note[i] ) return FLUID_OK;
// note is wrong
            fluid_synth_noteoff(fluid_synth, SYNTH_CHANNEL_OUT, note_key);
            if( cur_num_wrong < N_MAX - 1 )
                cur_notes_wrong.noteOn[cur_num_wrong++] = note_key;
            return FLUID_OK;

        case MIDI_NOTE_OFF:
// remove wrong noteplay_midi_tick
            for( i=0; i < cur_num_wrong; ++i)
                if( note_key == cur_notes_wrong.noteOn[i]) {
                    cur_notes_wrong.noteOn[i] = cur_notes_wrong.noteOn[--cur_num_wrong];
                    goto check;
                }
//remove right note
            for( i = 0; i < tuple_nums[cur_position][NEW_N]; ++i)
                if( note_key == cur_tuple.noteOn[i]) {
                    cur_tuple.noteOn[i] = 0;
                    return FLUID_OK;
                }
            return FLUID_OK;
    }
    return FLUID_OK;
}

int play_update_graphics ( void *, fluid_midi_event_t *event ){
    fluid_synth_handle_midi_event( fluid_synth, event );
    if( fluid_midi_event_get_type( event ) == MIDI_NOTE_ON )
        if ( fluid_midi_event_get_velocity(event) > 0 ) {
            if(  midiTicks[cur_position] < fluid_player_get_current_tick( player ))
                if( cur_position < cur_finish ) {
                    ++cur_position;
                    mainwindow->update();
                } else fluid_play( false, false );
        } else if( cur_position >= cur_finish ) fluid_play( false, false );
    return FLUID_OK;
}

static int read_keyboard(void *, fluid_midi_event_t *event){

    fluid_synth_handle_midi_event(fluid_synth, event);

    int note_key = fluid_midi_event_get_key(event);
    int i, k;

    switch (fluid_midi_event_get_type(event)) {
        case MIDI_NOTE_ON:
// check if this is correct note
            for( i=0; i < tuple_nums[cur_position][NEW_N]; ++i)
                if( note_key == tune_newNotes[cur_position].noteOn[i]) {
                    cur_tuple.noteOn[i] = note_key;
check:
                    if( cur_tuple.all == tune_newNotes[cur_position].all && cur_num_wrong == 0) {
                        if(++cur_position >= cur_finish) {
//repeat from beginning
                            reset_keyboard_fluid( cur_start );
                        } else {
//reset noteOn
                            cur_tuple.all = 0;
                        }
// update graph
                        mainwindow->update();
                    }
                    return FLUID_OK;
                }
// check if this is admissible note
            for( k = LE_H; k<= RI_H; ++k )
                for( i = 0; i < tuple_nums[cur_position][k]; ++i)
                    if( note_key == tune_data[cur_position].hand[k].note[i] ) return FLUID_OK;
// note is wrong
            fluid_synth_all_notes_off( fluid_synth, SYNTH_CHANNEL_OUT );
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
            for( i = 0; i < tuple_nums[cur_position][NEW_N]; ++i)
                if( note_key == cur_tuple.noteOn[i]) {
                    cur_tuple.noteOn[i] = 0;
                    return FLUID_OK;
                }
            return FLUID_OK;
    }
    return FLUID_OK;
}

static int read_keyboard_OneHand(void *, fluid_midi_event_t *event){
// LE_H, RI_H

    fluid_synth_handle_midi_event(fluid_synth, event);

    int note_key = fluid_midi_event_get_key(event);
    int i;

    switch (fluid_midi_event_get_type(event)) {
        case MIDI_NOTE_ON:
// check if this is correct note
            for( i=0; i < tuple_nums[cur_position][hand + HtoD]; ++i)
                if( note_key == tune_draw[cur_position].hand[hand].note[i]) {
                    cur_tuple.noteOn[i] = note_key;
check:
                    if( cur_tuple.all == tune_draw[cur_position].hand[hand].hand && cur_num_wrong == 0) {
                        ++cur_position;
                        oneHandStartPos();
                        if( cur_position >= cur_finish ) {
//repeat from beginning
                            reset_keyboard_fluid( cur_start );
                            oneHandStartPos();
                        } else {
//reset noteOn
                            cur_tuple.all = 0;
                        }
// update graph
                        mainwindow->update();
                    }
                    return FLUID_OK;
                }
// check if this is admissible note
                for( i = 0; i < tuple_nums[cur_position][hand]; ++i)
                    if( note_key == tune_data[cur_position].hand[hand].note[i] ) return FLUID_OK;
// note is wrong
            fluid_synth_noteoff(fluid_synth, SYNTH_CHANNEL_OUT, note_key);
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
            for( i = 0; i < tuple_nums[cur_position][NEW_N]; ++i)
                if( note_key == cur_tuple.noteOn[i]) {
                    cur_tuple.noteOn[i] = 0;
                    return FLUID_OK;
                }
            return FLUID_OK;
    }
    return FLUID_OK;
}

void delete_keyboard_fluid(){
    if( player != NULL ) {
        fluid_player_stop ( player );
        fluid_player_join( player );
        delete_fluid_player( player );
    }
    delete_fluid_midi_driver(read_keyboard_driver);
    delete_fluid_audio_driver(fluid_audio_driver);
    delete_fluid_synth(fluid_synth);
    delete_fluid_settings(fluid_settings);
}

void set_hand( int key) {
    delete_fluid_midi_driver(read_keyboard_driver);
    switch ( hand = key ) {
        case ALL_H:
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard, NULL);
        break;
        case NO_H:
            read_keyboard_driver = new_fluid_midi_driver(fluid_settings, fluid_synth_handle_midi_event, fluid_synth);
        break;
        case LE_H:
        case RI_H:
            read_keyboard_driver = new_fluid_midi_driver( fluid_settings, read_keyboard_OneHand, NULL );
            oneHandStartPos();
            if( cur_position >= cur_finish ) cur_position = cur_start;
            oneHandStartPos();
            mainwindow->update();
        break;
        case LE_D:
        case RI_D:
            soundStartHandPos( cur_position );
            if( player != NULL ) {
                fluid_player_stop ( player );
                fluid_player_join( player );
                delete_fluid_player( player );
            }
            player = new_fluid_player( fluid_synth );
            fluid_player_set_playback_callback( player, read_midi_file_one_track, NULL );

            fluid_player_add( player, getTuneFile().toStdString().c_str());
            fluid_player_seek( player, midiTicks[ cur_position ] );
            soundStartHandPos( cur_position ) ;
            read_keyboard_driver = new_fluid_midi_driver( fluid_settings, read_keyboard_with_sound, NULL );
            fluid_player_play( player );
        break;
    }
}

void fluid_play(bool play , bool del) {
    if ( play ){
        mainwindow->tunePlayAction->setText( "Stop");
        if ( player != NULL ) {
            fluid_player_stop( player );
            fluid_player_join( player );
            delete_fluid_player( player );
        }
        if( cur_position >= cur_finish ) reset_keyboard_fluid( cur_start );
        player = new_fluid_player( fluid_synth );
        fluid_player_set_playback_callback( player, play_update_graphics, NULL );
        fluid_player_add( player, getTuneFile().toStdString().c_str());
        fluid_player_seek( player, midiTicks[ cur_position ] );
        fluid_player_set_tempo( player, FLUID_PLAYER_TEMPO_INTERNAL, play_speed);
        fluid_player_play( player );
    } else {
        mainwindow->tunePlayAction->setText( "Play");
        if( player != NULL ){
            fluid_player_stop ( player );
            fluid_player_join( player );
            if( del ) {
                delete_fluid_player( player );
                player = NULL;
            }
        }
    }
}

void reset_keyboard_fluid( int cur_pos ) {
    if( cur_pos < 0 ) { cur_start = 0; cur_finish = tune_length;  cur_position = 0; }
    else cur_position = cur_pos;
    cur_num_wrong = 0;
    cur_notes_wrong.all = 0;
    cur_tuple.all = 0;
 }

void init_keyboard_libfluidsynth( QSettings *conf ){

    fluid_settings = new_fluid_settings();
    conf->beginGroup( "FLUIDSYNTH" );
        for( QString key : conf->allKeys() )
            switch ( fluid_settings_get_type( fluid_settings, key.toStdString().c_str()) ) {
                case FLUID_NUM_TYPE:
                    fluid_settings_setnum( fluid_settings, key.toStdString().c_str(), conf->value( key ).toDouble() );
                break;
                case FLUID_INT_TYPE:
                    fluid_settings_setint( fluid_settings, key.toStdString().c_str(), conf->value( key ).toInt() );
                break;
                case FLUID_STR_TYPE:
                    fluid_settings_setstr( fluid_settings, key.toStdString().c_str(), conf->value( key ).toString().toStdString().c_str() );
                break;
            }
    conf->endGroup();
    fluid_synth = new_fluid_synth( fluid_settings) ;
#ifdef QT_DEBUG
        fluid_synth_sfload(fluid_synth, "../piano.sf3" ,1);
#else
    fluid_synth_sfload(fluid_synth, conf->value("MIDI/sound_font").toString().toStdString().c_str() ,1);
#endif
    fluid_audio_driver = new_fluid_audio_driver(fluid_settings, fluid_synth);
    read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard, NULL);

    tune_length = 0;
    reset_keyboard_fluid( -1 );
 }

void oneHandStartPos(){
    while ( tuple_nums[cur_position][hand + HtoD] == 0)
        if( cur_position < cur_finish ) ++cur_position;
        else return;
}

void oneHandMoveNextMrg() {
    oneHandStartPos();
    if( cur_position >= cur_finish )
        while( tuple_nums[cur_position][hand + HtoD] == 0)
            if( cur_position > cur_start ) --cur_position;
            else return;
}

void oneHandMovePreviousMrg(){
    while( tuple_nums[cur_position][hand + HtoD] == 0)
        if( cur_position > cur_start ) --cur_position;
        else break;
    if( tuple_nums[cur_position][hand + HtoD] == 0 ) oneHandStartPos();
}