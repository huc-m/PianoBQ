#include "globals.h"
#include <fluidsynth.h>
#include "midi_with_fluidsynth.h"

namespace {

struct ticks {
    int tick;
    int old_tick;
};

int error;

void remove_note(int note, int channel){

    int i;

    if(channel == left_hand_channel) {
        for( i = 0; i < Nmax ; ++i)
            if( tune_data[tune_length].hand[LE_H].note[i] == note) tune_data[tune_length].hand[LE_H].note[i] = 0;
    }
    if(channel == right_hand_channel) {
        for( i = 0; i < Nmax ; ++i)
            if( tune_data[tune_length].hand[RI_H].note[i] == note) tune_data[tune_length].hand[RI_H].note[i] = 0;
    }
   return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void introduce_note(int note, int channel){

    int i;

    if(channel == left_hand_channel) {
        for( i = 0; i < Nmax && tune_data[tune_length].hand[LE_H].note[i]; ++i);
        if( i < Nmax ) tune_data[tune_length].hand[LE_H].note[i] = note; else error = MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE;
    }
    if(channel == right_hand_channel) {
        for( i = 0; i < Nmax && tune_data[tune_length].hand[RI_H].note[i]; ++i);
        if( i < Nmax ) tune_data[tune_length].hand[RI_H].note[i] = note; else error = MIDI_FILE_TOO_MANY_NOTES_IN_TUPLE;
    }
    return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void copy_tuple(){

    if(tune_length == TUNE_LENGTH_MAX - 1) { error = MIDI_FILE_TUNE_IS_TOO_LONG; return; }

    int i, j, c, tmp;
    for( j = 0; c = 1, j < 2; ++j )
        while( c )
            for( i = 0, c = 0; i < Nmax -1; ++i )
                if( (tune_data[tune_length].hand[j].note[i] == 0 ? 128 : tune_data[tune_length].hand[j].note[i])
                    >
                    (tune_data[tune_length].hand[j].note[i + 1] == 0 ? 128 : tune_data[tune_length].hand[j].note[i + 1])
                    ){
                    c = 1;
                    tmp = tune_data[tune_length].hand[j].note[i];
                    tune_data[tune_length].hand[j].note[i] = tune_data[tune_length].hand[j].note[i + 1];
                    tune_data[tune_length].hand[j].note[i + 1] = tmp;
                }
    tune_length++;
    tune_data[tune_length].all = tune_data[tune_length-1].all;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

int get_midi_file_event(void* tick, fluid_midi_event_t *event){

    if(fluid_midi_event_get_type(event) == MIDI_NOTE_ON)
        if( ((ticks*)tick)->tick == ((ticks*)tick)->old_tick )
            if(fluid_midi_event_get_velocity(event)==0) remove_note(fluid_midi_event_get_key(event), fluid_midi_event_get_channel(event));
            else introduce_note(fluid_midi_event_get_key(event), fluid_midi_event_get_channel(event));
        else {
            copy_tuple();
            if(fluid_midi_event_get_velocity(event)==0) remove_note(fluid_midi_event_get_key(event), fluid_midi_event_get_channel(event));
            else introduce_note(fluid_midi_event_get_key(event), fluid_midi_event_get_channel(event));
            ((ticks*)tick)->old_tick = ((ticks*)tick)->tick;
        }
    return FLUID_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

int update_midi_tick(void* tick, int curr_tick){

    ((ticks*)tick)->tick = curr_tick;
    return FLUID_OK;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
} // namespace


int read_midi_file(char *midi_file_name){

    fluid_settings_t* fluid_settings;
    fluid_synth_t* fluid_synth;
    fluid_player_t* fluid_player;
    fluid_file_renderer_t* fluid_file_renderer;

    struct ticks ticks = {0, 0};
    tune_length = 0;
    tune_data[0].hand[LE_H].hand = tune_data[0].hand[RI_H].hand = 0;
    error = 0;

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

    while (fluid_player_get_status(fluid_player) == FLUID_PLAYER_PLAYING)
        if (fluid_file_renderer_process_block(fluid_file_renderer) != FLUID_OK) break;


                    fluid_player_join(fluid_player);
    del_renderer:   delete_fluid_file_renderer(fluid_file_renderer);
    del_player:     delete_fluid_player(fluid_player);
                    delete_fluid_synth(fluid_synth);
                    delete_fluid_settings(fluid_settings);

    return tune_length < 1 ? MIDI_FILE_ERROR_ON_READING : error;
}
