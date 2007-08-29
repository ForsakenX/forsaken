
// localize language
#if defined ( VERSION_SPANISH )
#include "SpanishLocal.h"
#elif defined( VERSION_ITALIAN )
#include "ItalianLocal.h"
#elif defined( VERSION_GERMAN )
#include "GermanLocal.h"
#elif defined( VERSION_FRENCH )
#include "FrenchLocal.h"
#else
#include "EnglishLocal.h"
#endif

// other localizations
#if defined( VERSION_FRENCH )

#define MARKET_FRANCE
#define MARKET_NAME "French"

#elif defined( VERSION_GERMAN )

#define MARKET_GERMANY
#define MARKET_NAME "German"
#define NO_GORE

#elif defined( VERSION_ITALIAN )

#define MARKET_ITALY
#define MARKET_NAME "Italian"

#elif defined( VERSION_SPANISH )

#define MARKET_SPAIN
#define MARKET_NAME "Spanish"

#elif defined( VERSION_UK )

#define MARKET_UK
#define MARKET_NAME "UK"

#elif defined( VERSION_KOREAN )

#define MARKET_KOREAN
#define MARKET_NAME "Korean"
#define NO_GORE

#else

#define MARKET_USA
#define MARKET_NAME "US"

#endif