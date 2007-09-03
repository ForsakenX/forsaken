/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Includes
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#include <stdio.h>
#include <stdarg.h>
#include <stdio.h>
#include "typedefs.h"
#include "New3D.h"
#include "typedefs.h"
#include <dplay.h>
#include "new3d.h"
#include "quat.h"
#include "CompObjects.h"
#include "bgobjects.h"
#include "Object.h"
#include "mydplay.h"
#include "ships.h"
#include "main.h"
#include "Title.h"
#include "Text.h"
#include "screenpolys.h"
#include "credits.h"
#include "local.h"
#include "XMem.h"

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Defines
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
#define		BIGFONT_YSIZE		22
#define		BIGFONT_CHAR_GAP	2
#define		BIGFONT_SPACESIZE	12
#define		BIGFONT_OFFSET		0
#define		SMALLFONT_YSIZE		16
#define		SMALLFONT_CHAR_GAP	1
#define		SMALLFONT_SPACESIZE	8
#define		SMALLFONT_OFFSET	1

#define		BIGFONT				0
#define		SMALLFONT			1
#define		FORSAKEN_OFFSET		0

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	External Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
extern	float			framelag;
extern	VECTOR			Forward;
extern	VECTOR			Backward;
extern	VECTOR			SlideUp;
extern	VECTOR			SlideDown;
extern	VECTOR			SlideLeft;
extern	VECTOR			SlideRight;
extern	MATRIX			MATRIX_Identity;
extern	SCRPOLY			ScrPolys[ MAXNUMOFSCRPOLYS ];
extern	FRAME_INFO	*	CreditsFont_Header;
extern	CAMERA			CurrentCamera;
extern	int				GameCompleted;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Global Variables
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/

