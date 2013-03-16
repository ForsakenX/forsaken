#ifdef LUA_BOT
#include "botai_main.h"
 
bool BOTAI_CanICollectPickup( u_int16_t i );
bool BOTAI_CheckForGravgons( VECTOR * Pos );
bool BOTAI_ClearLOS( VECTOR * SPos, u_int16_t Group , VECTOR * Pos );
bool BOTAI_ClearLOSNonZero( OBJECT * SObject, VECTOR * Pos, float radius );
bool BOTAI_FriendlyFireCheck();
bool BOTAI_InViewCone( VECTOR * Pos, MATRIX * Mat, VECTOR * TPos, float ViewConeCos );
bool BOTAI_SafeToMove();
bool BOTAI_WillHomingMissileHit(VECTOR * MyPos);
float BOTAI_DistanceToWall( bool u, bool d, bool l, bool r, bool f, bool b );
float BOTAI_WhenWillBulletHitMe(VECTOR * MyPos);
float BOTAI_WhenWillBulletHitSlide( bool u, bool d, bool l, bool r, bool f, bool b );
int BOTAI_CenterCheck( int move );
int BOTAI_ComparativeEnemyStrength();
int BOTAI_ShipStrength( int i );
void BOTAI_GetNearestPickup();
void BOTAI_GetSHIPTarget();
void BOTAI_LookAhead( float Accuracy, VECTOR * SPos, BYTE TargetID, VECTOR * NewPos, float SpeedOfBullet );
void BOTAI_LookAheadPos( float Time, VECTOR * NewPos, VECTOR SlideDirection );
void BOTAI_UpdateSensors();

#endif
