// in the following text:
// <team name>=any one of LT_TEAM_??? defined below

// text.c
//#define LT_TEAM_1		"‡·‰ƒËÈ…ÎÏÌÔÚÛˆ˘˙¸‹Ò—"
#define LT_TEAM_1		"Rossa"
#define LT_TEAM_2		"Verde"
#define LT_TEAM_3		"Blu" 
#define LT_TEAM_4		"Gialla"

#define LIVES			"Vite"
#define TEAM			"Squadra"

// Mydplay.c
#define IS_JOINING_THE_GAME "si sta unendo alla partita"		// player is joining the games
#define THE_SESSION_HAS_BEEN_LOST_PLEASE_QUIT "Impossibile proseguire nella sessione, devi uscire"
#define YOU_HAVE_BECOME_THE_HOST "Sei diventato l'host"
#define HAS_LEFT_THE_GAME "ha abbandonato la partita"
#define YOUVE_BEEN_SCATTERED "Sei stato polverizzato"
#define FROM_YOUR_OWN_TEAM "dalla tua stessa squadra"
#define YOU "tu"
#define GOT_CAUGHT_WITH_A_BOMB "sei stato colpito da una bomba"
#define WITH_THE_BOUNTY "con il lingotto"
#define NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY "nessun punto per aver ucciso %s senza il lingotto" // %s= Name of Player
#define HIMSELF "da solo"
#define ON_HIS_OWN_TEAM "dalla sua stessa squadra"
#define PACKETS_PER_SECOND_SET "Impostazione pacchetti al secondo"
#define THE_COLOUR_TEAM_HAVE_SCORED "la squadra %s ha segnato" // %s=<team name>
#define THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG "la squadra %s sta riportando la sua bandiera"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED "la bandiera della squadra %s Ë stata riportata alla base"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL "la bandiera della squadra %s Ë finita nella propria porta" // %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL "la bandiera della squadra %s Ë sfuggita dalla propria porta"	// %s=<team name>
#define HAS_GOT_THE_BOUNTY "ha il lingotto"
#define YOU_HAVE_GOT_A_NEW_BOMB "adesso hai una nuova bomba"
#define ON_YOUR_OWN_TEAM "per la tua squadra"

// Multiplayer.c
#define YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE "Devi installare DirectX 6 per utilizzare la Patch\n"
#define CONNECTION_INITIALIZATION_ERROR "Errore di inizializzazione della connessione..."
#define COULDNT_GET_SERVICE_PROVIDER_CAPS "Impossibile dialogare con il provider"
#define COULDNT_OPEN_SESSION "Impossibile aprire sessione"
#define COULDNT_CREATE_PLAYER "Impossibile creare giocatore"
#define NO_MULTIPLAYER_LEVELS "Nessun livello multigiocatore"


// Pickups.c
// in the following text:
// <player name>=any player's name (typed in by player)
// <pickup name>=any one of LT_PICKUP_??? defined below

