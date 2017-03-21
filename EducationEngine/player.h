#ifndef PLAYER_H
#define PLAYER_H

#include "collision_system.h"

/* MovementSpeed * SecondsPerFrame * PixelsPerMeter
   1.4 meters/sec * 1/60 sec/frame * 0.25 pix/meter
*/

class Player
{
public:
	Player();
	~Player();

	void UpdateRotation();
	void UpdateTranslation();
	v3* GetDirection();
	v3* GetFrontDirection();

	v3 PlayerFrontVect, PlayerUpVect;
	v3 PlayerDirectVect;
	v2 CursorOffset, PrevCursorPosition;
	float pitch, yaw;
};

#endif