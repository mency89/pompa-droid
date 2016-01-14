#ifndef __ACTIONTAG_H__
#define __ACTIONTAG_H__

enum ActionTags
{
	kHeroIdle = 1001,
	kHeroWalk = 1002,
	kHeroRun = 1003,
	kHeroJump = 1004,
	kHeroAttcak = 1005,
	kHeroRunAttcak = 1006,
	kHeroJumpAttcak = 1007,
	kHeroHurt = 1008,
	kHeroKnockout = 1009,
	kheroGetup = 1010,
	kBossIdle = 1011,
	kBossWalk = 1012,
	kBossHurt = 1013,
	kBossKnockout = 1015,
	kBossGetup = 1015,
	kBossAttack = 1016,
	kRobotIdle = 1017,
	kRobotWalk = 1018,
	kRobotAttack = 1019,
	kRobotHurt = 1020,
	kRobotKnockout = 1021,
	kRobotGetup = 1022,
	kRobotSmokeAttack = 1023,
	kRobotSmokeGetup = 1024,
	kRobotSmokeHurt = 1025,
	kRobotSmokeIdle = 1026,
	kRobotSmokeKnockout = 1027,
	kRobotSmokeWalk = 1028,
	kRobotBeltAttack = 1029,
	kRobotBeltGetup = 1030,
	kRobotBeltHurt = 1031,
	kRobotBeltIdle = 1032,
	kRobotBeltKnockout = 1033,
	kRobotBeltWalk = 1034,
};

#endif