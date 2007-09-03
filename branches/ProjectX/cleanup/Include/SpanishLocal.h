// in the following text:
// <team name>=any one of LT_TEAM_??? defined below

// text.c
//#define LT_TEAM_1		"àÀáÁäÄâÂèÈéÉëËêÊìÌíÍïÏîÎ"
//#define LT_TEAM_2		"òÒóÓöÖôÔùÙúÚüÜûÛñÑ"
#define LT_TEAM_1		"Rojo"
#define LT_TEAM_2		"Verde"
#define LT_TEAM_3		"Azul" 
#define LT_TEAM_4		"Amarillo"

#define LIVES			"Vidas"
#define TEAM			"Equipo"

// Mydplay.c
#define IS_JOINING_THE_GAME "se une al juego"		// player is joining the games
#define THE_SESSION_HAS_BEEN_LOST_PLEASE_QUIT "Has perdido la Sesión, abandona por favor"
#define YOU_HAVE_BECOME_THE_HOST "Ahora eres el Anfitrión"
#define HAS_LEFT_THE_GAME "ha abandonado el juego"
#define YOUVE_BEEN_SCATTERED "Has sido dispersado"
#define FROM_YOUR_OWN_TEAM "de tu propio equipo"
#define YOU "tú"
#define GOT_CAUGHT_WITH_A_BOMB "Fue Atrapado Con una Bomba"
#define WITH_THE_BOUNTY "con el botín"
#define NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY "no hay puntos por matar %s sin el botín" // %s= Name of Player
#define HIMSELF "él mismo"
#define ON_HIS_OWN_TEAM "en su propio equipo"
#define PACKETS_PER_SECOND_SET "Packs por segundo definidos"
#define THE_COLOUR_TEAM_HAVE_SCORED "el equipo %s ha anotado" // %s=<team name>
#define THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG "el equipo %s recupera su bandera"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED "la bandera del equipo %s ha sido recuperada"	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL "la bandera del equipo %s se ha deslizado en su meta" // %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL " la bandera del equipo %s ha escapado de su meta"	// %s=<team name>
#define HAS_GOT_THE_BOUNTY "ha obtenido el botín"
#define YOU_HAVE_GOT_A_NEW_BOMB "Has Obtenido una Nueva Bomba"
#define ON_YOUR_OWN_TEAM "En Tu Propio Equipo"

// Multiplayer.c
#define YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE "Tienes que instalar DirectX 6 para poder utilizar el parche\n"
#define CONNECTION_INITIALIZATION_ERROR "Error de Conexión de Inicialización..."
#define COULDNT_GET_SERVICE_PROVIDER_CAPS "No Obtuvo Provider Caps"
#define COULDNT_OPEN_SESSION "No Pudo Iniciar Sesión"
#define COULDNT_CREATE_PLAYER "No Pudo Crear Jugador"
#define NO_MULTIPLAYER_LEVELS "No hay niveles Varios Jugadores"


// Pickups.c
// in the following text:
// <player name>=any player's name (typed in by player)
// <pickup name>=any one of LT_PICKUP_??? defined below

