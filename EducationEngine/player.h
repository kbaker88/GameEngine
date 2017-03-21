#ifndef PLAYER_H
#define PLAYER_H

#include "entity_system.h"
#include "camera.h" // TODO: Remove to somewhere else

/* MovementSpeed * SecondsPerFrame * PixelsPerMeter
   1.4 meters/sec * 1/60 sec/frame * 0.25 pix/meter
*/

class Player
{
public:
	Player();
	~Player();

	bool InitPlayer(uint32 EntityBlockNumber, uint32 DesiredID,
		uint32 ObjectBlockNumber, uint32 ObjectID, v3 &StartPosition);

	void UpdateRotation();
	void UpdateTranslation();
	v3* GetDirection();
	v3* GetFrontDirection();
	uint32 GetID();

	v3 PlayerFrontVect, PlayerUpVect;
	v3 PlayerDirectVect;

	v2 CursorOffset, PrevCursorPosition;

	uint32 PlayerEntityID, ObjectID;
	float pitch, yaw;
};

Player* GetPlayer(uint32 ID);

#endif