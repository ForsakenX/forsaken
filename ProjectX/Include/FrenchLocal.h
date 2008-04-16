// in the following text:
// <team name>=any one of LT_TEAM_??? defined below

// text.c
#define LT_TEAM_1		"Rouge"
#define LT_TEAM_2		"Verte"
#define LT_TEAM_3		"Bleue" 
#define LT_TEAM_4		"Jaune"

#define LIVES			"Vies"
#define TEAM			"Equipe"

// Mydplay.c
#define IS_JOINING_THE_GAME "rejoint la partie"		// player is joining the games
#define THE_SESSION_HAS_BEEN_LOST_PLEASE_QUIT "La session a été perdue ; veuillez quitter"
#define YOU_HAVE_BECOME_THE_HOST "Vous êtes maintenant l'hôte"
#define HAS_LEFT_THE_GAME "a quitté la partie"
#define YOUVE_BEEN_SCATTERED "Vous avez été dispersé"
#define FROM_YOUR_OWN_TEAM "de votre équipe"
#define YOU "vous"
#define WITH_THE_BOUNTY "avec le trésor"
#define NO_POINTS_FOR_KILLING_PLAYER_WITHOUT_THE_BOUNTY "pas de points pour tuer %s sans le trésor" // %s= Name of Player
#define HIMSELF "lui-même"
#define ON_HIS_OWN_TEAM "dans son équipe"
#define PACKETS_PER_SECOND_SET "Paquets par seconde"
#define THE_COLOUR_TEAM_HAVE_SCORED "l'équipe %s a marqué" // %s=<team name>
#define THE_COLOUR_TEAM_ARE_RETURNING_THEIR_FLAG "l'équipe %s rapporte son drapeau "	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_BEEN_RETURNED "le drapeau de l'équipe %s a été rapporté "	// %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_DRIFTED_INTO_THEIR_GOAL "le drapeau de l'équipe %s est passé dans son but" // %s=<team name>
#define THE_COLOUR_TEAM_FLAG_HAS_ESCAPED_FROM_THEIR_GOAL "le drapeau de l'équipe %s a quitté son but "	// %s=<team name>
#define HAS_GOT_THE_BOUNTY "détient le trésor"
#define ON_YOUR_OWN_TEAM "Dans votre équipe"

// Multiplayer.c
#define YOU_NEED_TO_INSATLL_THE_DIRECT_PLAY_50A_UPDATE "Vous devez installer DirectX 6\n"
#define CONNECTION_INITIALIZATION_ERROR "Erreur initialisation connexion..."
#define COULDNT_GET_SERVICE_PROVIDER_CAPS "Imp. accéder prestataire services"
#define COULDNT_OPEN_SESSION "Imp. ouvrir session"
#define COULDNT_CREATE_PLAYER "Imp. créer joueur"
#define NO_MULTIPLAYER_LEVELS "Pas de niveau multijoueur"


// Pickups.c
// in the following text:
// <player name>=any player's name (typed in by player)
// <pickup name>=any one of LT_PICKUP_??? defined below

