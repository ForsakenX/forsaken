// in the following text:
// <team name>=any one of LT_TEAM_??? defined below

// text.c
#define LT_TEAM_1		"Rot"
#define LT_TEAM_2		"Grün"
#define LT_TEAM_3		"Blau" 
#define LT_TEAM_4		"Gelb"

#define LIVES			"Leben"
#define TEAM			"Team"

// Mydplay.c
#define IS_JOINING_THE_GAME "tritt dem Spiel bei"		// player is joining the games
#define THE_SESSION_HAS_BEEN_LOST_PLEASE_QUIT "Verbindung abgebrochen - bitte beenden"
#define YOU_HAVE_BECOME_THE_HOST "Sie sind nun der Host"
#define HAS_LEFT_THE_GAME "hat das Spiel verlassen"
#define YOUVE_BEEN_SCATTERED "Sie wurden gescattert"
#define FROM_YOUR_OWN_TEAM "aus dem eigenen Team"
#define YOU "Sie"
#define WITH_THE_BOUNTY "mit der Beute"
#define NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY "keine Punkte für Abschuß von %s ohne die Beute" // %s= Name of Player
#define HIMSELF "sich selbst"
#define ON_HIS_OWN_TEAM "im eigenen Team"
#define PACKETS_PER_SECOND_SET "Pakete pro Sekunde eingestellt"
#define THE_COLOUR_TEAM_HAVE_SCORED "Team %s hat gepunktet" // %s=<team name>
#define THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG "Team %s bringt seine Flagge zurück"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED "Flagge von %s wurde zurückgebracht"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL "Flagge von %s ist ins eigene Ziel getrieben" // %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL "Flagge von %s hat eigenes Ziel verlassen"	// %s=<team name>
#define HAS_GOT_THE_BOUNTY "hat die Beute"
#define ON_YOUR_OWN_TEAM "In Ihrem Team"

// Multiplayer.c
#define YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE "Sie müssen DirectX 6 installieren, um den Patch zu benutzen\n"
#define CONNECTION_INITIALIZATION_ERROR "Verbindungsinitialisierungsfehler..."
#define COULDNT_GET_SERVICE_PROVIDER_CAPS "Komme nicht an Service Provider"
#define COULDNT_OPEN_SESSION "Konnte Sitzung nicht öffnen"
#define COULDNT_CREATE_PLAYER "Konnte Spieler nicht erstellen"
#define NO_MULTIPLAYER_LEVELS "Keine Multiplayer-Level"


// Pickups.c
// in the following text:
// <player name>=any player's name (typed in by player)
// <pickup name>=any one of LT_PICKUP_??? defined below

#define LT_PICKUP_TROJAX				"Trojax"						// Primary Weapons
#define LT_PICKUP_PYROLITE				"Pyrolite"
#define LT_PICKUP_TRANSPULSE			"Transpulse"
#define LT_PICKUP_SUSSGUN				"Suss-Kanone"
#define LT_PICKUP_LASER					"Beamlaser"
#define LT_PICKUP_MUG					"Mug-Rakete"					// Secondary Weapons
#define LT_PICKUP_MUG_PACK				"Mug-Raketen"
#define LT_PICKUP_SOLARIS				"Solaris-Rakete"
#define LT_PICKUP_SOLARIS_PACK			"Solaris-Raketenpack"
#define LT_PICKUP_THIEF					"Thief-Rakete"
#define LT_PICKUP_SCATTER				"Scatter-Rakete"
#define LT_PICKUP_GRAVGON				"Gravgon-Rakete"
#define LT_PICKUP_MFRL					"Raketenwerfer"
#define LT_PICKUP_TITAN					"TitanStar-Rakete"
#define LT_PICKUP_PURGE					"Schwebeminenpack"
#define LT_PICKUP_PINE					"Raketenminenpack"
#define LT_PICKUP_QUANTUM				"Quantumminenpack"
#define LT_PICKUP_SPIDER				"Spiderminenpack"
#define LT_PICKUP_PARASITE				"Parasitenmine"
#define LT_PICKUP_FLARE					"Leuchtrakete"
#define LT_PICKUP_GENAMMO				"Allround-Munition"					// Ammo
#define LT_PICKUP_PYROFUEL				"Pyrolite-Brennstoff"
#define LT_PICKUP_SUSSAMMO				"Suss-Munition"
#define LT_PICKUP_POWER_POD				"Power Pack"					// Extras
#define LT_PICKUP_SHIELD				"Schild"
#define LT_PICKUP_INVULBERABILITY		"Unverwundbarkeit"
#define LT_PICKUP_EXTRA_LIFE			"Extraleben"
#define LT_PICKUP_TARGETING_COMPUTER	"Zielhilfe-Computer"
#define LT_PICKUP_SMOKE					"Rauchwolke"
#define LT_PICKUP_NITRO					"Nitro"
#define LT_PICKUP_GOGGLES				"Schutzbrille"
#define LT_PICKUP_GOLD_BARS				"Goldbarren"
#define LT_PICKUP_STEALTH				"Tarnmantel"
#define LT_PICKUP_CRYSTAL				"Kristall"
#define LT_PICKUP_ORBITAL				"Orbit Pulsar"
#define LT_PICKUP_GOLDEN_POWER_POD		"Goldenes Power Pack"
#define LT_PICKUP_DNA					"DNS"
#define LT_PICKUP_BOMB					"Bombe"
#define LT_PICKUP_GOLDEN_IDOL			"Goldener Götze"

