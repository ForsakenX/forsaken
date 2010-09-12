
#ifndef STATS
#define STATS

/*******************************************************************\
*																	
*	stats.h															
*																	
*	Header file for all statistical related proceedures				
*																	
\*******************************************************************/

/* prototypes */
void UpdateKillStats(int Killer, int Victim, int WeaponType, int Weapon);		// update the kill statistics
void UpdateBonusStats(int Player, int Points);										// update the bonus statistics (e.g. CTF or bounty points)
void UpdateKillCount(int Killer);															// updates player's kill counter for this life
void ResetAllStats();																			// Resets all statistics
void ResetIndividualStats(int Player);													// Resets one player's statistics
void ScoreSort();																				// Sorts player's score from highest to lowest
void InitScoreSortTab(int Player);														// Initiate Score Sort Tab player IDs
int GetTotalKills(int Killer);																	// Get total number of kills (not including suicides)
int GetTotalDeaths(int Victim);															// Get total number of deaths
int GetKillStats(int Killer, int Victim);													// Get an individual kill statistic
int GetScoreStats(int Player);																// Get an individual score statistic
int GetRealScore(int Player);
int GetBonusStats(int Player);															// Get an individual bonus statistic
int GetTeamScore(int Player);																// Get a player's team's total score (including kills)
int GetWeaponKillStats(int PlayerID, int WeaponType, int Weapon);			// Get Weapon Kill Stats	
int GetPlayerByRank(int Player);																// Get the id of the player at 1st, 2nd 3rd place etc.
int GetTeamScoreByTeamNumber(int Team);											// Get a player's team's total score (including kills)
char* GetWeaponName(int WeaponType, int Weapon);							// Get weapon name

// new functions

int GetSuicides( int Player );
int GetDeaths( int Player );
int GetKills( int Player );
int GetScore( int Player );
int GetTeamScore2( int Player );
int GetEffeciency( int Player );
int player_left( int player );
int scoreable_status( int player );

#endif

