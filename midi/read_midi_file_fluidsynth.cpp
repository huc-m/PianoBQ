#include <fluidsynth.h>

#include "midi/globals.h"
#include "midi/midi_with_fluidsynth.h"

struct ticks {
    int tick;
    int old_tick;
};

int error;

int get_midi_file_event(void* tick, fluid_midi_event_t *event){

    int i, j, hand, hand_d;
    int channel, note;

    if ( error != NO_ERRORS ) return error;

    if( fluid_midi_event_get_type( event ) == MIDI_NOTE_ON ) {
        channel = fluid_midi_event_get_channel( event );
        note = fluid_midi_event_get_key( event );
        if( channel == left_hand_channel ) { hand = LE_H; hand_d = LE_D; }
        else if ( channel == right_hand_channel) { hand = RI_H; hand_d = RI_D; }
            else return FLUID_OK;

        if( ((ticks*)tick)->tick != ((ticks*)tick)->old_tick ) {
            ((ticks*)tick)->old_tick = ((ticks*)tick)->tick;
//copy tuple
            if( tune_length + 1 == TUNE_LENGTH_MAX) { error = MIDI_FILE_TUNE_IS_TOO_LONG; return FLUID_OK; }
            if( tune_newNotes[tune_length].all != 0) {
                ++tune_length;
                midiTicks[tune_length] = ((ticks*)tick)->tick;
                tune_data[tune_length].all = tune_data[tune_length - 1].all;
                tuple_nums[tune_length][LE_H] = tuple_nums[tune_length - 1][LE_H];
                tuple_nums[tune_length][RI_H] = tuple_nums[tune_length - 1][RI_H];
//reset new notes
                tune_newNotes[tune_length].all = 0;
                tuple_nums[tune_length][NEW_N] = 0;
//reset draw data
                tune_draw[tune_length].all = 0;
                tuple_nums[tune_length][LE_D] = tuple_nums[tune_length][RI_D] = 0;
            }
        }
        if( fluid_midi_event_get_velocity( event ) == 0 ) {
// remove note
            for( i = 0; i < tuple_nums[tune_length][hand] ; ++i)
                    if( tune_data[tune_length].hand[hand].note[i] == note) {
                        tune_data[tune_length].hand[hand].note[i] = tune_data[tune_length].hand[hand].note[tuple_nums[tune_length][hand] - 1];
                        tune_data[tune_length].hand[hand].note[--tuple_nums[tune_length][hand]] = 0;
                        return FLUID_OK;
                    }
        } else
// introduce note
            if( tuple_nums[tune_length][hand] == N_MAX ) error = MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE;
            else {
                tune_data[tune_length].hand[hand].note[tuple_nums[tune_length][hand]++] = note;
// add new note if not exists
                for( i = 0; i < tuple_nums[tune_length][NEW_N] && tune_newNotes[tune_length].noteOn[i] != note; ++i);
                if ( i == tuple_nums[tune_length][NEW_N])
                    tune_newNotes[tune_length].noteOn[tuple_nums[tune_length][NEW_N]++] = note;
//draw data with sort
                for( i = 0; i < tuple_nums[tune_length][hand_d]; ++i )
                    if( tune_draw[tune_length].hand[hand].note[i] > note ) {
                        for( j = tuple_nums[tune_length][hand_d] + 1; j > i; --j )
                            tune_draw[tune_length].hand[hand].note[j] = tune_draw[tune_length].hand[hand].note[j-1];
                        break;
                    }
                tune_draw[tune_length].hand[hand].note[i] = note; ++tuple_nums[tune_length][hand_d];
            }
    }
    return FLUID_OK;
}


int update_midi_tick(void* tick, int curr_tick){

    ((ticks*)tick)->tick = curr_tick;
    return FLUID_OK;
}

int read_midi_file(const char *midi_file_name){

    fluid_settings_t* fluid_settings;
    fluid_synth_t* fluid_synth;
    fluid_player_t* fluid_player;
    fluid_file_renderer_t* fluid_file_renderer;

    struct ticks ticks = {0, 0};
    tune_length = 0;
    cur_position = 0;
    tuple_nums[0][LE_H] = tuple_nums[0][RI_H] = tuple_nums[0][NEW_N] = 0;
    tune_data[0].all = tune_newNotes[0].all = 0;
    tuple_nums[0][LE_D] = tuple_nums[0][RI_D] = 0;
    tune_draw[0].all = 0;
    error = NO_ERRORS;
    midiTicks[0] = 0;

    fluid_settings = new_fluid_settings();
    fluid_settings_setstr(fluid_settings, "audio.file.name", NULL_FILE_NAME);
    fluid_settings_setstr(fluid_settings, "player.timing-source", "sample");
    fluid_settings_setint(fluid_settings, "synth.lock-memory", 0);

    fluid_settings_setint(fluid_settings, "synth.chorus.active", 0);
    fluid_settings_setint(fluid_settings, "synth.min-note-length", 0);
    fluid_settings_setint(fluid_settings, "synth.reverb.active", 0);
    fluid_settings_setnum(fluid_settings, "synth.sample-rate", 8000);
    fluid_settings_setint(fluid_settings, "audio.periods", 64);
    fluid_settings_setint(fluid_settings, "audio.period-size", 8192);
    fluid_settings_setstr(fluid_settings, "audio.file.type", "raw");

    fluid_synth = new_fluid_synth(fluid_settings);
    fluid_player = new_fluid_player(fluid_synth);

    fluid_player_set_playback_callback(fluid_player, get_midi_file_event,  &ticks);
    fluid_player_set_tick_callback(fluid_player, update_midi_tick, &ticks);
    if(fluid_player_add(fluid_player, midi_file_name) == FLUID_FAILED) {error = MIDI_FILE_ERROR_ON_READING; goto del_player;}
    fluid_file_renderer = new_fluid_file_renderer (fluid_synth);

    fluid_file_set_encoding_quality(fluid_file_renderer, 0.0);

    if(fluid_player_play(fluid_player) == FLUID_FAILED) {error = MIDI_FILE_ERROR_ON_READING; goto del_renderer;}
    while (fluid_player_get_status(fluid_player) == FLUID_PLAYER_PLAYING) {
        if ( error != NO_ERRORS) goto del_renderer;
        if (fluid_file_renderer_process_block(fluid_file_renderer) != FLUID_OK) break;
    }

                    fluid_player_join(fluid_player);
    del_renderer:   delete_fluid_file_renderer(fluid_file_renderer);
    del_player:     delete_fluid_player(fluid_player);
                    delete_fluid_synth(fluid_synth);
                    delete_fluid_settings(fluid_settings);

    reset_keyboard_fluid( -1 );
    set_hand( ALL_H );

    return tune_length < 1 ? MIDI_FILE_ERROR_ON_READING : error;
}
