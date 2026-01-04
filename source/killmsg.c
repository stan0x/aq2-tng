//-----------------------------------------------------------------------------
// killmsg.c - Q2Admin-Python interface functions
//
// This file provides data export functions for q2admin-python to access
// game state without modifying core TNG source files.
//-----------------------------------------------------------------------------

#include "g_local.h"
#include "killmsg.h"

//
// Helper function to validate and get client edict
//
static edict_t* GetClientEdict(int client_num)
{
	if (client_num < 0 || client_num >= game.maxclients)
		return NULL;
	
	edict_t *ent = g_edicts + 1 + client_num;
	
	if (!ent->inuse || !ent->client)
		return NULL;
	
	return ent;
}

//
// Get player's current health (-1 if invalid client)
//
int Q2A_GetPlayerHealth(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return -1;
	
	return ent->health;
}

//
// Get player's score/frags
//
int Q2A_GetPlayerScore(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return ent->client->resp.score;
}

//
// Get player's team (0 = none, 1 = team1, 2 = team2, 3 = team3)
//
int Q2A_GetPlayerTeam(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return ent->client->resp.team;
}

//
// Get player's kills and deaths
//
void Q2A_GetPlayerKD(int client_num, int *kills, int *deaths)
{
	*kills = 0;
	*deaths = 0;
	
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return;
	
	*kills = ent->client->resp.kills;
	*deaths = ent->client->resp.deaths;
}

//
// Get player's current weapon name (empty string if invalid)
//
const char* Q2A_GetPlayerWeapon(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent || !ent->client->weapon)
		return "";
	
	return ent->client->weapon->pickup_name;
}

//
// Get player's position in the world
//
void Q2A_GetPlayerOrigin(int client_num, float *x, float *y, float *z)
{
	*x = *y = *z = 0.0f;
	
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return;
	
	*x = ent->s.origin[0];
	*y = ent->s.origin[1];
	*z = ent->s.origin[2];
}

//
// Check if player is alive
//
int Q2A_IsPlayerAlive(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return (ent->health > 0 && ent->deadflag == DEAD_NO) ? 1 : 0;
}

//
// Get comprehensive player stats as a formatted string
// Format: "name:X|team:X|health:X|score:X|kills:X|deaths:X|shots:X|hits:X"
//
void Q2A_GetPlayerStats(int client_num, char *buf, int bufsize)
{
	buf[0] = '\0';
	
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return;
	
	Com_sprintf(buf, bufsize,
		"name:%s|team:%d|health:%d|score:%d|kills:%d|deaths:%d|shots:%d|hits:%d",
		ent->client->pers.netname,
		ent->client->resp.team,
		ent->health,
		ent->client->resp.score,
		ent->client->resp.kills,
		ent->client->resp.deaths,
		ent->client->resp.shotsTotal,
		ent->client->resp.hitsTotal
	);
}

//
// Get player's name
//
const char* Q2A_GetPlayerName(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return "";
	
	return ent->client->pers.netname;
}

//
// Get number of active players
//
int Q2A_GetPlayerCount(void)
{
	int i;
	int count = 0;
	edict_t *ent;
	
	for (i = 0; i < game.maxclients; i++)
	{
		ent = g_edicts + 1 + i;
		if (ent->inuse && ent->client)
			count++;
	}
	
	return count;
}

//
// Get player's IP address
//
const char* Q2A_GetPlayerIP(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return "";
	
	return ent->client->pers.ip;
}

//
// Get current map name
//
const char* Q2A_GetMapName(void)
{
	return level.mapname;
}

//
// Get player's accuracy stats (returns accuracy percentage 0-100)
//
float Q2A_GetPlayerAccuracy(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0.0f;
	
	int shots = ent->client->resp.shotsTotal;
	int hits = ent->client->resp.hitsTotal;
	
	if (shots == 0)
		return 0.0f;
	
	return (float)hits / (float)shots * 100.0f;
}

//
// Get player's headshot count
//
int Q2A_GetPlayerHeadshots(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return ent->client->resp.hitsLocations[LOC_HDAM];
}

//
// Get player's total shots fired
//
int Q2A_GetPlayerShotsFired(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return ent->client->resp.shotsTotal;
}

//
// Get player's total hits
//
int Q2A_GetPlayerHits(int client_num)
{
	edict_t *ent = GetClientEdict(client_num);
	if (!ent)
		return 0;
	
	return ent->client->resp.hitsTotal;
}