#define LT_PICKUP_TROJAX				"Trojax"						// Primary Weapons
#define LT_PICKUP_PYROLITE				"Fucile Pyrolite"
#define LT_PICKUP_TRANSPULSE				"Cannone Transpulse"
#define LT_PICKUP_SUSSGUN				"Suss-Gun"
#define LT_PICKUP_LASER					"Beam Laser"
#define LT_PICKUP_MUG					"Missile Mug "					// Secondary Weapons
#define LT_PICKUP_MUG_PACK				"Missili Mug"
#define LT_PICKUP_SOLARIS				"Missile a ricerca calorica"
#define LT_PICKUP_SOLARIS_PACK			"Ricarica missili a ricerca calorica"
#define LT_PICKUP_THIEF					"Missile Stealth"
#define LT_PICKUP_SCATTER				"Missile Scatter"
#define LT_PICKUP_GRAVGON				"Missile Gravgon"
#define LT_PICKUP_MFRL					"Lanciarazzi "
#define LT_PICKUP_TITAN					"Missile TitanStar"
#define LT_PICKUP_PURGE					"Ricarica Mine Purge"
#define LT_PICKUP_PINE					"Ricarica Mine Pine"
#define LT_PICKUP_QUANTUM				"Ricarica Mine Quantum "
#define LT_PICKUP_SPIDER				"Ricarica Mine Spider"
#define LT_PICKUP_PARASITE				"Mine Parasite"
#define LT_PICKUP_FLARE					"Flare"
#define LT_PICKUP_GENAMMO				"Munizione generica "					// Ammo
#define LT_PICKUP_PYROFUEL				"Carburante Pyrolite"
#define LT_PICKUP_SUSSAMMO				"Munizioni Suss-Gun"
#define LT_PICKUP_POWER_POD				"Capsula energetica"					// Extras
#define LT_PICKUP_SHIELD				"Scudo"
#define LT_PICKUP_INVULBERABILITY		"Invulnerabilit‡"
#define LT_PICKUP_EXTRA_LIFE			"Vita extra"
#define LT_PICKUP_TARGETING_COMPUTER	"Computer di puntamento"
#define LT_PICKUP_SMOKE					"Frangifumo"
#define LT_PICKUP_NITRO					"Nitro"
#define LT_PICKUP_GOGGLES				"Visori"
#define LT_PICKUP_GOLD_BARS				"Lingotti"
#define LT_PICKUP_STEALTH				"Mantello Stealth"
#define LT_PICKUP_CRYSTAL				"Cristallo"
#define LT_PICKUP_ORBITAL				"Pulsar orbitante"
#define LT_PICKUP_GOLDEN_POWER_POD		"Capsula energetica dorata"
#define LT_PICKUP_DNA					"DNA"
#define LT_PICKUP_BOMB					"Bomba"
#define LT_PICKUP_GOLDEN_IDOL			"Idolo dorato"

#define YOU_ALREADY_HAVE				"Possiedi gi‡ %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_A				"Possiedi gi‡ un %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_MAX			"Possiedi gi‡ il massimo di %s" // %s=<pickup name>
#define FULL_POWER_LEVEL				"Livello di massima energia"
#define POWER_POD_LEVEL					"Capsula energetica livello %d" // %d=1, 2 or 3
#define YOU_ALREADY_HAVE_MAX_POWER		"Hai gi‡ la massima energia"
#define YOU_CANT_HANDLE_ANY_MORE		"» pi˘ di quanto puoi caricare"
#define YOUVE_EARNED_EXTRA_LIFE			"Ti sei guadagnato una vita extra..."
#define YOU_HAVE_ONE_GOLD_BAR			"Possiedi %hd lingotto" // %hd=1
#define YOU_HAVE_GOLD_BARS				"Possiedi %hd lingotti" // %hd=2, 3, 4...
#define YOU_HAVE_ONE_CRYSTAL			"Possiedi %d cristallo" // %hd=1
#define YOU_HAVE_CRYSTALS				"Possiedi %d cristalli" // %hd=2, 3, 4...

#define TAKE_FLAG_TO_GOAL				"porta la bandiera alla porta della squadra %s" // %s=<team name>
#define SOMEONE_HAS_GOT_THE_FLAG		"%s della squadra %s ha la bandiera" // %s1=<player name>, %s2=<team name>
#define YOU_HAVE_GOT_THE_BOUNTY			"hai trovato il lingotto" 
#define TEAM_FLAG_RETURNED				"la bandiera della squadra %s Ë stata riportata alla base" // %s=<team name>
#define RETURN_TEAM_FLAG				"riporta la bandiera alla base della squadra %s " // %s=<team name>
#define CANNOT_PICKUP_OWN_FLAG			"non puoi raccogliere la bandiera della tua squadra"
#define SOMEONE_HAS_OTHER_TEAM_FLAG		"%s della squadra %s ha raccolto la bandiera della squadra %s" // %s1=<player name>, %s2=<team name>, %s3=<team name>
#define FLAG_DRIFTED_BACK				"la bandiera della squadra %s Ë finita in porta" // %s=<team name>
#define FLAG_ESCAPED					"la bandiera della squadra %s Ë sfuggita dalla porta" // %s=<team name>

// oct2.c
#define A_BOMB_KILLED_YOU				"Una bomba ti ha ucciso..."