#define LT_PICKUP_TROJAX				"Trojax"						// Primary Weapons
#define LT_PICKUP_PYROLITE				"Fusil Pyrolite"
#define LT_PICKUP_TRANSPULSE			"Transpulseur "
#define LT_PICKUP_SUSSGUN				"Arme intel."
#define LT_PICKUP_LASER					"Rayon laser"
#define LT_PICKUP_MUG					"Missile Mug"					// Secondary Weapons
#define LT_PICKUP_MUG_PACK				"Missiles Mug"
#define LT_PICKUP_SOLARIS				"Missile thermoguidé"
#define LT_PICKUP_SOLARIS_PACK			"Pack missile thermoguidé"
#define LT_PICKUP_THIEF					"Missile Voleur"
#define LT_PICKUP_SCATTER				"Missile dispersé"
#define LT_PICKUP_GRAVGON				"Missile Gravgon"
#define LT_PICKUP_MFRL					"Lance-roquettes"
#define LT_PICKUP_TITAN					"Missile Titan"
#define LT_PICKUP_PURGE					"Pack mine purgeante"
#define LT_PICKUP_PINE					"Pack mine multi-tête "
#define LT_PICKUP_QUANTUM				"Pack mine quantum"
#define LT_PICKUP_SPIDER				"Pack mine Araignée"
#define LT_PICKUP_PARASITE				"Mine parasite"
#define LT_PICKUP_FLARE					"Leurres IR"
#define LT_PICKUP_GENAMMO				"Munitions gén."					// Ammo
#define LT_PICKUP_PYROFUEL				"Carb. Pyrolite"
#define LT_PICKUP_SUSSAMMO				"Munitions intel."
#define LT_PICKUP_POWER_POD				"Pod énerg."					// Extras
#define LT_PICKUP_SHIELD				"Bouclier"
#define LT_PICKUP_INVULBERABILITY		"Invulnérabilité"
#define LT_PICKUP_EXTRA_LIFE			"Vie suppl."
#define LT_PICKUP_TARGETING_COMPUTER	"Ord. De visée"
#define LT_PICKUP_SMOKE					"Traces fumée"
#define LT_PICKUP_NITRO					"Nitro"
#define LT_PICKUP_GOGGLES				"Lunettes"
#define LT_PICKUP_GOLD_BARS				"Lingots d'or"
#define LT_PICKUP_STEALTH				"Manteau invisible"
#define LT_PICKUP_CRYSTAL				"Cristal"
#define LT_PICKUP_ORBITAL				"Pulsar en orbite"
#define LT_PICKUP_GOLDEN_POWER_POD		"Pod énerg. or"
#define LT_PICKUP_DNA					"ADN"
#define LT_PICKUP_GOLDEN_IDOL			"Statuette or"

#define YOU_ALREADY_HAVE				"Vous avez déjà un(e) %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_A				" Vous avez déjà un(e) %s" // %s=<pickup name>
#define YOU_ALREADY_HAVE_MAX			"Vous avez déjà le max de %s" // %s=<pickup name>
#define FULL_POWER_LEVEL				"Niv. puis. max"
#define POWER_POD_LEVEL					"Niv. Pod énerg. %d" // %d=1, 2 or 3
#define YOU_ALREADY_HAVE_MAX_POWER		"Vous avez atteint puis. max"
#define YOU_CANT_HANDLE_ANY_MORE		"Vous avez atteint votre limite"
#define YOUVE_EARNED_EXTRA_LIFE			"Vous avez gagné une vie supplémentaire..."
#define YOU_HAVE_ONE_GOLD_BAR			"Vous avez %hd lingot d'or" // %hd=1
#define YOU_HAVE_GOLD_BARS				"Vous avez %hd lingots d'or" // %hd=2, 3, 4...
#define YOU_HAVE_ONE_CRYSTAL			"Vous avez %d cristal" // %hd=1
#define YOU_HAVE_CRYSTALS				"Vous avez %d cristaux" // %hd=2, 3, 4...

#define TAKE_FLAG_TO_GOAL				"apportez le drapeau au but de l'équipe %s" // %s=<team name>
#define SOMEONE_HAS_GOT_THE_FLAG		"%s, de l'équipe %s, a le drapeau" // %s1=<player name>, %s2=<team name>
#define YOU_HAVE_GOT_THE_BOUNTY			"vous avez trouvé le trésor" 
#define TEAM_FLAG_RETURNED				"le drapeau de l'équipe %s a été rapporté" // %s=<team name>
#define RETURN_TEAM_FLAG				"rapportez le drapeau au but de l'équipe %s" // %s=<team name>
#define CANNOT_PICKUP_OWN_FLAG			"vous ne pouvez pas prendre le drapeau de votre équipe"
#define SOMEONE_HAS_OTHER_TEAM_FLAG		"%s, de l'équipe %s, a le drapeau de l'équipe %s" // %s1=<player name>, %s2=<team name>, %s3=<team name>
#define FLAG_DRIFTED_BACK				"le drapeau de l'équipe %s est retourné à son but" // %s=<team name>
#define FLAG_ESCAPED					"le drapeau de l'équipe %s a quitté son but" // %s=<team name>

