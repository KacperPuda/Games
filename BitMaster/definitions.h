#pragma once
#ifndef definitions_h
#define definitions_h

//player
#define Playershotspeed 18
#define InvOn 1
#define InvOff 0
#define PlayerBulletSpeed 15
#define StartPositionX 500
#define StartPositionY 900
#define PlayerSpeed 4
#define PlayerHitboxU	-2
#define PlayerHitboxD	40
#define PlayerHitboxR	20
#define PlayerHitboxL	0
#define PlayerHealth	3
#define Reload	20

//window
#define SCREEN_WIDTH	1200
#define NAV_X	1044
#define NAV_Y	50
#define REAL_SCREEN_WIDTH	1200
#define SCREEN_HEIGHT	1000
#define FPSLIM	70
#define FightSreenCenterY -420
#define FightSreenCenterX -80
#define AdditionalLength 20
#define BackgroundFightCenterX 240
#define BackgroundFightCenterY 50
#define HealthBarL 900
#define HealthBarH 20
#define HealthBarX 50
#define HealthBarY 50

//info
#define WIN	4
#define GAMEOVER	3
#define FIGHT	2
#define MENU	1
#define MS	1000
#define DeltaToSecond 100

//bullets
#define CFBulletHitboxU	-2
#define CFBulletHitboxD	40
#define CFBulletHitboxR	23
#define CFBulletHitboxL	-4
#define AreaBulletHitboxU	-20
#define AreaBulletHitboxD	350
#define AreaBulletHitboxR	320
#define AreaBulletHitboxL	-50
#define BrimHitboxL	-25
#define BrimHitboxR	125
#define BrimHitboxU	-10
#define BrimHitboxD	140
#define CircleBulets	45
#define CircleBuletsR	10
#define BulletsOnPlayer 50
#define CommonBullet 1
#define SpiralBullet 2
#define WaveBulletFirst 3
#define WaveBulletSecond 4
#define BoomBullet 5
#define BoomDoneType 6
#define WarningArea 7
#define HotArea 8
#define Laser 9
#define HomingBullet 11
#define CBulletAnimatonOne 20
#define CBulletAnimatonTwo 21
#define BoomTypeAnimationOne 30
#define BoomTypeAnimationTwo 31
#define BoomTypeAnimationThree 32
#define BoomTypeAnimationFour 33
#define TrashBullet 0
#define AreaCenterLength 150
#define AreaCenterLengthTwo 450

//bosses
#define ErFU	-5
#define ErFD	100
#define ErFR	150
#define ErFL	-30
#define Bossnr1 1
#define Bossnr2 2
#define BossFCenterX 55
#define BossFCenterY  38
#define BossSCenterX 65
#define BossSCenterY  50
#define BossTCenterX 15
#define BossTCenterY  58
#define BossStartCenterX  440
#define BossStartCenterY  210
#define AnimationChange  50
#define AnimationMS  2
#define BossSpeedDelta  70

#endif