// models.c
// in the following text:
// <death method>=any one of the LT_DEATHMETHOD_??? defined below
#define AN_ENEMY_KILLED_YOU				"un nemico ti ha %s" // %s=<death method>
#define ENEMY_KILLED_YOU				"il nemico ti ha %s" // %s=<death method>
#define YOU_KILLED_YOURSELF_HOW			"ti sei %s da solo" // %s=<death method>
#define SOMEONE_KILLED_YOU				"%s Ë stato %s %s" // %s1=<player name> %s2=<death method>, %s3=either FROM_YOUR_OWN_TEAM (see following line) or empty
//#define FROM_YOUR_OWN_TEAM				"dalla sua stessa squadra"
#define YOU_DIED						"sei morto"

// primary.c
#define LT_DEATHMETHOD_KILLED			"ucciso"
#define LT_DEATHMETHOD_PURGE_MINED		"bombardato"
#define LT_DEATHMETHOD_PINE_MINED		"folgorato"
#define LT_DEATHMETHOD_QUANTUM_MINED	"quantizzato"
#define LT_DEATHMETHOD_SPIDER_MINED		"avvoltolato"

#define YOU_DONT_HAVE_ANY_AMMO			"Non hai munizioni"
#define YOU_DONT_HAVE_THAT_WEAPON		"Non possiedi tale arma"
#define LASER_OVERHEATED				"Laser surriscaldato"

// secondary.c
#define INCOMING_MISSILE				"Missile in avvicinamento"
#define YOUVE_BEEN_SCATTERED			"Sei stato polverizzato"

// ships.c
#define TEAM_SCORED						"la squadra %s ha segnato" // %s=<team name>
#define OTHER_TEAM_FLAG_RETURNED		"la squadra %s ha restituito la sua bandiera" // %s=<team name>
#define YOU_NEED_FLAG					"ti serve la bandiera della squadra %s per segnare" // %s=<team name>
#define YOU_KILLED_YOURSELF				"ti sei ucciso da solo"
#define NO_LIVES_LEFT					"Nessuna vita residua"
#define LAST_LIFE						"Ultima vita"
#define ONE_LIFE_LEFT					"%d vita rimasta" // %d=1
#define LIVES_LEFT						"%d vite rimaste" // %d=2, 3, 4...
#define RESTART_ACTIVATED				"Attivato riposizionamento iniziale"

#define SHIELD_CRITICAL					"scudo critico"
#define HULL_CRITICAL					"scafo critico"


// screenpolys.c
#define OUT_OF_TIME						"Fuori tempo limite"
#define IF_YOU_CANT_TAKE_THE_HEAT		"Se non ti piace il fuoco"
#define GET_OUT_OF_THE_KITCHEN			"stai lontano dal camino..."

// credits.c
#define	GAME_COMPLETE1				"CONGRATULAZIONI"
#define	GAME_COMPLETE2				"Hai completato tutti"
#define	GAME_COMPLETE3				"i livelli."

#define	COMPLETE_NOT_ALL_CRYSTALS1	"ma non hai raccolto"
#define	COMPLETE_NOT_ALL_CRYSTALS2	"tutti i cristalli."
#define COMPLETE_NOT_ALL_CRYSTALS3	"Dovrai ricominciare tutto"
#define COMPLETE_NOT_ALL_CRYSTALS4	"da capo se vorrai essere"
#define COMPLETE_NOT_ALL_CRYSTALS5	"adeguatamente ricompensato."

#define	COMPLETE_ALL_CRYSTALS1		"e raccolto tutti i"
#define	COMPLETE_ALL_CRYSTALS2		"cristalli."
#define	COMPLETE_ALL_CRYSTALS3		"Sei stato premiato"
#define	COMPLETE_ALL_CRYSTALS4		"con il centauro segreto."

#define	COMPLETE_ALLBUT1_CRYSTALS1	"Sei uscito indenne"
#define	COMPLETE_ALLBUT1_CRYSTALS2	"dal livello segreto."
#define	COMPLETE_ALLBUT1_CRYSTALS3	"ma non hai recuperato il"
#define	COMPLETE_ALLBUT1_CRYSTALS4	"cristallo finale. Miglior"
#define	COMPLETE_ALLBUT1_CRYSTALS5	"fortuna la prossima volta."

#define	PRESS_ANY_KEY1				"Premi"
#define	PRESS_ANY_KEY2				"un tasto"

#define DEFAULT_NEWNAME "nome"
