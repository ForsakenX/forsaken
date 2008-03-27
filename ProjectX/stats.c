/*******************************************************************\
*																	
*	stats.c														
*																
*	Deals with all statistical related proceedures				
*																
\*******************************************************************/

#include "stats.h"
#include <stdio.h>
#include "main.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "Node.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "ddsurfhand.h"
#include "getdxver.h"
#include <stdarg.h>
#include <math.h>
#include <time.h>
#include "d3dmain.h"
#include "2dtextures.h"
#include "mload.h"
#include "primary.h"
#include "secondary.h"
#include "triggers.h"
#include "pickups.h"
#include "Ships.h"
#include "exechand.h"
#include "collision.h"
#include "ddutil.h"
#include "2dpolys.h"
#include "lines.h"
#include "polys.h"
#include "lights.h"
#include "models.h"
#include "2dtextures.h"
#include "visi.h"
#include "tload.h"
#include "sfx.h"
#include "transexe.h"
#include "Text.h"
#include "Mxaload.h"
#include "dinput.h"
#include "camera.h"
#include "title.h"
#include "screenpolys.h"
#include "controls.h"
#include "comm.h"
#include "triggers.h"
#include "enemies.h"
#include "tasker.h"
#include "bsp.h"
#include "trigarea.h"
#include "multiplayer.h"
#include  <fcntl.h>
#include  <sys/types.h>
#include  <sys/stat.h>
#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>
#include  <malloc.h>
#include  <string.h>
#include  "file.h"
#include  "pvrd3d.h"
#include  "PolySort.h"
#include  "Ai.h"
#include  "Water.h"
#include  "spotfx.h"
#include <process.h>
#include  "ExtForce.h"
#include  "Teleport.h"
#include  "rtlight.h"
#include  "avi.h"
#include  "restart.h"
#include "Local.h"
#include  "goal.h"
#include  "LoadSave.h"
#include  "XMem.h"
#include "dpthread.h"

/* external variables */
extern  BYTE  TeamNumber[MAX_PLAYERS];										// which team each player is on
extern GetName(int Player);															// returns player's short name from Ships.c

/* internal variables */
int	PrimaryStats[MAX_PLAYERS+1][MAXPRIMARYWEAPONS+1];				// PrimaryStats[Killer][PrimaryWeaponType];
int	SecondaryStats[MAX_PLAYERS+1][TOTALSECONDARYWEAPONS];		// SecondaryStats[Killer][SecondaryWeaponType];
int	KillStats[MAX_PLAYERS+1][MAX_PLAYERS+1];								// KillStats[Killer][Victim];
int BonusStats[MAX_PLAYERS+1];
int KillCounter[MAX_PLAYERS+1];													// number of kills made during this life
int x, z;																						// index counters

char *PrimaryWeaponName[MAXPRIMARYWEAPONS+1]			= { "PULSAR", "TROJAX", "PYROLITE", "TRANSPULSE", "SUSS-GUN", "LASER", "ORBITOR" };
char *SecondaryWeaponName[TOTALSECONDARYWEAPONS]	= { "MUG", "SOLARIS", "THIEF", "SCATTER", "GRAVGON", "MFRL", "TITAN", "PURGE MINE", "PINE MINE", "QUANTUM MINE", "SPIDER MINE", "PINE MISSILE", "TITAN SHRAPNEL", "ENEMY SPIRAL MISSILE", "ENEMY HOMING MISSILE", "ENEMY BLUE HOMING MISSILE", "ENEMY FIREBALL", "ENEMY TENTACLE", "ENEMY DEPTH CHARGE" };
	

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get name of player's weapon with most kills...
  Input   :   int Player ID, int: Weapon Type
  Output    :   char*: name of weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get name of player's weapon with the most kills of specified type */
