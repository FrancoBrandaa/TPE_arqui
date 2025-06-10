#ifndef SOUND_DRIVER_H
#define SOUND_DRIVER_H

#include <libasm.h>

/**
 * @brief Plays a tone at the specified frequency for the given duration.
 * 
 * @param frequency The frequency of the tone in Hz.
 * @param duration_ms Duration of the tone in milliseconds.
 */
void playTone(int frequency, int duration_ms);

#endif