// models.c
// in the following text:
// <death method>=any one of the LT_DEATHMETHOD_??? defined below
#define AN_ENEMY_KILLED_YOU				"vous avez été %s" // %s=<death method>
#define ENEMY_KILLED_YOU				"l'ennemi vous a %s" // %s=<death method>
#define YOU_KILLED_YOURSELF_HOW			"vous vous êtes %s" // %s=<death method>
#define SOMEONE_KILLED_YOU				"%s, %s, vous a %s" // %s1=<player name> %s2=<death method>, %s3=either FROM_YOUR_OWN_TEAM (see following line) or empty
#define FROM_YOUR_OWN_TEAM				"de votre équipe"
#define YOU_DIED						"vous êtes mort"

// primary.c
#define LT_DEATHMETHOD_KILLED			"tué"
#define LT_DEATHMETHOD_PURGE_MINED		"tué par mine purgeante"
#define LT_DEATHMETHOD_PINE_MINED		"tué par mine multi-tête"
#define LT_DEATHMETHOD_QUANTUM_MINED	"tué par mine quantum"
#define LT_DEATHMETHOD_SPIDER_MINED		"tué par mine araignée"

#define YOU_DONT_HAVE_ANY_AMMO			"Vous n'avez pas de munitions"
#define YOU_DONT_HAVE_THAT_WEAPON		"Vous n'avez pas cette arme"
#define LASER_OVERHEATED				"Surchauffe du laser"

// secondary.c
#define INCOMING_MISSILE				"Missile en approche"
#define YOUVE_BEEN_SCATTERED			"Vous avez été dispersé"

// ships.c
#define TEAM_SCORED						"l'équipe %s a marqué" // %s=<team name>
#define OTHER_TEAM_FLAG_RETURNED		"l'équipe %s a rapporté son drapeau" // %s=<team name>
#define YOU_NEED_FLAG					"il vous faut le drapeau de l'équipe %s pour marquer" // %s=<team name>
#define YOU_KILLED_YOURSELF				"vous vous êtes suicidé"
#define NO_LIVES_LEFT					"Plus de vie"
#define LAST_LIFE						"Dernière vie"
#define ONE_LIFE_LEFT					"Plus qu'%d vie" // %d=1
#define LIVES_LEFT						"Plus que %d vies" // %d=2, 3, 4...
#define RESTART_ACTIVATED				"Position de redémarrage activée"

#define SHIELD_CRITICAL					"Bouclier état critique"
#define HULL_CRITICAL					"Coque état critique"


// screenpolys.c
#define OUT_OF_TIME						"Temps écoulé"
#define IF_YOU_CANT_TAKE_THE_HEAT		"Si la pression est trop forte"
#define GET_OUT_OF_THE_KITCHEN			"ne restez pas là...."

// credits.c
#define	GAME_COMPLETE1				"FELICITATIONS"
#define	GAME_COMPLETE2				"Vous avez terminé tous les"
#define	GAME_COMPLETE3				"niveaux."

#define	COMPLETE_NOT_ALL_CRYSTALS1	"mais vous n'avez pas obtenu"
#define	COMPLETE_NOT_ALL_CRYSTALS2	"tous les cristaux."
#define COMPLETE_NOT_ALL_CRYSTALS3	"Vous devez recommencer"
#define COMPLETE_NOT_ALL_CRYSTALS4	"si vous voulez être"
#define COMPLETE_NOT_ALL_CRYSTALS5	"correctement récompensé."

#define	COMPLETE_ALL_CRYSTALS1		"et ramassé tous les"
#define	COMPLETE_ALL_CRYSTALS2		"cristaux."
#define	COMPLETE_ALL_CRYSTALS3		"Vous avez reçu"
#define	COMPLETE_ALL_CRYSTALS4		"le biker secret."

#define	COMPLETE_ALLBUT1_CRYSTALS1	"vous avez terminé le"
#define	COMPLETE_ALLBUT1_CRYSTALS2	"niveau secret."
#define	COMPLETE_ALLBUT1_CRYSTALS3	"mais vous n'avez pas le"
#define	COMPLETE_ALLBUT1_CRYSTALS4	"dernier cristal. Vous ferez"
#define	COMPLETE_ALLBUT1_CRYSTALS5	"mieux la prochaine fois."

#define	PRESS_ANY_KEY1				"App."
#define	PRESS_ANY_KEY2				"sur une touche"

#define DEFAULT_NEWNAME "nom"
