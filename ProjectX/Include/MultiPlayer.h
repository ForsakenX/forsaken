
#ifndef MULTIPLAYER_INCLUDED
#define MULTIPLAYER_INCLUDED

/*
 * Defines....
 */
#define MAXSESSIONS 32


extern DPSESSIONDESC2 Sessions[];
extern BOOL	SessionsRefresh[];


/*
 * Application messages
 */

BOOL WINAPI EnumSessions(LPCDPSESSIONDESC2 lpDPSessionDesc, LPDWORD lpdwTimeOut, DWORD dwFlags, 
                        LPVOID lpContext);
BOOL WINAPI EnumPlayers(DPID pidID, DWORD dwPlayerType, LPCDPNAME lpName,
    DWORD dwFlags, LPVOID lpContext);

BOOL StartAHostSession ( MENUITEM * Item );
void GetCurrentSessions( MENU *Menu );
void GetCurrentSessions_ReScan( MENUITEM *Item );
BOOL JoinASession ( MENUITEM * Item );
void SelectSession( MENUITEM *Item );
void GetPlayersInCurrentSession( MENUITEM *Item );
void GoToSynchup ( MENUITEM * Item );

void BailMultiplayer( MENU * Menu );
void SwapListItem( LIST * Source , LIST * Dest );
void TeamGoToSynchup ( MENUITEM * Item );
void InitTeamSelection( MENU *Menu );
void InitTeamLists( MENU *Menu );
void InitExistingGameJoin( MENU *Menu );
void SwapTeamIDs( LIST * Source , LIST * Dest );
void ReAssembleTeamNumbers(int *dummy);
void CheckForMenuChange(int *dummy);
void BailMultiplayerFrontEnd( MENU * Menu );
void DistributeTeamsToLists(int *dummy);

void GetSessionInfo ( LPDPSESSIONDESC2 sd );

void SetUpGameType( int type );
BOOL RefreshDPlay ( void );

#endif	// MULTIPLAYER_INCLUDED