uint8	CharTable[] = {

	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 0 - 9
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 10 - 19
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 20 - 29
	0, 0, 0,37,38,39,40,41,45,70,		// 30 - 39
   42,43, 0, 0,71,46,47, 0,26,27,		// 40 - 49
   28,29,30,31,32,33,34,35, 0, 0,		// 50 - 59
   52,54,53,55,56, 0, 1, 2, 3, 4,		// 60 - 69
	5, 6, 7, 8, 9,10,11,12,13,14,		// 70 - 79
   15,16,17,18,19,20,21,22,23,24,		// 80 - 89
   25, 0, 0, 0, 0, 0, 0, 0, 1, 2,		// 90 - 99
	3, 4, 5, 6, 7, 8, 9,10,11,12,		// 100 - 109
   13,14,15,16,17,18,19,20,21,22,		// 110 - 119
   23,24,25, 0, 0, 0, 0, 0, 0, 0,		// 120 - 129
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 130 - 139
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 140 - 149
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 150 - 159
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 160 - 169
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 170 - 179
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 180 - 189
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 190 - 199
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 200 - 209
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 210 - 219
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 220 - 229
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 230 - 239
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// 240 - 249
    0, 0, 0, 0, 0, 0,					// 250 - 255
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Programming Credits
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
CREDIT	ProbeTitle = { "PROBE DEVELOPMENT TEAM", 1, NULL };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Programming Credits
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ProgrammingNames[] = { "Dave Collins", "Dan Phillips", "Oliver Castle", "Phil Yard" };
PAGE	ProgrammingPages[] = { { 4, &ProgrammingNames[ 0 ] } };
CREDIT	Programming = { "PROGRAMMING", 1, &ProgrammingPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Software Programming Credit
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	SoftwareProgrammingNames[] = { "Chris Walsh" };
PAGE	SoftwareProgrammingPages[] = { { 1, &SoftwareProgrammingNames[ 0 ] } };
CREDIT	SoftwareProgramming = { "SOFTWARE RENDERER", 1, &SoftwareProgrammingPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Development Director Credit
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	DevelopmentDirectorNames[] = { "Tony Beckwith" };
PAGE	DevelopmentDirectorPages[] = { { 1, &DevelopmentDirectorNames[ 0 ] } };
CREDIT	DevelopmentDirector = { "DEVELOPMENT DIRECTOR", 1, &DevelopmentDirectorPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Project Manager Credit
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ProjectManagerNames[] = { "James Stewart" };
PAGE	ProjectManagerPages[] = { { 1, &ProjectManagerNames[ 0 ] } };
CREDIT	ProjectManager = { "PROJECT MANAGER", 1, &ProjectManagerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Artwork
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ArtworkNames[] = { "Ned Langman", "Anthony Rosbottom", "Daniel Burke" };
PAGE	ArtworkPages[] = { { 3, &ArtworkNames[ 0 ] } };
CREDIT	Artwork = { "ARTWORK", 1, &ArtworkPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Level Creation
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	LevelCreationkNames1[] = { "Nick Baynes", "Errol Gale", "Ned Langman", "Paul Sinton" };
uint8 *	LevelCreationkNames2[] = { "Chris Baker", "Nick McGee", "Ian Hudson", "Neil Gartell" };
uint8 *	LevelCreationkNames3[] = { "Tom Geddes", "Alec Prenter", "Kevin Row", "Andy Williams" };
uint8 *	LevelCreationkNames4[] = { "Adam Bouskill", "Andrew Cowe", "Craig Kerrison", "Anthony Rosbottom", "Ian Lovett" };

PAGE	LevelCreationPages[] = {
	{ 4, &LevelCreationkNames1[ 0 ] },
	{ 4, &LevelCreationkNames2[ 0 ] },
	{ 4, &LevelCreationkNames3[ 0 ] },
	{ 5, &LevelCreationkNames4[ 0 ] } };

CREDIT	LevelCreation = { "LEVEL CREATION", 4, &LevelCreationPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Concept Artwork
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ConceptArtworkNames[] = { "Jeremy Elford", "Alex Lawrence" };
PAGE	ConceptArtworkPages[] = { { 2, &ConceptArtworkNames[ 0 ] } };
CREDIT	ConceptArtwork = { "CONCEPT ARTWORK", 1, &ConceptArtworkPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Music
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	MusicNames[] = { "\"The Swarm\"", "Stephen Root", "Dominic Glynn" };
PAGE	MusicPages[] = { { 3, &MusicNames[ 0 ] } };
CREDIT	Music = { "MUSIC CREATED BY", 1, &MusicPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	SFX Design
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	SFX_DesignNames[] = { "Andy Brock" };
PAGE	SFX_DesignPages[] = { { 1, &SFX_DesignNames[ 0 ] } };
CREDIT	SFX_Design = { "SFX DESIGN", 1, &SFX_DesignPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	FMV Audio
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	SFX_FMVAudioNames[] = { "Stephen Root", "Andy Brock", "Simon Robertson" };
PAGE	SFX_FMVAudioPages[] = { { 3, &SFX_FMVAudioNames[ 0 ] } };
CREDIT	SFX_FMVAudio = { "FMV AUDIO", 1, &SFX_FMVAudioPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Probe Voice Talents
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ProbeVoicesNames[] = { "Brenda O'Farrell as \"BRENDA\"", "Kate O'Rourke as \"LEPRACOM\"" };
PAGE	ProbeVoicesPages[] = { { 2, &ProbeVoicesNames[ 0 ] } };
CREDIT	ProbeVoices = { "VOICE TALENTS", 1, &ProbeVoicesPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Voice Processing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	VoiceProcessingNames[] = { "Andy Brock" };
PAGE	VoiceProcessingPages[] = { { 1, &VoiceProcessingNames[ 0 ] } };
CREDIT	VoiceProcessing = { "VOICE PROCESSING", 1, &VoiceProcessingPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Habitat
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	HabitatNames[] = { "Paul Sinnet", "Bob Armour", "Harvey Gilpin" };
PAGE	HabitatPages[] = { { 3, &HabitatNames[ 0 ] } };
CREDIT	Habitat = { "WORLD TOOL \"HABITAT\"", 1, &HabitatPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Tools
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	ToolsNames[] = { "Oliver Castle", "Tony Monkton", "Dave Collins" };
PAGE	ToolsPages[] = { { 3, &ToolsNames[ 0 ] } };
CREDIT	Tools = { "TOOLS", 1, &ToolsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	QA Manager
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	QA_ManagerNames[] = { "Clifford Ramsey" };
PAGE	QA_ManagerPages[] = { { 1, &QA_ManagerNames[ 0 ] } };
CREDIT	QA_Manager = { "QA MANAGER", 1, &QA_ManagerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	QA Department
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	QA_DepartmentNames[] = { "Richard Foster", "Mitchell Slater", "Scott Priestly",
								 "Geoff Cullen", "Andy Aish" };
PAGE	QA_DepartmentPages[] = { { 5, &QA_DepartmentNames[ 0 ] } };
CREDIT	QA_Department = { "QA DEPARTMENT", 1, &QA_DepartmentPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Special Thanks
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	SpecialThanksNames[] = { "Greg Michael", "Tony Monckton", "Mark Fisher", "James Hawkins", "Emma Denson" };
PAGE	SpecialThanksPages[] = { { 5, &SpecialThanksNames[ 0 ] } };
CREDIT	SpecialThanks = { "SPECIAL THANKS", 1, &SpecialThanksPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Title
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
CREDIT	AcclaimTitle = { "ACCLAIM PRODUCT DEVELOPMENT", 1, NULL };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Producer
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccProducerNames[] = { "Shawn David Rosen" };
PAGE	AccProducerPages[] = { { 1, &AccProducerNames[ 0 ] } };
CREDIT	AccProducer = { "PRODUCER", 1, &AccProducerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Lead Analysts
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccLeadAnalystsNames[] = { "James \"China White\" Craddock", "Michael \"Lenin\" Patterson" };
PAGE	AccLeadAnalystsPages[] = { { 2, &AccLeadAnalystsNames[ 0 ] } };
CREDIT	AccLeadAnalysts = { "LEAD ANALYSTS", 1, &AccLeadAnalystsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Project Leads
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccProjectLeadsNames[] = { "Jeremy \"Silkworm\" Pope", "Matt \"Daddy\" Canonico" };
PAGE	AccProjectLeadsPages[] = { { 2, &AccProjectLeadsNames[ 0 ] } };
CREDIT	AccProjectLeads = { "PROJECT LEADS", 1, &AccProjectLeadsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Testers
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccTestersNames1[] = { "Russell \"Nani\" Ballenger", "Paul \"Wasabi\" DiCarlo", "James \"Velvets\" Ackermann", "Joe \"Redwing\" Howell" }; 
uint8 *	AccTestersNames2[] = { "Mark \"Gyoza\" Garone", "Sue \"Saucy\" Elliot", "Bonchi \"Ill\" Martinez", "Matthew \"Coach\" McEnerney" }; 
uint8 *	AccTestersNames3[] = { "Ted Kaplan", "Dan \"X\" Badilla", "Mike \"Sideshow\" Sterzel", "Jen \"Dr. Evil\" Walka" }; 
uint8 *	AccTestersNames4[] = { "Adam \"YTYTY\" Rosen", "Martin \"Razor\" Glantz", "Dave \"Dishwalla\" Walowitz", "Chris \"Crusher\" Jean", "Brian \"Gouki\" Regan"}; 
PAGE	AccTestersPages[] = { { 4, &AccTestersNames1[ 0 ] }, { 4, &AccTestersNames2[ 0 ] }, { 4, &AccTestersNames3[ 0 ] }, { 5, &AccTestersNames4[ 0 ] } };
CREDIT	AccTesters = { "TESTERS", 4, &AccTestersPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Marketing Product Manager
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccMarketingProductManagerNames[] = { "Donald Jackson" };
PAGE	AccMarketingProductManagerPages[] = { { 1, &AccMarketingProductManagerNames[ 0 ] } };
CREDIT	AccMarketingProductManager = { "MARKETING PRODUCT MANAGER", 1, &AccMarketingProductManagerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Assistant Product Manager
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccAssistantProductManagerNames[] = { "Ed Fortune" };
PAGE	AccAssistantProductManagerPages[] = { { 1, &AccAssistantProductManagerNames[ 0 ] } };
CREDIT	AccAssistantProductManager = { "ASSISTANT PRODUCT MANAGER", 1, &AccAssistantProductManagerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim UK Marketing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccUKMarketingNames[] = { "Frank Sagnier", "Theresa Noonam", "Ned Browning", "Vincent Mercier", "Rainer Zipp" };
PAGE	AccUKMarketingPages[] = { { 5, &AccUKMarketingNames[ 0 ] } };
CREDIT	AccUKMarketing = { "UK MARKETING", 1, &AccUKMarketingPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Creative Services
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccCreativeServicesNames[] = { "Scott Jenkins", "Lenny O'Donnell", "Beth Pawluk" };
PAGE	AccCreativeServicesPages[] = { { 3, &AccCreativeServicesNames[ 0 ] } };
CREDIT	AccCreativeServices = { "CREATIVE SERVICES", 1, &AccCreativeServicesPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Manager Internet Services
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccInternetManagerNames[] = { "Ben Fischbach" };
PAGE	AccInternetManagerPages[] = { { 1, &AccInternetManagerNames[ 0 ] } };
CREDIT	AccInternetManager = { "INTERNET SERVICES MANAGER", 1, &AccInternetManagerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim WebMaster
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccWebmasterNames[] = { "Bobby Saha" };
PAGE	AccWebmasterPages[] = { { 1, &AccWebmasterNames[ 0 ] } };
CREDIT	AccWebmaster = { "WEBMASTER", 1, &AccWebmasterPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Forsaken Manuals
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccForsakenManualsNames[] = { "Bill Dickson" };
PAGE	AccForsakenManualsPages[] = { { 1, &AccForsakenManualsNames[ 0 ] } };
CREDIT	AccForsakenManuals = { "MANUALS", 1, &AccForsakenManualsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim PC Tech Supplement
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccPCTechSupplementNames[] = { "Howard Perlman", "David Pollick" };
PAGE	AccPCTechSupplementPages[] = { { 2, &AccPCTechSupplementNames[ 0 ] } };
CREDIT	AccPCTechSupplement = { "PC TECH SUPPLEMENT", 1, &AccPCTechSupplementPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Forsaken Strategy Guide
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccStrategyGuideNames1[] = { "Jeff Gomez", "Joe Caponsacco", "Alana Zdinak", "Dan Garcia" };
uint8 *	AccStrategyGuideNames2[] = { "Matthew Canonico", "Jeremy Pope", "Alex Glass" };
PAGE	AccStrategyGuidePages[] = { { 4, &AccStrategyGuideNames1[ 0 ] }, { 3, &AccStrategyGuideNames2[ 0 ] } };
CREDIT	AccStrategyGuide = { "STRATEGY GUIDES", 2, &AccStrategyGuidePages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Forskan Public Relations
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccPublicRelationsNames[] = { "Kathryn Morris", "Michael Meyers" };
PAGE	AccPublicRelationsPages[] = { { 2, &AccPublicRelationsNames[ 0 ] } };
CREDIT	AccPublicRelations = { "PUBLIC RELATIONS", 1, &AccPublicRelationsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim UK Public Relations
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccUKPublicRelationsNames1[] = { "Simon Smith-Wright", "Reza Memari", "Sigfrido Buttner-Garcia" };
uint8 *	AccUKPublicRelationsNames2[] = { "Alex Ward", "Stefan Luludes", "Stephanie Bolla" };
PAGE	AccUKPublicRelationsPages[] = { { 3, &AccUKPublicRelationsNames1[ 0 ] }, { 3, &AccUKPublicRelationsNames2[ 0 ] } };
CREDIT	AccUKPublicRelations = { "UK PUBLIC RELATIONS", 2, &AccUKPublicRelationsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Translations
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccTranslationsNames[] = { "Reza Memari", "Vincent Mercier", "Phil Brannelly", "Sigfrido Buttner-Garcia" };
PAGE	AccTranslationsPages[] = { { 4, &AccTranslationsNames[ 0 ] } };
CREDIT	AccTranslations = { "TRANSLATIONS", 1, &AccTranslationsPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim European Product Coordinator
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccEuroProductCoordinatorNames[] = { "Harvey Elliott" };
PAGE	AccEuroProductCoordinatorPages[] = { { 1, &AccEuroProductCoordinatorNames[ 0 ] } };
CREDIT	AccEuroProductCoordinator = { "EUROPEAN PRODUCT COORDINATOR", 1, &AccEuroProductCoordinatorPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Technical Support Group
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccTSGNames1[] = { "Harry \"FilthyEars\" Reimer", "Howard \"Ex-Kane\" Perlman", "Kevin Denehy", "Robert \"Moish\" Coffey", "Andrew Fullaytor" };
uint8 *	AccTSGNames2[] = { "Greg \"BigDaddy\" McGovern", "Ulises \"Pana-san\" Batalla", "David \"HooVooLoo\" Pollick", "Leigh \"Frodo\" Busch", "Andy Scalka" };
PAGE	AccTSGPages[] = { { 5, &AccTSGNames1[ 0 ] }, { 5, &AccTSGNames2[ 0 ] } };
CREDIT	AccTSG = { "TECHNICAL SUPPORT GROUP", 2, &AccTSGPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Translations
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccPCSoftwareEngineerNames[] = { "Angel Cordero" };
PAGE	AccPCSoftwareEngineerPages[] = { { 1, &AccPCSoftwareEngineerNames[ 0 ] } };
CREDIT	AccPCSoftwareEngineer = { "PC SOFTWARE ENGINEER", 1, &AccPCSoftwareEngineerPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Voice Talents
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
CREDIT	AccVoices = { "VOICE TALENTS", 1, NULL };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Voice Talents of ProMotions
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccProMotionNames1[] = { "Lani Minella as", "",
								"Nubia, Nim Soo Sun,",
								"Lokasenna, Flygirl,",
								"Foetoid, Mephistofun",
								"and LANI-1" };

uint8 *	AccProMotionNames2[] = { "Jon St.John as", "",
								"Earl Sleek, Cuvel Clark,",
								"Septer, Cerbero and",
								"Roadster" };

uint8 *	AccProMotionNames3[] = { "Steve Brodie as", "",
								"Rex Hardy and Phil-3B" };

uint8 *	AccProMotionNames4[] = { "Rick Calvert as", "",
								"Beard, Ex-Cop and",
								"Dr. Nepenthe" };

uint8 *	AccProMotionNames5[] = { "Chris Wilcox as", "",
								"L.A. Jay and HK5" };

uint8 *	AccProMotionNames6[] = { "and Bill Corkery as Jo" };

uint8 *	AccProMotionNames7[] = { "Casting and Direction by", "",
								 "Lani Minella" };

PAGE	AccProMotionPages[] = { { 6, &AccProMotionNames1[ 0 ] },
								{ 5, &AccProMotionNames2[ 0 ] },
								{ 4, &AccProMotionNames3[ 0 ] },
								{ 4, &AccProMotionNames4[ 0 ] },
								{ 3, &AccProMotionNames5[ 0 ] },
								{ 1, &AccProMotionNames6[ 0 ] },
								{ 3, &AccProMotionNames7[ 0 ] } };

CREDIT	AccProMotion = { "PRO MOTIONS PRODUCTIONS", 6, &AccProMotionPages[ 0 ] };

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Acclaim Special Thanks
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	AccSpecialThanksNames1[] = { "Angel Cordero", "Col Stone", "Brett Gow", "Paul Eibler", "Roger Booth" };
uint8 *	AccSpecialThanksNames2[] = { "Carol Caracciolo", "Jeff Rosa", "Dale Taylor", "Mike \"Descentile\" Vitelli", "Roberto \"R2D298\" Toledo" };
uint8 *	AccSpecialThanksNames3[] = { "Don \"DrDon\" Tulloch", "Chris \"Glock21\" Cacioppo", "Jim \"Xciter\" Springer", "Andrew \"Peartman\" Peart", "Ron \"Vipe!\" Darge" };
uint8 *	AccSpecialThanksNames4[] = { "Dion \"Centaur\" Hopkins", "Kyle \"Kiln\" Johnson", "Chad \"Zepplin\" Smith", "Anthony \"Cyborg\"", "Joe \"Baywatch\" Fargo" };
uint8 *	AccSpecialThanksNames5[] = { "Scott \"Delcio\" Delcio", "Banning \"Spaz\"", "Joe \"Karash\" Brown", "Chris \"Gwar\" Ledwith", "Josh \"watSup\" Vaughn" };
uint8 *	AccSpecialThanksNames6[] = { "Steve \"HolyGhost\" Leonard", "Jeremy \"Psion\"", "Joshua \"JackHammer\" Haley", "Master \"Sirian\"", "\"Redlion\"" };
uint8 *	AccSpecialThanksNames7[] = { "Jan \"DrDust\"", "Steve \"Koolbear\"", "Matt Ployhar", "Jim Clardy", "Brett Schneth" };
uint8 *	AccSpecialThanksNames8[] = { "Kevin Shue", "Lisa Solomon", "Jason Delerocca", "Jay Eisenlohr", "Chris Dunphy" };
uint8 *	AccSpecialThanksNames9[] = { "Justin Cooney", "Patrick Hewitt", "Brien Bruning", "Andrew Bergstein", "Savannah Kimble" };
uint8 *	AccSpecialThanksNames10[] = { "Kevin Wagner", "Kevin Spross", "Steve Cox", "Bill Havlicek", "Jay Cotton" };
uint8 *	AccSpecialThanksNames11[]= { "Chris Dunphy", "James Daly", "Lani Minella" };

PAGE	AccSpecialThanksPages[] = { { 5, &AccSpecialThanksNames1[ 0 ] }, { 5, &AccSpecialThanksNames2[ 0 ] },
									{ 5, &AccSpecialThanksNames3[ 0 ] }, { 5, &AccSpecialThanksNames4[ 0 ] },
									{ 5, &AccSpecialThanksNames5[ 0 ] }, { 5, &AccSpecialThanksNames6[ 0 ] },
									{ 5, &AccSpecialThanksNames7[ 0 ] }, { 5, &AccSpecialThanksNames8[ 0 ] },
									{ 5, &AccSpecialThanksNames9[ 0 ] }, { 5, &AccSpecialThanksNames10[ 0 ] },
									{ 3, &AccSpecialThanksNames11[ 0 ] } };

CREDIT	AccSpecialThanks = { "SPECIAL THANKS", 11, &AccSpecialThanksPages[ 0 ] };

CREDIT	*	Credits[] = {

	&ProbeTitle,
	&Programming,
	&SoftwareProgramming,
	&DevelopmentDirector,
	&ProjectManager,
	&Artwork,
	&LevelCreation,
	&ConceptArtwork,
	&Music,
	&SFX_Design,
	&SFX_FMVAudio,
	&ProbeVoices,
	&VoiceProcessing,
	&Habitat,
	&Tools,
	&QA_Manager,
	&QA_Department,
	&SpecialThanks,

	&AcclaimTitle,
	&AccProducer,
	&AccLeadAnalysts,
	&AccProjectLeads,
	&AccTesters,
	&AccMarketingProductManager,
	&AccAssistantProductManager,
	&AccUKMarketing,
	&AccCreativeServices,
	&AccInternetManager,
	&AccWebmaster,
	&AccForsakenManuals,
	&AccPCTechSupplement,
	&AccStrategyGuide,
	&AccPublicRelations,
	&AccUKPublicRelations,
	&AccTranslations,
	&AccEuroProductCoordinator,
	&AccTSG,
	&AccPCSoftwareEngineer,
	&AccVoices,
	&AccProMotion,
	&AccSpecialThanks,
	NULL
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Game Complete Messages ( Without All Crystals )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	GameComplete1Names1[] = { GAME_COMPLETE2, GAME_COMPLETE3 };
uint8 *	GameComplete1Names2[] = { COMPLETE_NOT_ALL_CRYSTALS1, COMPLETE_NOT_ALL_CRYSTALS2 };
uint8 *	GameComplete1Names3[] = { COMPLETE_NOT_ALL_CRYSTALS3, COMPLETE_NOT_ALL_CRYSTALS4,
								  COMPLETE_NOT_ALL_CRYSTALS5 };
PAGE	GameComplete1Pages[] = { { 2, &GameComplete1Names1[ 0 ] }, { 2, &GameComplete1Names2[ 0 ] },
								 { 3, &GameComplete1Names3[ 0 ] } };

CREDIT	GameComplete1 = { GAME_COMPLETE1, 3, &GameComplete1Pages[ 0 ] };

CREDIT	*	Credits1[] = {

	&GameComplete1,
	NULL
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Game Complete Messages ( With All Crystals )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	GameComplete2Names1[] = { GAME_COMPLETE2, GAME_COMPLETE3 };
uint8 *	GameComplete2Names2[] = { COMPLETE_ALL_CRYSTALS1, COMPLETE_ALL_CRYSTALS2 };
uint8 *	GameComplete2Names3[] = { COMPLETE_ALL_CRYSTALS3, "\"Flygirl\".",
								  COMPLETE_ALL_CRYSTALS4 };
PAGE	GameComplete2Pages[] = { { 2, &GameComplete2Names1[ 0 ] }, { 2, &GameComplete2Names2[ 0 ] },
								 { 3, &GameComplete2Names3[ 0 ] } };

CREDIT	GameComplete2 = { GAME_COMPLETE1, 3, &GameComplete2Pages[ 0 ] };

CREDIT	*	Credits2[] = {

	&GameComplete2,
	NULL
};

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Game Complete Messages ( With All Crystals except last )
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
uint8 *	GameComplete3Names1[] = { COMPLETE_ALLBUT1_CRYSTALS1, COMPLETE_ALLBUT1_CRYSTALS2 };
uint8 *	GameComplete3Names2[] = { COMPLETE_ALLBUT1_CRYSTALS3, COMPLETE_ALLBUT1_CRYSTALS4, COMPLETE_ALLBUT1_CRYSTALS5 };
PAGE	GameComplete3Pages[] = { { 2, &GameComplete3Names1[ 0 ] }, { 3, &GameComplete3Names2[ 0 ] } };

CREDIT	GameComplete3 = { GAME_COMPLETE1, 2, &GameComplete3Pages[ 0 ] };

CREDIT	*	Credits3[] = {

	&GameComplete3,
	NULL
};

static	CREDIT	**	CreditsPtr = &Credits[ 0 ];
static	int16	CurrentCredit = 0;
static	int16	CurrentPage = 0;
static	float	Delay = ( 3.0F * ANIM_SECOND );
static	int16	CreditMode = CREDIT_INITIALPAUSE;
static	float	TitleRed = 0.0F;
static	float	TitleGreen = 0.0F;
static	float	TitleBlue = 0.0F;
static	float	TitleTrans = 0.0F;
static	float	PageRed = 0.0F;
static	float	PageGreen = 0.0F;
static	float	PageBlue = 0.0F;
static	float	PageTrans = 0.0F;
static	BOOL	CreditsEnabled = FALSE;

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Setup Credits
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SetupCredits( void )
{
	CreditsEnabled = TRUE;
	Delay = ( 3.0F * ANIM_SECOND );
	CreditMode = CREDIT_INITIALPAUSE;
	CreditsPtr = &Credits[ 0 ];

#if 0
	if( CreditsPtr == &Credits[ 0 ] )
	{
		CreditsPtr = &Credits1[ 0 ];
		return;
	}

	if( CreditsPtr == &Credits1[ 0 ] )
	{
		CreditsPtr = &Credits2[ 0 ];
		return;
	}

	if( CreditsPtr == &Credits2[ 0 ] )
	{
		CreditsPtr = &Credits3[ 0 ];
		return;
	}

	if( CreditsPtr == &Credits3[ 0 ] )
	{
		CreditsPtr = &Credits[ 0 ];
		return;
	}
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Setup Credits
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SetupGameCompleteCredits( void )
{
	CreditsEnabled = TRUE;
	Delay = ( 3.0F * ANIM_SECOND );
	CreditMode = CREDIT_INITIALPAUSE;

	switch( GameCompleted )
	{
		case GAMECOMPLETE_WithoutAllCrystals:
			CreditsPtr = &Credits1[ 0 ];
			break;

		case GAMECOMPLETE_WithAllCrystals:
			CreditsPtr = &Credits2[ 0 ];
			break;

		case GAMECOMPLETE_WithAllCrystalsExceptLast:
			CreditsPtr = &Credits3[ 0 ];
			break;

		case GAMECOMPLETE_NotComplete:
		case GAMECOMPLETE_GotAllCrystalsSoFar:
		case GAMECOMPLETE_Cheated:
		case GAMECOMPLETE_AdditionalLevels:
			break;

		default:
			CreditsPtr = &Credits1[ 0 ];
			break;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Credits
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayCredits( void )
{
	PAGE	*	FirstPage;
	uint8	**	NamesArray;
	int16		Xpos, Ypos;
	int16		Count;
	float		Step;
	float		Step2;

	if( !CreditsEnabled ) return;
	if( CreditsPtr[ CurrentCredit ] == NULL ) return;

	switch( CreditMode )
	{
		case CREDIT_INITIALPAUSE:
			Delay -= framelag;

			if( Delay < 0.0F )
			{
				CurrentCredit = 0;
				CurrentPage = 0;
				TitleRed = 0.0F;
				TitleGreen = 0.0F;
				TitleBlue = 0.0F;
				TitleTrans = 0.0F;
				PageRed = 0.0F;
				PageGreen = 0.0F;
				PageBlue = 0.0F;
				PageTrans = 0.0F;
				Delay = ( 0.5F * ANIM_SECOND );
				CreditMode = CREDIT_FADEINTITLE;
			}
			break;

		case CREDIT_FADEINTITLE:
			Step = ( framelag * 8.5F );
			Step2 = ( framelag * 3.4F );
			FadeColourFloat( &TitleRed, 255.0F, Step );
			FadeColourFloat( &TitleGreen, 255.0F, Step );
			FadeColourFloat( &TitleBlue, 255.0F, Step );
			FadeColourFloat( &TitleTrans, 255.0F, Step );

			FadeColourFloat( &PageRed, 0.0F, Step2 );
			FadeColourFloat( &PageGreen, 255.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step2 );
			FadeColourFloat( &PageTrans, 255.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 4.0F * ANIM_SECOND );
				CreditMode = CREDIT_PAGEPAUSE;
			}
			break;

		case CREDIT_FADEINPAGE:
			Step = ( framelag * 8.5F );
			Step2 = ( framelag * 3.4F );
			FadeColourFloat( &PageRed, 0.0F, Step2 );
			FadeColourFloat( &PageGreen, 255.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step2 );
			FadeColourFloat( &PageTrans, 255.0F, Step );
  
			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 4.0F * ANIM_SECOND );
				CreditMode = CREDIT_PAGEPAUSE;
			}
			break;

		case CREDIT_FADEOUTPAGE:
			Step = ( framelag * 4.5F );
			FadeColourFloat( &PageRed, 0.0F, Step );
			FadeColourFloat( &PageGreen, 0.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step );
			FadeColourFloat( &PageTrans, 0.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 0.25F * ANIM_SECOND );
				CreditMode = CREDIT_BLANKPAUSE;
			}
			break;

		case CREDIT_FADEOUTTITLE:
			Step = ( framelag * 4.5F );
			FadeColourFloat( &TitleRed, 0.0F, Step );
			FadeColourFloat( &TitleGreen, 0.0F, Step );
			FadeColourFloat( &TitleBlue, 0.0F, Step );
			FadeColourFloat( &TitleTrans, 0.0F, Step );

			FadeColourFloat( &PageRed, 0.0F, Step );
			FadeColourFloat( &PageGreen, 0.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step );
			FadeColourFloat( &PageTrans, 0.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 0.25F * ANIM_SECOND );
				CreditMode = CREDIT_BLANKPAUSE;
			}
			break;

		case CREDIT_PAGEPAUSE:
			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 1.0F * ANIM_SECOND );

				if( CurrentPage == ( CreditsPtr[ CurrentCredit ]->NumPages - 1 ) )
				{ 
					CreditMode = CREDIT_FADEOUTTITLE;
				}
				else
				{
					CreditMode = CREDIT_FADEOUTPAGE;
				}
			}
			break;

		case CREDIT_BLANKPAUSE:
			Delay -= framelag;

			if( Delay < 0.0F )
			{
				Delay = ( 0.5F * ANIM_SECOND );

				if( CurrentPage == ( CreditsPtr[ CurrentCredit ]->NumPages - 1 ) )
				{
					if( !CreditsPtr[ CurrentCredit + 1 ] )
					{
						Delay = ( 60.0F * ANIM_SECOND );
						CreditMode = CREDIT_INITIALPAUSE;
					}
					else
					{
						CreditMode = CREDIT_FADEINTITLE;
						CurrentPage = 0;
						CurrentCredit++;
					}
				}
				else
				{
					CreditMode = CREDIT_FADEINPAGE;
					CurrentPage++;
				}
			}
			break;
	}

	if( CreditMode != CREDIT_INITIALPAUSE )
	{
		if( CreditsPtr[ CurrentCredit ]->FirstPage )
		{
			Xpos = (int16) ( CurrentCamera.Viewport.dwWidth / 2.0F );
			Ypos = (int16) ( CurrentCamera.Viewport.dwHeight / 6.0F );
		}
		else
		{
			Xpos = (int16) ( CurrentCamera.Viewport.dwWidth / 2.0F );
			Ypos = (int16) ( CurrentCamera.Viewport.dwHeight / 4.0F );
		}

		CenterPrintString( SMALLFONT, Xpos, Ypos, CreditsPtr[ CurrentCredit ]->TitleString,
						 (uint8) TitleRed, (uint8) TitleGreen, (uint8) TitleBlue, (uint8) TitleTrans );

		Ypos += ( BIGFONT_YSIZE * 3 );

		FirstPage = ( CreditsPtr[ CurrentCredit ]->FirstPage );

		if( FirstPage )
		{
			NamesArray = FirstPage[ CurrentPage ].Names;

			for( Count = 0; Count < FirstPage[ CurrentPage ].NumNames; Count++ )
			{ 
				CenterPrintString( SMALLFONT, Xpos, Ypos, NamesArray[ Count ],
							(uint8) PageRed, (uint8) PageGreen, (uint8) PageBlue, (uint8) PageTrans );
				Ypos += SMALLFONT_YSIZE;
			}
		}
	}

#if 0
	Xpos = (int16) ( CurrentCamera.Viewport.dwWidth / 2.0F );
	Ypos = (int16) ( CurrentCamera.Viewport.dwHeight - ( SMALLFONT_YSIZE * 2.0F ) );
	CenterPrintString( SMALLFONT, Xpos, Ypos, "àáäâåèéëêìíïîòóöôùúüûñç", 255, 255, 255, 255 );
	Ypos += SMALLFONT_YSIZE;
	CenterPrintString( SMALLFONT, Xpos, Ypos, "ÀÁÄÂÅÈÉËÊÌÍÏÎÒÓÖÔÙÚÜÛÑÇ¿", 255, 255, 255, 255 );
#endif
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Setup ForsakenAnyKey functions
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SetupForsakenAnyKey( void )
{
	Delay = ( 3.0F * ANIM_SECOND );
	CreditMode = CREDIT_INITIALPAUSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Credits
	Input		:	Nothing
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayForsakenAnyKey( void )
{
	int16		Xpos, Ypos;
	float		Step;

	switch( CreditMode )
	{
		case CREDIT_INITIALPAUSE:
			Delay -= framelag;

			if( Delay < 0.0F )
			{
				CurrentCredit = 0;
				CurrentPage = 0;
				TitleRed = 0.0F;
				TitleGreen = 0.0F;
				TitleBlue = 0.0F;
				TitleTrans = 0.0F;
				PageRed = 0.0F;
				PageGreen = 0.0F;
				PageBlue = 0.0F;
				PageTrans = 0.0F;
				Delay = ( 0.5F * ANIM_SECOND );
				CreditMode = CREDIT_FADEINTITLE;
			}
			break;

		case CREDIT_FADEINTITLE:
			Step = ( framelag * 8.5F );
			FadeColourFloat( &TitleRed, 255.0F, Step );
			FadeColourFloat( &TitleGreen, 255.0F, Step );
			FadeColourFloat( &TitleBlue, 255.0F, Step );
			FadeColourFloat( &TitleTrans, 255.0F, Step );

			FadeColourFloat( &PageRed, 255.0F, Step );
			FadeColourFloat( &PageGreen, 255.0F, Step );
			FadeColourFloat( &PageBlue, 255.0F, Step );
			FadeColourFloat( &PageTrans, 255.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 2.0F * ANIM_SECOND );
				CreditMode = CREDIT_PAGEPAUSE;
			}
			break;

		case CREDIT_FADEINPAGE:
			Step = ( framelag * 8.5F );
			FadeColourFloat( &PageRed, 255.0F, Step );
			FadeColourFloat( &PageGreen, 255.0F, Step );
			FadeColourFloat( &PageBlue, 255.0F, Step );
			FadeColourFloat( &PageTrans, 255.0F, Step );
  
			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 2.0F * ANIM_SECOND );
				CreditMode = CREDIT_PAGEPAUSE;
			}
			break;

		case CREDIT_FADEOUTPAGE:
			Step = ( framelag * 4.25F );
			FadeColourFloat( &PageRed, 0.0F, Step );
			FadeColourFloat( &PageGreen, 0.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step );
			FadeColourFloat( &PageTrans, 0.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 0.25F * ANIM_SECOND );
				CreditMode = CREDIT_BLANKPAUSE;
			}
			break;

		case CREDIT_FADEOUTTITLE:
			Step = ( framelag * 4.25F );
			FadeColourFloat( &TitleRed, 0.0F, Step );
			FadeColourFloat( &TitleGreen, 0.0F, Step );
			FadeColourFloat( &TitleBlue, 0.0F, Step );
			FadeColourFloat( &TitleTrans, 0.0F, Step );

			FadeColourFloat( &PageRed, 0.0F, Step );
			FadeColourFloat( &PageGreen, 0.0F, Step );
			FadeColourFloat( &PageBlue, 0.0F, Step );
			FadeColourFloat( &PageTrans, 0.0F, Step );

			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 0.25F * ANIM_SECOND );
				CreditMode = CREDIT_BLANKPAUSE;
			}
			break;

		case CREDIT_PAGEPAUSE:
			Delay -= framelag;
			if( Delay < 0.0F )
			{
				Delay = ( 1.0F * ANIM_SECOND );
				CreditMode = CREDIT_FADEOUTPAGE;
			}
			break;

		case CREDIT_BLANKPAUSE:
			Delay -= framelag;

			if( Delay < 0.0F )
			{
				Delay = ( 0.5F * ANIM_SECOND );
				CreditMode = CREDIT_FADEINPAGE;
			}
			break;
	}

	if( CreditMode != CREDIT_INITIALPAUSE )
	{
		Xpos = (int16) ( CurrentCamera.Viewport.dwWidth / 2.0F );
		Ypos = (int16) ( CurrentCamera.Viewport.dwHeight / 6.0F );

		DisplayForsaken( Xpos, Ypos,
						 (uint8) TitleRed, (uint8) TitleGreen, (uint8) TitleBlue, (uint8) TitleTrans );

		Xpos = (int16) ( CurrentCamera.Viewport.dwWidth / 2.0F );
		Ypos = (int16) ( CurrentCamera.Viewport.dwHeight - 40.0F );

		if( CreditMode != CREDIT_BLANKPAUSE )
		{
			CenterPrintString( SMALLFONT, Xpos, Ypos, PRESS_ANY_KEY1,
							(uint8) PageRed, (uint8) PageGreen, (uint8) PageBlue, (uint8) PageTrans );

			Ypos += 20;

			CenterPrintString( SMALLFONT, Xpos, Ypos, PRESS_ANY_KEY2,
							(uint8) PageRed, (uint8) PageGreen, (uint8) PageBlue, (uint8) PageTrans );
		}
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Release Credits
	Input		:	Nothing
 	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void ReleaseCredits( void )
{
	CreditsEnabled = FALSE;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Fade Colour to wanted colour (float)
	Input		:	float	*	Colour
				:	float		Wanted Colour
	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void FadeColourFloat( float * Colour, float WantedColour, float Step )
{														 
	float	WanCol, CurCol;

	WanCol = WantedColour;
	CurCol = *Colour;

	if( CurCol < WanCol )
	{
		CurCol += Step;
		if( CurCol > WanCol ) CurCol = WanCol;
	}

	if( CurCol > WanCol )
	{
		CurCol -= Step;
		if( CurCol < WanCol ) CurCol = WanCol;
	}

	*Colour = CurCol;
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Get String Length
	Input		:	uint8	*	String
				:	int16		Font
 	Output		:	int16		String Length
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
int16 GetStringLength( uint8 * String, int16 Font )
{
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	uint8			Character;
	int16			StringLength = 0;

	if( !String ) return( StringLength );
	if( !(*String) ) return( StringLength );

	while( *String )
	{
		if( *String != 32 )
		{
			switch( Font )
			{
				case BIGFONT:
					Character = ( CharTable[ *String ] + BIGFONT_OFFSET );
					break;

				case SMALLFONT:
					Character = ( CharTable[ *String ] + SMALLFONT_OFFSET );
					break;
			}

			Bit_Ptr = ( CreditsFont_Header->Bit_Info + Character );
			Off_Ptr = ( CreditsFont_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( CreditsFont_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			switch( Font )
			{
				case BIGFONT:
					StringLength += Box_Ptr->xsize + BIGFONT_CHAR_GAP;
					break;

				case SMALLFONT:
					StringLength += Box_Ptr->xsize + SMALLFONT_CHAR_GAP;
					break;
			}
		}
		else
		{
			switch( Font )
			{
				case BIGFONT:
					StringLength += BIGFONT_SPACESIZE;
					break;

				case SMALLFONT:
					StringLength += SMALLFONT_SPACESIZE;
					break;
			}
		}

		String++;
	}

	return( StringLength );
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display Forsaken Logo
	Input		:	int16		X Position
				:	int16		Y Position
				:	uint8		Red, Green, Blue, Trans
 	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void DisplayForsaken( int16 Xpos, int16 Ypos, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans )
{
	uint16			i;

	i = FindFreeScrPoly();

	if( i != (uint16) -1 )
	{
		ScrPolys[ i ].Flags = SCRFLAG_Scale;
		ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
		ScrPolys[ i ].Pos.x = Xpos;
		ScrPolys[ i ].Pos.y = Ypos;
		ScrPolys[ i ].R = Red;
		ScrPolys[ i ].G = Green;
		ScrPolys[ i ].B = Blue;
		ScrPolys[ i ].Trans = Trans;
		ScrPolys[ i ].Frame = FORSAKEN_OFFSET;
		ScrPolys[ i ].FrameRate = 1.0F;
		ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
		ScrPolys[ i ].Frm_Info = &CreditsFont_Header;
		ScrPolys[ i ].Xscale = 1.0F;
		ScrPolys[ i ].Yscale = 1.0F;
		AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, FORSAKEN_OFFSET ) );
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Display String in center
	Input		:	int16		Font
				:	int16		X Position
				:	int16		Y Position
				:	uint8	*	String
				:	uint8		Red, Green, Blue, Trans
 	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void CenterPrintString( int16 Font, int16 Xpos, int16 Ypos, uint8 * String, uint8 Red, uint8 Green, uint8 Blue, uint8 Trans )
{
	BIT_INFO	*	Bit_Ptr;
	BOX_INFO	*	Box_Ptr;
	OFF_INFO	*	Off_Ptr;
	int16			StringLength = 0;
	uint8			Character;
	uint16			i;

	if( !String ) return;
	if( !(*String) ) return;

	Xpos -= ( GetStringLength( String, Font ) / 2 );

	while( *String )
	{
		if( *String != 32 )
		{
			switch( Font )
			{
				case BIGFONT:
					Character = ( CharTable[ *String ] + BIGFONT_OFFSET );
					break;

				case SMALLFONT:
					Character = ( CharTable[ *String ] + SMALLFONT_OFFSET );
					break;
			}

			Bit_Ptr = ( CreditsFont_Header->Bit_Info + Character );
			Off_Ptr = ( CreditsFont_Header->Off_Info + Bit_Ptr->startbit );
			Box_Ptr = ( CreditsFont_Header->Box_Info + ( Off_Ptr->box & 0x0fff ) );

			i = FindFreeScrPoly();

			if( i != (uint16) -1 )
			{
				ScrPolys[ i ].Flags = SCRFLAG_Nothing;
				ScrPolys[ i ].Type = SCRTYPE_LastAFrame;
				ScrPolys[ i ].Pos.x = Xpos;
				ScrPolys[ i ].Pos.y = Ypos;
				ScrPolys[ i ].R = Red;
				ScrPolys[ i ].G = Green;
				ScrPolys[ i ].B = Blue;
				ScrPolys[ i ].Trans = Trans;
				ScrPolys[ i ].Frame = (float) Character;					// Animation Frame
				ScrPolys[ i ].FrameRate = 1.0F;
				ScrPolys[ i ].SeqNum = SCRSEQ_Nothing;
				ScrPolys[ i ].Frm_Info = &CreditsFont_Header;
				ScrPolys[ i ].Xscale = 1.0F;
				ScrPolys[ i ].Yscale = 1.0F;
				AddScrPolyToTPage( i, GetTPage( *ScrPolys[ i ].Frm_Info, Character ) );
			}

			switch( Font )
			{
				case BIGFONT:
					Xpos += Box_Ptr->xsize + BIGFONT_CHAR_GAP;
					break;

				case SMALLFONT:
					Xpos += Box_Ptr->xsize + SMALLFONT_CHAR_GAP;
					break;
			}
		}
		else
		{
			switch( Font )
			{
				case BIGFONT:
					Xpos += BIGFONT_SPACESIZE;
					break;

				case SMALLFONT:
					Xpos += SMALLFONT_SPACESIZE;
					break;
			}
		}

		String++;
	}
}

/*ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
	Procedure	:	Setup Trans Table
	Input		:	Nothing
 	Output		:	Nothing
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ*/
void SetupCharTransTable( void )
{
	CharTable[(uint8)'à'] = -SMALLFONT_OFFSET + 57;
	CharTable[(uint8)'À'] = -SMALLFONT_OFFSET + 57;
	CharTable[(uint8)'á'] = -SMALLFONT_OFFSET + 58;
	CharTable[(uint8)'Á'] = -SMALLFONT_OFFSET + 58;
	CharTable[(uint8)'ä'] = -SMALLFONT_OFFSET + 59;
	CharTable[(uint8)'Ä'] = -SMALLFONT_OFFSET + 59;
	CharTable[(uint8)'â'] = -SMALLFONT_OFFSET + 74;
	CharTable[(uint8)'Â'] = -SMALLFONT_OFFSET + 74;
	CharTable[(uint8)'å'] = -SMALLFONT_OFFSET + 75;
	CharTable[(uint8)'Å'] = -SMALLFONT_OFFSET + 75;
	CharTable[(uint8)'è'] = -SMALLFONT_OFFSET + 60;
	CharTable[(uint8)'È'] = -SMALLFONT_OFFSET + 60;
	CharTable[(uint8)'é'] = -SMALLFONT_OFFSET + 61;
	CharTable[(uint8)'É'] = -SMALLFONT_OFFSET + 61;
	CharTable[(uint8)'ë'] = -SMALLFONT_OFFSET + 76;
	CharTable[(uint8)'Ë'] = -SMALLFONT_OFFSET + 76;
	CharTable[(uint8)'ê'] = -SMALLFONT_OFFSET + 62;
	CharTable[(uint8)'Ê'] = -SMALLFONT_OFFSET + 62;
	CharTable[(uint8)'ì'] = -SMALLFONT_OFFSET + 63;
	CharTable[(uint8)'Ì'] = -SMALLFONT_OFFSET + 63;
	CharTable[(uint8)'í'] = -SMALLFONT_OFFSET + 79;
	CharTable[(uint8)'Í'] = -SMALLFONT_OFFSET + 79;
	CharTable[(uint8)'ï'] = -SMALLFONT_OFFSET + 77;
	CharTable[(uint8)'Ï'] = -SMALLFONT_OFFSET + 77;
	CharTable[(uint8)'î'] = -SMALLFONT_OFFSET + 78;
	CharTable[(uint8)'Î'] = -SMALLFONT_OFFSET + 78;
	CharTable[(uint8)'ò'] = -SMALLFONT_OFFSET + 64;
	CharTable[(uint8)'Ò'] = -SMALLFONT_OFFSET + 64;
	CharTable[(uint8)'ó'] = -SMALLFONT_OFFSET + 65;
	CharTable[(uint8)'Ó'] = -SMALLFONT_OFFSET + 65;
	CharTable[(uint8)'ö'] = -SMALLFONT_OFFSET + 66;
	CharTable[(uint8)'Ö'] = -SMALLFONT_OFFSET + 66;
	CharTable[(uint8)'ô'] = -SMALLFONT_OFFSET + 80;
	CharTable[(uint8)'Ô'] = -SMALLFONT_OFFSET + 80;
	CharTable[(uint8)'ù'] = -SMALLFONT_OFFSET + 67;
	CharTable[(uint8)'Ù'] = -SMALLFONT_OFFSET + 67;
	CharTable[(uint8)'ú'] = -SMALLFONT_OFFSET + 68;
	CharTable[(uint8)'Ú'] = -SMALLFONT_OFFSET + 68;
	CharTable[(uint8)'ü'] = -SMALLFONT_OFFSET + 69;
	CharTable[(uint8)'Ü'] = -SMALLFONT_OFFSET + 69;
	CharTable[(uint8)'û'] = -SMALLFONT_OFFSET + 81;
	CharTable[(uint8)'Û'] = -SMALLFONT_OFFSET + 81;
	CharTable[(uint8)'ñ'] = -SMALLFONT_OFFSET + 70;
	CharTable[(uint8)'Ñ'] = -SMALLFONT_OFFSET + 70;
	CharTable[(uint8)'Ç'] = -SMALLFONT_OFFSET + 73;
	CharTable[(uint8)'ç'] = -SMALLFONT_OFFSET + 73;
	CharTable[(uint8)'¿'] = -SMALLFONT_OFFSET + 82;
}
