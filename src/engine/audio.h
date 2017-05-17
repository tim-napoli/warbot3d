/* audio.h
 *  Load and play sounds in a 3d environnement.
 */

#ifndef _audio_h_
#define _audio_h_

#ifndef NOSOUND
#include <AL/al.h>
#include <AL/alc.h>
#else
    typedef int ALuint;
#endif

// Initialize the audio module. Must be called before any use of other functions.
// Return 0 on error.
int audio_init ();

// Close the audio module.
void audio_close ();


// Load a sound from a file.
// Return 0 on error.
ALuint audio_load_sound (const char* path);

// Play a sound from a source (see below).
void audio_play_sound (ALuint sound, ALuint source, float gain);

// Delete a sound from memory.
void audio_delete_sound (ALuint sound);


// Create a new audio source. A source is a point (not only but...) in the space from where a sound is emitted.
ALuint audio_create_source (float ref_dist);

// Update the position and the direction of a source.
void audio_source_update (ALuint source, float x, float y, float z, float dx, float dy, float dz);

// Delete a source from memory.
void audio_delete_source (ALuint source);

// Update a listener (like gluLookAt).
void audio_update_listener (float x, float y, float z, float dx, float dy, float dz, float ux, float uy, float uz);

#endif