#define YOU_ALREADY_HAVE				"Sie haben schon %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_A				"Sie haben schon 1 %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_MAX			"Sie haben schon max. %s" // %s=<pickup name>
#define FULL_POWER_LEVEL				"Max. Power Level"
#define POWER_POD_LEVEL					"Power Pack Level %d" // %d=1, 2 or 3
#define YOU_ALREADY_HAVE_MAX_POWER		"Sie haben schon max. Power"
#define YOU_CANT_HANDLE_ANY_MORE		"Mehr geht nicht"
#define YOUVE_EARNED_EXTRA_LIFE			"Sie haben sich ein Extraleben verdient..."
#define YOU_HAVE_ONE_GOLD_BAR			"Sie haben jetzt %hd Goldbarren" // %hd=1
#define YOU_HAVE_GOLD_BARS				"Sie haben jetzt %hd Goldbarren" // %hd=2, 3, 4...
#define YOU_HAVE_ONE_CRYSTAL			"Sie haben jetzt %d Kristall" // %hd=1
#define YOU_HAVE_CRYSTALS				"Sie haben jetzt %d Kristalle" // %hd=2, 3, 4...

#define TAKE_FLAG_TO_GOAL				"Flagge zum Ziel von %s bringen" // %s=<team name>
#define SOMEONE_HAS_GOT_THE_FLAG		"%s vom Team %s hat die Flagge" // %s1=<player name>, %s2=<team name>
#define YOU_HAVE_GOT_THE_BOUNTY			"Sie haben die Beute gefunden" 
#define TEAM_FLAG_RETURNED				"Flagge von %s wurde zurückgebracht" // %s=<team name>
#define RETURN_TEAM_FLAG				"Flagge zum Ziel von %s zurückbringen" // %s=<team name>
#define CANNOT_PICKUP_OWN_FLAG			"Sie können nicht die Flagge Ihres Teams aufheben"
#define SOMEONE_HAS_OTHER_TEAM_FLAG		"%s von %s hat die Flagge von %s" // %s1=<player name>, %s2=<team name>, %s3=<team name>
#define FLAG_DRIFTED_BACK				"Flagge von %s ist wieder ins eigene Ziel getrieben" // %s=<team name>
#define FLAG_ESCAPED					"Flagge von %s hat eigenes Ziel verlassen" // %s=<team name>

// models.c
// in the following text:
// <death method>=any one of the LT_DEATHMETHOD_??? defined below
#define AN_ENEMY_KILLED_YOU				"Ein Gegner hat Sie %s" // %s=<death method>
#define ENEMY_KILLED_YOU				"Gegner hat Sie %s" // %s=<death method>
#define YOU_KILLED_YOURSELF_HOW			"Sie haben sich selbst %s" // %s=<death method>
#define SOMEONE_KILLED_YOU				"%s hat Sie %s %s" // %s1=<player name> %s2=<death method>, %s3=either FROM_YOUR_OWN_TEAM (see following line) or empty
#define FROM_YOUR_OWN_TEAM				"aus dem eigenen Team"
#define YOU_DIED						"Sie sind tot"

