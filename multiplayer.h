#ifndef MULTIPLAYER_INCLUDED
#define MULTIPLAYER_INCLUDED

//
// Networking functions mainly used by title.c
//

void StartAHostSession ( MENUITEM * Item );
void GetPlayersInCurrentSession( MENUITEM *Item );
void GoToSynchup ( MENUITEM * Item );
void BailMultiplayer( MENU * Menu );
void TeamGoToSynchup ( MENUITEM * Item );
void InitTeamSelection( MENU *Menu );
void InitTeamLists( MENU *Menu );
void InitExistingGameJoin( MENU *Menu );
void CheckForMenuChange(int *dummy);
void BailMultiplayerFrontEnd( MENU * Menu );
void DistributeTeamsToLists(int *dummy);
void SetUpGameType( int type );

#endif	// MULTIPLAYER_INCLUDED
