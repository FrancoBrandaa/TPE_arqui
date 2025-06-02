
#include<libasm.h>


/*
 * play_tone: Generate a sound on the PC speaker.
 *
 * Parameter:
 *   frequency_hz – Tone frequency in Hertz (e.g., 440 for A4).
 *
 * Requirements:
 *   • Available functions: inb(port) and outb(port, value):
 *       unsigned char inb(unsigned short port);
 *       void outb(unsigned short port, unsigned char value);
 *   • The PC PIT runs at 1,193,180 Hz.
 *   • PIT channel 2 is used for the speaker and is set with command 0xB6.
 *   • To enable the speaker, read port 0x61 and OR with 0x03.
 *   • To disable the speaker, AND port 0x61 with 0xFC.
 */
void playTone(unsigned int frequency);



/**
 * @brief Stops the sound on the PC speaker.
 *
 * This function disables the speaker by clearing the appropriate bits in port 0x61.
 */
void stopSound(void);