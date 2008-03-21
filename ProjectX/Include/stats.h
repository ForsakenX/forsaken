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
int GetTotalKills(int Killer);																	// Get total number of kills (not including suicides)
int GetTotalDeaths(int Victim);															// Get total number of deaths
int GetKillStats(int Killer, int Victim);													// Get an individual kill statistic
int GetScoreStats(int Player);																// Get an individual score statistic
int GetBonusStats(int Player);															// Get an individual bonus statistic
int GetTeamScore(int Player);																// Get a player's team's total score (including kills)
void ResetAllStats();																			// Resets all statistics
char* GetFavWeapon(int PlayerID, int WeaponType);								// Get name of player's weapon with the most kills of specified type
char* GetWeaponName(int WeaponType, int Weapon);							// Get weapon name
int GetWeaponKillStats(int PlayerID, int WeaponType, int Weapon);			// Get Weapon Kill Stats	
