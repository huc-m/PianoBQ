#include <fluidsynth.h>
#include "constants.h"
#include "globals.h"
#include "midi/midi_with_fluidsynth.h"
//#include <unistd.h>

#include "myevents.h"
#include <QApplication>


static fluid_settings_t* fluid_settings;
static fluid_synth_t* fluid_synth;
static fluid_midi_driver_t* read_keyboard_driver;
static fluid_audio_driver_t* fluid_audio_driver;

static int cur_position;
static int cur_num_wrong;
static int synth_channel_out = 0;
static tune_def cur_tuple;


static int read_keyboard(void *data, fluid_midi_event_t *event){
    int i, tmp;
    int event_type = fluid_midi_event_get_type(event);
    int note_key = fluid_midi_event_get_key(event);
    int note_velocity = fluid_midi_event_get_velocity(event);

    if(event_type == MIDI_NOTE_ON || event_type == MIDI_NOTE_OFF){
        if (event_type == MIDI_NOTE_ON ){

            fluid_synth_noteon(fluid_synth, synth_channel_out, note_key, note_velocity);

            for(i = 0; (tmp = tune_data[cur_position].hand[LE_H].note[i]) && i < Nmax; ++i)
                if( note_key == tmp ) {
                    cur_tuple.hand[LE_H].note[i] = tmp; goto check;
                }
            for(i = 0; (tmp = tune_data[cur_position].hand[RI_H].note[i]) && i < Nmax; ++i)
                if( note_key == tmp ) {
                    cur_tuple.hand[RI_H].note[i] = tmp; goto check;
                }
            cur_num_wrong++;
            fluid_synth_noteoff(fluid_synth, synth_channel_out, note_key);
            return FLUID_OK;

        } else {

            for(i = 0; i < Nmax; ++i)
                if( note_key == cur_tuple.hand[LE_H].note[i] ) {
                    cur_tuple.hand[LE_H].note[i] = 0;
                    fluid_synth_noteoff(fluid_synth, synth_channel_out, note_key);
                    goto check;
                }
            for(i = 0; i < Nmax; ++i)
                if( note_key == cur_tuple.hand[RI_H].note[i] ) {
                    cur_tuple.hand[RI_H].note[i] = 0;
                    fluid_synth_noteoff(fluid_synth, synth_channel_out, note_key);
                    goto check;
                }
            cur_num_wrong--;

        }
        check:
        if(!cur_num_wrong && tune_data[cur_position].all == cur_tuple.all) {
            cur_position++; if(cur_position == tune_length) cur_position = 0;
            NewPos* myevent = new NewPos(QEvent::User);
            myevent->cur_pos = cur_position;
            QApplication::postEvent((MainWindow*)data, myevent);
        }

    } else {
        fluid_synth_handle_midi_event(fluid_synth, event);
    }
    return FLUID_OK;
}

void read_midi_keyboard_with_libfluidsynth(MainWindow* mainWin){

    cur_position = 0;
    cur_num_wrong = 0;
    cur_tuple.all = 0;

    fluid_settings = new_fluid_settings();
    fluid_settings_setint(fluid_settings, "midi.autoconnect", 1);
    fluid_settings_setstr(fluid_settings, "audio.driver", "alsa");
    fluid_settings_setstr(fluid_settings, "audio.alsa.device", "hw:0");
    fluid_settings_setint(fluid_settings, "synth.cpu-cores", 4);
    fluid_settings_setint(fluid_settings, "audio.periods", 2);
    fluid_settings_setint(fluid_settings, "audio.period-size", 128);
    fluid_settings_setnum(fluid_settings, "synth.gain", 1);

    fluid_synth = new_fluid_synth(fluid_settings);
    fluid_synth_sfload(fluid_synth, "/usr/share/sounds/sf2/FluidR3_GM.sf2",1);
    fluid_audio_driver = new_fluid_audio_driver(fluid_settings, fluid_synth);
    read_keyboard_driver = new_fluid_midi_driver(fluid_settings, read_keyboard, mainWin);
 }

void delete_fluid(){
    delete_fluid_midi_driver(read_keyboard_driver);
    delete_fluid_audio_driver(fluid_audio_driver);
    delete_fluid_synth(fluid_synth);
    delete_fluid_settings(fluid_settings);
}