// primary.c
#define LT_DEATHMETHOD_KILLED			"abgeschossen"
#define LT_DEATHMETHOD_PURGE_MINED		"schwebe-gesprengt"
#define LT_DEATHMETHOD_PINE_MINED		"raketen-gesprengt"
#define LT_DEATHMETHOD_QUANTUM_MINED	"quantum-gesprengt"
#define LT_DEATHMETHOD_SPIDER_MINED		"spider-gesprengt"

#define YOU_DONT_HAVE_ANY_AMMO			"Sie haben keine Munition"
#define YOU_DONT_HAVE_THAT_WEAPON		"Diese Waffe haben Sie nicht"
#define LASER_OVERHEATED				"Laser überhitzt"

// secondary.c
#define INCOMING_MISSILE				"Rakete nähert sich"
#define YOUVE_BEEN_SCATTERED			"Sie wurden gescattert"

// ships.c
#define TEAM_SCORED						"Team %s hat gepunktet" // %s=<team name>
#define OTHER_TEAM_FLAG_RETURNED		"Team %s hat seine Flagge zurückgebracht" // %s=<team name>
#define YOU_NEED_FLAG					"Zum Punkten brauchen Sie die Flagge von %s" // %s=<team name>
#define YOU_KILLED_YOURSELF				"Sie haben sich abgeschossen"
#define NO_LIVES_LEFT					"Keine Leben übrig"
#define LAST_LIFE						"Letztes Leben"
#define ONE_LIFE_LEFT					"%d Leben übrig" // %d=1
#define LIVES_LEFT						"%d Leben übrig" // %d=2, 3, 4...
#define RESTART_ACTIVATED				"Neustartposition aktiviert"

#define SHIELD_CRITICAL					"Schild kritisch"
#define HULL_CRITICAL					"Rumpf kritisch"


// screenpolys.c
#define OUT_OF_TIME						"Zeit um"
#define IF_YOU_CANT_TAKE_THE_HEAT		"Wenn Sie Hitze nicht vertragen,"
#define GET_OUT_OF_THE_KITCHEN			"dann raus aus der Küche..."

// credits.c
#define	GAME_COMPLETE1				"GLÜCKWUNSCH"
#define	GAME_COMPLETE2				"Sie haben alle Level"
#define	GAME_COMPLETE3				"geschafft."

#define	COMPLETE_NOT_ALL_CRYSTALS1	"Aber Sie haben nicht alle"
#define	COMPLETE_NOT_ALL_CRYSTALS2	"Kristalle."
#define COMPLETE_NOT_ALL_CRYSTALS3	"Sie müssen nochmal von vorn"
#define COMPLETE_NOT_ALL_CRYSTALS4	"anfangen, wenn Sie die"
#define COMPLETE_NOT_ALL_CRYSTALS5	"volle Belohnung möchten."

#define	COMPLETE_ALL_CRYSTALS1		"Und Sie haben alle"
#define	COMPLETE_ALL_CRYSTALS2		"Kristalle."
#define	COMPLETE_ALL_CRYSTALS3		"Dafür erhalten Sie"
#define	COMPLETE_ALL_CRYSTALS4		"den versteckten Biker."

#define	COMPLETE_ALLBUT1_CRYSTALS1	"Sie haben den verborgenen"
#define	COMPLETE_ALLBUT1_CRYSTALS2	"Level geschafft."
#define	COMPLETE_ALLBUT1_CRYSTALS3	"Aber Ihnen fehlt der"
#define	COMPLETE_ALLBUT1_CRYSTALS4	"letzte Kristall. Vielleicht"
#define	COMPLETE_ALLBUT1_CRYSTALS5	"klappt's nächstes Mal."

#define	PRESS_ANY_KEY1				"Beliebige"
#define	PRESS_ANY_KEY2				"Taste drücken"

#define DEFAULT_NEWNAME "neu"
