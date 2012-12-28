#ifdef LUA_BOT

bool BOTAI_AimAtTarget( MATRIX * InvMat, VECTOR * SPos, VECTOR * TPos );
bool BOTAI_CollectNearestPickup(bool SlideOnly);
bool BOTAI_FireTitan();
bool BOTAI_MaintainDistanceToTargetShip();
bool BOTAI_MoveToTarget(VECTOR * TPos);
bool BOTAI_SetAction( float * action, float value, char * str );
bool BOTAI_ShootEnemyMines();
bool BOTAI_SlideToTarget(VECTOR * TPos);
void BOTAI_AimAtTargetShip();
void BOTAI_AvoidBullets();
void BOTAI_AvoidHomingMissiles();
void BOTAI_ClearActions();
void BOTAI_FireMissiles();
void BOTAI_SelectWeapon( bool MineTarget );
void BOTAI_ShootAtTargetShip(); 

#endif
