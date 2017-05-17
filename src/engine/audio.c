#include "engine/audio.h"

#ifndef NOSOUND
#include <sndfile.h>
#endif

#include <stdio.h>
#include <stdlib.h>

// Voir : http://loulou.developpez.com/tutoriels/openal/premiers-pas/
int audio_init () {
#ifndef NOSOUND
    ALCdevice* device = alcOpenDevice (NULL);
    if (!device) {
        fprintf (stderr, "audio_init error : Cannot open alc device.\n");
        return 0;
    }
    
    ALCcontext* context = alcCreateContext (device, NULL);
    if (!context) {
        fprintf (stderr, "audio_init error : Cannot open alc context.\n");
        return 0;
    }
    
    if (!alcMakeContextCurrent (context)) {
        fprintf (stderr, "audio_init error : Cannot make alc context as current context.\n");
        return 0;
    }
#endif
    return 1;
}

void audio_close () {
#ifndef NOSOUND
    ALCcontext* context = alcGetCurrentContext ();
    ALCdevice* device = alcGetContextsDevice (context);
    
    alcMakeContextCurrent (NULL);
    
    alcDestroyContext (context);
    alcCloseDevice (device);
#endif
}

ALuint audio_load_sound (const char* path) {
#ifndef NOSOUND
    // Ouverture du fichier audio avec libsndfile
    SF_INFO file_info;
    SNDFILE* file = sf_open (path, SFM_READ, &file_info);
    if (!file) {
        fprintf (stderr, "audio_load_sample error : Cannot open file %s\n", path);
        return 0;
    }
    
    // Lecture du nombre d'échantillons et du taux d'échantillonnage (nombre d'échantillons à lire par seconde)
    ALsizei n_samples   = (ALsizei) (file_info.channels * file_info.frames);
    ALsizei sample_rate = (ALsizei) file_info.samplerate;
    ALshort* samples = malloc (n_samples * sizeof (ALshort));
    if (sf_read_short (file, samples, n_samples) < n_samples) {
        sf_close (file);
        free (samples);
        fprintf (stderr, "audio_load_sample error : Cannot read file %s\n", path);
        return 0;
    }
    sf_close (file);
    
    // Stéréo ou mono ?
    if (file_info.channels != 1 && file_info.channels != 2) {
        free (samples);
        fprintf (stderr, "audio_load_sample error : Format of %s is not supported\n", path);
        return 0;
    }
    ALenum format = ((file_info.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16);
    
    // Création du tampon
    ALuint buffer_id;
    alGenBuffers (1, &buffer_id);
    alBufferData (buffer_id, format, samples, n_samples * sizeof (ALushort), sample_rate);
    free (samples);
    
    if (alGetError () != AL_NO_ERROR) {
        fprintf (stderr, "audio_load_sample error : openAL failed at the creation of a buffer for %s\n", path);
        return 0;
    }
    
    return buffer_id;
#else
    return 1;
#endif
}

void audio_play_sound (ALuint sound, ALuint source, float gain) {
#ifndef NOSOUND
    if (source == 0 || sound == 0) {
        return;
    }

    alSourceStop (source);
    alSourcei (source, AL_BUFFER, sound);
    alSourcef (source, AL_GAIN, gain);
    alSourcePlay (source);
#endif
}

void audio_delete_sound (ALuint sound) {
#ifndef NOSOUND
    alDeleteBuffers (1, &sound);
#endif
}

ALuint audio_create_source (float ref_dist) {
#ifndef NOSOUND
    ALuint source;
    alGenSources (1, &source);
    alSourcef (source, AL_REFERENCE_DISTANCE, ref_dist);
    return source;
#else
    return 1;
#endif
}

void audio_source_update (ALuint source, float x, float y, float z, float dx, float dy, float dz) {
#ifndef NOSOUND
    alSource3f (source, AL_POSITION, x, y, z);
    alSource3f (source, AL_DIRECTION, dx, dy, dz);
#endif
}

void audio_delete_source (ALuint source) {
#ifndef NOSOUND
    alSourcei (source, AL_BUFFER, 0);
    alDeleteSources (1, &source);
#endif
}

void audio_update_listener (float x, float y, float z, float dx, float dy, float dz, float ux, float uy, float uz) {
#ifndef NOSOUND
    alListener3f (AL_POSITION, x, y, z);
    
    float fv [6] = {dx, dy, dz, ux, uy, uz};
    alListenerfv (AL_ORIENTATION, fv);
#endif
}


