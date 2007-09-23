/*******************************************************************\
*																	*
*	stats.h															*
*																	*
*	Header file for all statistical related proceedures				*
*																	*
\*******************************************************************/

/* prototypes */
void UpdateStats(int Killer, int Victim, int WeaponType, int Weapon);	// update the statistics
int GetTotalKills(int Killer);											// Get total number of kills (not including suicides)
int GetTotalDeaths(int Victim);											// Get total number of deaths
int GetStats(int Killer, int Victim);									// Get individual statistics
void ResetAllStats();													// Resets all statistics
char* GetFavWeapon(int PlayerID, int WeaponType);						// Get name of player's weapon with the most kills of specified type
char* GetWeaponName(int WeaponType, int Weapon);						// Get weapon name
int GetWeaponKillStats(int PlayerID, int WeaponType, int Weapon);		// Get Weapon Kill Stats	