char* GetFavWeapon(int PlayerID, int WeaponType)
{
	char* FavWeapon = "";	// weapon text to return
	char WeaponKills[6];	// used to convert highest kills as int to char
	int highest = 0;		// highest number of kills with a single weapon
	BOOL Found = FALSE;		// could we find at least one weapon with some kills?

	// find primary weapon(s) with most kills
	if(WeaponType == WEPTYPE_Primary)
	{
		// search all multiplayer primary weapons
		for( x = 0; x < MAXPRIMARYWEAPONS+1; x++)
		{
			// found the highest killing primary weapon
			if(PrimaryStats[PlayerID][x] > highest)
			{
				highest = PrimaryStats[PlayerID][x];		// update highest
				strcpy(FavWeapon,GetWeaponName(WEPTYPE_Primary,x));
				sprintf(WeaponKills, "%i\0", highest);		// convert int to char
				strcat(FavWeapon," (");
				strcat(FavWeapon,WeaponKills);
				strcat(FavWeapon,")");
				Found = TRUE;
			}
		}
		if(Found == TRUE)
			return FavWeapon;	// return primary weapon with most kills
		else
			return "NONE";		// couldn't find any primary weapons with kills
	}
	// find secondary weapon(s) with most kills
	else if(WeaponType == WEPTYPE_Secondary)
	{
		// search all multiplayer secondary weapons
		for( x = 0; x < TOTALSECONDARYWEAPONS; x++)
		{
			// found the highest killing secondary weapon
			if(SecondaryStats[PlayerID][x] > highest)
			{
				highest = SecondaryStats[PlayerID][x];		// update highest
				strcpy(FavWeapon,GetWeaponName(WEPTYPE_Secondary,x));
				sprintf(WeaponKills, "%i\0", highest);		// convert int to char
				strcat(FavWeapon," (");
				strcat(FavWeapon,WeaponKills);
				strcat(FavWeapon,")");
				Found = TRUE;
			}			
		}
		if(Found == TRUE)
			return FavWeapon;	// return secondary weapon with most kills
		else
			return "NONE";		// couldn't find any secondary weapons with kills
	}
	// invalid weapon type given
	else
		return "GetFavWeapon():: INVALID WEAPON TYPE";
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Weapon Name...
  Input   :   int: Weapon Type, int: Weapon ID
  Output    :   char*: name of weapon
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get weapon name */
char* GetWeaponName(int WeaponType, int Weapon)
{
	if(WeaponType == WEPTYPE_Primary)
		return PrimaryWeaponName[Weapon];
	else if(WeaponType == WEPTYPE_Secondary)
		return SecondaryWeaponName[Weapon];
	else
		return "GetWeaponName():: INVALID WEAPON TYPE";
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Weapon Kill Stats...
  Input   :   int: Player ID, int: Weapon Type, int Weapon ID
  Output    :   int: number of kills
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get Weapon Kill Stats */
int GetWeaponKillStats(int PlayerID, int WeaponType, int Weapon)
{
	if(WeaponType == WEPTYPE_Primary)
		return PrimaryStats[PlayerID][Weapon];
	else if (WeaponType == WEPTYPE_Secondary)
		return SecondaryStats[PlayerID][Weapon];
	else
		return -1;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Reset all statistics...
  Input   :   nothing
  Output    :   nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Reset All Statistics */
void ResetAllStats()
{
	// for every player
	for(x = 0; x < MAX_PLAYERS; x++)
	{
		// reset all player's individual kill stats
		for(z = 0; z < MAX_PLAYERS; z++)
			KillStats[x][z] = 0;

		// reset all player's primary weapon kill stats
		for(z = 0; z < MAXPRIMARYWEAPONS+1; z++)
			PrimaryStats[x][z] = 0;

		// reset all player's secondary weapon kill stats
		for(z= 0; z < TOTALSECONDARYWEAPONS; z++)
			SecondaryStats[x][z] = 0;

		// reset all player's bonus stats
		BonusStats[x] = 0;

		// reset all player's sequential kill counters
		KillCounter[x] = 0;
	}
}



/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Update Kill Statistics...
  Input   :   killer id, victim id, weapon type, weapon used
  Output    :   nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Update Individual Kill and Weapon Kill Statistics */
void UpdateKillStats(int Killer, int Victim, int WeaponType, int Weapon)
{
	// note who killed whom
	KillStats[Killer][Victim]++;
	// note weapon used
	if(WeaponType == WEPTYPE_Primary)
		PrimaryStats[Killer][Weapon]++;
	else if (WeaponType == WEPTYPE_Secondary)
		SecondaryStats[Killer][Weapon]++;

	// update the current kills this life for the killer
	UpdateKillCount(Killer);

	// reset the current kills this life for the victim
	KillCounter[Victim] = 0;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Update Sequential Kill Count and displays messages as appropriate...
  Input   :   killer id
  Output    :   nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Update number of kills made during this life */
void UpdateKillCount(int Killer)
{
	char	tempstr[256];
	char	prefix[256];
	BOOL PlaySound = FALSE;

	// update counter
	KillCounter[Killer]++;

	// name of killer
	if(Killer == WhoIAm)
	{
		strcpy(prefix, "YOU ARE");
		PlaySound = TRUE;
	}
	else
	{
		strcpy(prefix, GetName(Killer));
		strcat(prefix, " IS");
	}

	// check for milestone achievements
	switch(KillCounter[Killer])
	{
		case 3:	
					if(PlaySound) PlaySfx( SFX_KILLINGSPREE, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "ON A KILLING SPREE (3 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
		case 5:
					if(PlaySound) PlaySfx( SFX_RAMPAGE, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "ON A RAMPAGE (5 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
		case 8:
					if(PlaySound) PlaySfx( SFX_DOMINATING, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "DOMINATING (8 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
		case 11:
					if(PlaySound) PlaySfx( SFX_UNSTOPPABLE, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "UNSTOPPABLE (11 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
		case 15:
					if(PlaySound) PlaySfx( SFX_WICKEDSICK, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "WICKED SICK (15 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
		case 20:
					if(PlaySound) PlaySfx( SFX_GODLIKE, 1.0F );
					sprintf( (char*)&tempstr[0], "%s %s", prefix, "GOD LIKE (20 KILLS)" );
   					AddMessageToQue( (char*)&tempstr[0] );
					break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Update Individual bonus statistics...
  Input   :   player id, points scored
  Output    :   nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Update Individual bonus statistics */
void UpdateBonusStats(int Player, int Points)
{
	BonusStats[Player] += Points;
}
/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Individual Kill Statistics...
  Input   :   killer id, victim id
  Output    :   amount of kills
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get An Individual Kill Statistic */
int GetKillStats(int Killer, int Victim)
{
	// return the amount of kills on victim
	return KillStats[Killer][Victim];	
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get a player's score...
  Input   :   player id
  Output    :   score
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get An Individual Score */
int GetScoreStats(int Player)
{
	int score = 0;

	// search all players
	for(x = 0; x < MAX_PLAYERS; x++)
	{
		// add kills
		if(x!=z && TeamNumber[x] != TeamNumber[z])
			score += GetKillStats(Player,x);
		// minus suicides and friendly kills
		else
			score -= GetKillStats(Player,x);
	}

	// add bonus points
	score += GetBonusStats(Player);

	// return an individual player's score
	return score;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get an individual's bonus statistics (e.g. flag or bounty points)...
  Input   :   player id
  Output    :   bonus points
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get an individual's bonus statistics */
int GetBonusStats(int Player)
{
	// return an individual player's bonus points
	return BonusStats[Player];	
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Team Kills...
  Input   :   killer id
  Output    :   total kills achieved by all players on the same team (minuses suicides and 'friendly kills')
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get A Player's Team's Total Kills */
int GetTeamKills(int Killer)
{
	int TeamKills = 0; // total kills team made

	// search all players
	for(x = 0; x < MAX_PLAYERS; x++)
	{
		// found a team mate or myself
		if(TeamNumber[Killer] == TeamNumber[x])
		{
			// search all the players that my team mate or i killed
			for(z = 0; z < MAX_PLAYERS; z++)
			{
				// add kills
				if(x!=z && TeamNumber[x] != TeamNumber[z])
					TeamKills += GetKillStats(x,z);
				// minus suicides and friendly kills
				else
					TeamKills -= GetKillStats(x,z);
			}
		}
	}

	return TeamKills;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Team Score (includes kills)...
  Input   :   killer id
  Output    :   total score achieved by all players on the same team (minuses suicides and 'friendly kills')
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get A Team's Total Score */
int GetTeamScore(int Player)
{
	int TeamScore = 0; // total score team achieved

	// for every player
	for(x = 0; x < MAX_PLAYERS; x++)
	{
		// add my team's scores
		if(TeamNumber[Player] == TeamNumber[x])
		{
			for(z = 0; z < MAX_PLAYERS; z++)
				TeamScore += GetScoreStats(z);
		}

	}

	return TeamScore;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Total Kills...
  Input   :   killer id
  Output    :   total kills on all players (excludes suicides)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get A Player's Total Kills */
int GetTotalKills(int Killer)
{
	int kills = 0; // total number of kills

	// add kills achieved on all players
	for(x = 0; x < MAX_PLAYERS; x++)
	{
		// don't add suicides
		if(Killer!=x)
			kills += GetKillStats(Killer,x);	// add kills
	}

	return kills;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  Procedure :   Get Total Deaths...
  Input   :   victim id
  Output    :   total deaths (include suicides)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
/* Get A Player's Total Deaths */
int GetTotalDeaths(int Victim)
{
	int deaths = 0; // total number of deaths

	// sum deaths
	for(x = 0; x < MAX_PLAYERS; x++)
		deaths += GetKillStats(x,Victim);

	return deaths;
}