#ifndef PONGIS_GOLF_H
#define PONGIS_GOLF_H

#include <libc.h>
#include <pongisLib.h>
#include <shell.h>

/**
 * @brief Initializes and displays the main menu of Pongis Golf.
 * 
 * This function shows the game menu with options for single player,
 * two player mode, difficulty selection, and exit. Uses keyboard
 * input to navigate through menu options.
 */
void startPongis(void);

/**
 * @brief Starts a new game session with the specified number of players.
 * 
 * This is the main game loop that handles player movement, ball physics,
 * collision detection, and win/lose conditions. The game continues until
 * a player wins or loses.
 * 
 * @param numPlayers Number of players (1 or 2).
 */
void startGame(int numPlayers);

/**
 * @brief Displays the victory screen when a level is completed.
 * 
 * Shows congratulations message and options to continue to next level
 * or quit the game. Handles progression through the 3 game levels.
 * 
 * @param numPlayers Number of players in the current game.
 * @param winner Player number who won (1 or 2), only relevant for 2-player mode.
 */
void win(int numPlayers, int winner);

/**
 * @brief Displays the game over screen when the player loses.
 * 
 * Shows a defeat message with accompanying sound effects and provides
 * option to return to the main menu. Resets collision counters.
 */
void lose(void);

/**
 * @brief Draws the collision counter on screen during gameplay.
 * 
 * Displays current collision count versus maximum allowed collisions
 * for each player in the top-left corner of the screen.
 * 
 * @param numPlayers Number of players to show counters for (1 or 2).
 */
void drawCollisionCounter(int numPlayers);

#endif // PONGIS_GOLF_H