#define LT_PICKUP_TROJAX				"Trojax"						// Primary Weapons
#define LT_PICKUP_PYROLITE				"Fusil Pirolita"
#define LT_PICKUP_TRANSPULSE			   	"Cañón Transpulso"
#define LT_PICKUP_SUSSGUN				"Cañón Perforador"
#define LT_PICKUP_LASER					"Rayo Láser"
#define LT_PICKUP_MUG					"Misil Mug"					// Secondary Weapons
#define LT_PICKUP_MUG_PACK				"Misiles Mug"
#define LT_PICKUP_SOLARIS				"Misil Solaris"
#define LT_PICKUP_SOLARIS_PACK			"Pack Misiles Solaris"
#define LT_PICKUP_THIEF					"Misil Ladrón"
#define LT_PICKUP_SCATTER				"Misil Dispersor"
#define LT_PICKUP_GRAVGON				"Misil Gravgon"
#define LT_PICKUP_MFRL					"LMCF"
#define LT_PICKUP_TITAN					"Misil TitanStar"
#define LT_PICKUP_PURGE					"Pack Mina Purgantes"
#define LT_PICKUP_PINE					"Pack Mina Revitalizadora"
#define LT_PICKUP_QUANTUM				"Pack Mina Cuántica"
#define LT_PICKUP_SPIDER				"Pack Mina Araña"
#define LT_PICKUP_PARASITE				"Mina Parásita"
#define LT_PICKUP_FLARE					"Bengala"
#define LT_PICKUP_GENAMMO				"Munición General"					// Ammo
#define LT_PICKUP_PYROFUEL				"Combustible Pirolita"
#define LT_PICKUP_SUSSAMMO				"Munición Cañón Perforador"
#define LT_PICKUP_POWER_POD				"Tanque Energético"					// Extras
#define LT_PICKUP_SHIELD				"Escudo"
#define LT_PICKUP_INVULBERABILITY		"Invulnerabilidad"
#define LT_PICKUP_EXTRA_LIFE			"Vida Extra"
#define LT_PICKUP_TARGETING_COMPUTER	"Ordenador Puntería"
#define LT_PICKUP_SMOKE					"Lanzahumo"
#define LT_PICKUP_NITRO					"Nitro"
#define LT_PICKUP_GOGGLES				"Gafas"
#define LT_PICKUP_GOLD_BARS				"Lingotes de Oro"
#define LT_PICKUP_STEALTH				"Invisibilidad"
#define LT_PICKUP_CRYSTAL				"Cristal"
#define LT_PICKUP_ORBITAL				"Pulsar Orbital"
#define LT_PICKUP_GOLDEN_POWER_POD		"Tanque Energético Dorado"
#define LT_PICKUP_DNA					"ADN"
#define LT_PICKUP_BOMB					"Bomba"
#define LT_PICKUP_GOLDEN_IDOL			"Ídolo Dorado"

#define YOU_ALREADY_HAVE				"Ya tienes %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_A				"Ya tienes un %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_MAX			"Ya tienes max %s" // %s=<pickup name>
#define FULL_POWER_LEVEL				"Nivel Energía Total"
#define POWER_POD_LEVEL					"Nivel Tanque Energético %d" // %d=1, 2 or 3
#define YOU_ALREADY_HAVE_MAX_POWER		"Ya tienes energía max"
#define YOU_CANT_HANDLE_ANY_MORE		"No puedes manejar más"
#define YOUVE_EARNED_EXTRA_LIFE		"Te has ganado una vida extra..."
#define YOU_HAVE_ONE_GOLD_BAR			"Ahora tienes %hd Lingote de oro" // %hd=1
#define YOU_HAVE_GOLD_BARS				"Ahora tienes %hd Lingotos de oro" // %hd=2, 3, 4...
#define YOU_HAVE_ONE_CRYSTAL			"Ahora tienes %d cristal" // %hd=1
#define YOU_HAVE_CRYSTALS				"Ahora tienes %d cristales" // %hd=2, 3, 4...

#define TAKE_FLAG_TO_GOAL				"lleva bandera a meta %s" // %s=<team name>
#define SOMEONE_HAS_GOT_THE_FLAG		"%s en el equipo %s tiene la bandera" // %s1=<player name>, %s2=<team name>
#define YOU_HAVE_GOT_THE_BOUNTY			"has encontrado el botín" 
#define TEAM_FLAG_RETURNED				"la bandera %s ha sido devuelta" // %s=<team name>
#define RETURN_TEAM_FLAG				"devuelve bandera a meta %s" // %s=<team name>
#define CANNOT_PICKUP_OWN_FLAG			"no puedes recoger la bandera de tu propio equipo"
#define SOMEONE_HAS_OTHER_TEAM_FLAG		"%s en el equipo %s tiene la bandera %s" // %s1=<player name>, %s2=<team name>, %s3=<team name>
#define FLAG_DRIFTED_BACK				"la bandera %s se deslizó de vuelta en su meta" // %s=<team name>
#define FLAG_ESCAPED					"la bandera %s escapó de su meta" // %s=<team name>

// oct2.c
#define A_BOMB_KILLED_YOU				"Una Bomba Te Ha Matado..."

