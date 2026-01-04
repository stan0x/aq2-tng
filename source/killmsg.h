//-----------------------------------------------------------------------------
// killmsg.h - Q2Admin-Python interface functions
//
// This file provides data export functions for q2admin-python to access
// game state without modifying core TNG source files.
//-----------------------------------------------------------------------------

#ifndef KILLMSG_H
#define KILLMSG_H

// Get player's current health (-1 if invalid client)
int Q2A_GetPlayerHealth(int client_num);

// Get player's score/frags
int Q2A_GetPlayerScore(int client_num);

// Get player's team (0 = none, 1 = team1, 2 = team2, 3 = team3)
int Q2A_GetPlayerTeam(int client_num);

// Get player's kills and deaths
void Q2A_GetPlayerKD(int client_num, int *kills, int *deaths);

// Get player's current weapon name (empty string if invalid)
const char* Q2A_GetPlayerWeapon(int client_num);

// Get player's position in the world
void Q2A_GetPlayerOrigin(int client_num, float *x, float *y, float *z);

// Check if player is alive
int Q2A_IsPlayerAlive(int client_num);

// Get comprehensive player stats as a formatted string
// Format: "name:X|team:X|health:X|score:X|kills:X|deaths:X|shots:X|hits:X"
void Q2A_GetPlayerStats(int client_num, char *buf, int bufsize);

// Get player's name
const char* Q2A_GetPlayerName(int client_num);

// Get number of active players
int Q2A_GetPlayerCount(void);

// Get player's IP address
const char* Q2A_GetPlayerIP(int client_num);

// Get current map name
const char* Q2A_GetMapName(void);

// Get player's accuracy stats (returns accuracy percentage 0-100)
float Q2A_GetPlayerAccuracy(int client_num);

// Get player's headshot count
int Q2A_GetPlayerHeadshots(int client_num);

// Get player's total shots fired
int Q2A_GetPlayerShotsFired(int client_num);

// Get player's total hits
int Q2A_GetPlayerHits(int client_num);

#endif // KILLMSG_H
