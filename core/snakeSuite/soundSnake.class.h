#include <alsa/asoundlib.h>
#include <pulse/pulseaudio.h>
#include <pulse/operation.h>
#include <pulse/simple.h>
#include <pulse/def.h>

struct pulseAudioDevice{
        pa_simple *s;
        pa_sample_spec ss;
}; typedef struct pulseAudioDevice audio_device_t; 

class SoundSnake{
	private:
		audio_device_t current;
		string error = "";
	public:
		bool initSimpleSpeaker(string appName){
        		current.ss.format = PA_SAMPLE_S16NE;
        		current.ss.channels = 2;
        		current.ss.rate = 44100;
        		current.s = pa_simple_new(
				NULL,            	// Use the default server.
        			appName.c_str(),       // Our application's name.
        		        PA_STREAM_PLAYBACK,   	// Marks which device to read/write from/to
        		        NULL,                 	// Use the default device.
        		        "Music",              	// Description of our stream.
        		        &current.ss,            // Our sample format.
        		        NULL,                 	// Use default channel map
        		        NULL,                 	// Use default buffering attributes.
        		        NULL                  	// Ignore error code.
        		);
			return true;
		}

		void initSimpleMicrophone(string appName){
			current.ss.format = PA_SAMPLE_S16NE;
                        current.ss.channels = 2;
                        current.ss.rate = 44100;
                        current.s = pa_simple_new(
                                NULL,                   // Use the default server.
                                appName.c_str(),       	// Our application's name.
                                PA_STREAM_RECORD,     	// Marks which device to read/write from/to
                                NULL,                   // Use the default device.
                                "Music",                // Description of our stream.
                                &current.ss,            // Our sample format.
                                NULL,                   // Use default channel map
                                NULL,                   // Use default buffering attributes.
                                NULL                    // Ignore error code.
                        );
		}

		void freeDevice(void){
			if(current.s){
				pa_simple_free(current.s);
				current.s = NULL;
			}
		}

		bool playSimpleAudio(char *audioBuf, int bufSize){
			int e;
			pa_simple_write(current.s, audioBuf, bufSize, &e);
			if(e < 0){
				error = "Failed to write to audio descriptor ("+to_string(e)+")";
				return false;
			}
			return true;
		}

		string getError(void){
			return error;
		}
};