// models.c
// in the following text:
// <death method>=any one of the LT_DEATHMETHOD_??? defined below
#define AN_ENEMY_KILLED_YOU				"un enemigo te %s" // %s=<death method>
#define ENEMY_KILLED_YOU				"enemigo te %s" // %s=<death method>
#define YOU_KILLED_YOURSELF_HOW			"tú te %s" // %s=<death method>
#define SOMEONE_KILLED_YOU				"%s %s te %s" // %s1=<player name> %s2=<death method>, %s3=either FROM_YOUR_OWN_TEAM (see following line) or empty
#define FROM_YOUR_OWN_TEAM				"de tu propio equipo"
#define YOU_DIED						"has muerto"

// primary.c
#define LT_DEATHMETHOD_KILLED			"muerto"
#define LT_DEATHMETHOD_PURGE_MINED		"con mina purgante"
#define LT_DEATHMETHOD_PINE_MINED		"con mina Revitalizadora"
#define LT_DEATHMETHOD_QUANTUM_MINED	"con mina cuántica"
#define LT_DEATHMETHOD_SPIDER_MINED		"con mina araña"

#define YOU_DONT_HAVE_ANY_AMMO			"No te queda munición"
#define YOU_DONT_HAVE_THAT_WEAPON		"No tienes esa arma"
#define LASER_OVERHEATED				"Láser Sobrecargado"

// secondary.c
#define INCOMING_MISSILE				"Misil llegando"
#define YOUVE_BEEN_SCATTERED			"Has sido dispersado"

// ships.c
#define TEAM_SCORED				"el equipo %s ha anotado" // %s=<team name>
#define OTHER_TEAM_FLAG_RETURNED		"el equipo %s ha recuperado su bandera" // %s=<team name>
#define YOU_NEED_FLAG					"necesitas la bandera %s para anotar" // %s=<team name>
#define YOU_KILLED_YOURSELF				"te has matado"
#define NO_LIVES_LEFT					"No Quedan Vidas"
#define LAST_LIFE						"Última Vida"
#define ONE_LIFE_LEFT					"Queda %d Vida" // %d=1
#define LIVES_LEFT						"Quedan %d Vidas" // %d=2, 3, 4...
#define RESTART_ACTIVATED				"Posición ReIniciar Activada"

#define SHIELD_CRITICAL					"escudo crítico"
#define HULL_CRITICAL					"casco crítico"


// screenpolys.c
#define OUT_OF_TIME						"Tiempo concluido"
#define IF_YOU_CANT_TAKE_THE_HEAT		"Si te molesta el calor"
#define GET_OUT_OF_THE_KITCHEN			"sal de la cocina...."

// credits.c
#define	GAME_COMPLETE1				"ENHORABUENA"
#define	GAME_COMPLETE2				"Has completado todos los"
#define	GAME_COMPLETE3				"niveles."

#define	COMPLETE_NOT_ALL_CRYSTALS1	"Pero no conseguiste todos"
#define	COMPLETE_NOT_ALL_CRYSTALS2	"los cristales."
#define COMPLETE_NOT_ALL_CRYSTALS3	"Tienes que volver a"
#define COMPLETE_NOT_ALL_CRYSTALS4	"hacerlo todo si quieres"
#define COMPLETE_NOT_ALL_CRYSTALS5	"una recompensa adecuada."

#define	COMPLETE_ALL_CRYSTALS1		"y conseguiste todos los"
#define	COMPLETE_ALL_CRYSTALS2		"cristales."
#define	COMPLETE_ALL_CRYSTALS3		"Has sido declarado"
#define	COMPLETE_ALL_CRYSTALS4		"El mercenario secreto."

#define	COMPLETE_ALLBUT1_CRYSTALS1	"pasaste por el"
#define	COMPLETE_ALLBUT1_CRYSTALS2	"nivel secreto."
#define	COMPLETE_ALLBUT1_CRYSTALS3	"Pero no conseguiste el"
#define	COMPLETE_ALLBUT1_CRYSTALS4	"cristal final. Mejor"
#define	COMPLETE_ALLBUT1_CRYSTALS5	"suerte en la próxima."

#define	PRESS_ANY_KEY1				"Aprieta"
#define	PRESS_ANY_KEY2				"cualquier tecla"

#define DEFAULT_NEWNAME "nombre"
