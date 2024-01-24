#pragma once

struct Matrix3x4
{
	float m[3][4];
};

struct Matrix4x4
{
	float m[4][4];
};

// Created with ReClass.NET 1.2 by KN4CK3R

class _Start
{
public:
	class TeamManager* TeamManager_ptr; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class TeamManager
{
public:
	char pad_0000[56]; //0x0000
	class UnkGame3* UnkGame3_ptr; //0x0038
	char pad_0040[400]; //0x0040
}; //Size: 0x01D0

class N000000D9
{
public:
	class N000000FF* N000000DA; //0x0000
	char pad_0008[192]; //0x0008
}; //Size: 0x00C8

class N000000FF
{
public:
	class N00000125* N00000100; //0x0000
}; //Size: 0x0008

class N00000125
{
public:
	char pad_0000[8]; //0x0000
}; //Size: 0x0008

class UnkGame3
{
public:
	class UnkGame4* UnkGame4_ptr; //0x0000
	char pad_0008[112]; //0x0008
}; //Size: 0x0078

class UnkGame4
{
public:
	char pad_0000[8]; //0x0000
	class UnkGame5* UnkGame5_ptr; //0x0008
	char pad_0010[256]; //0x0010
}; //Size: 0x0110

class UnkGame5
{
public:
	class UnkGame6* UnkGame6_ptr; //0x0000
}; //Size: 0x0008

class UnkGame6
{
public:
	class UnkGame7* UnkGame7_ptr; //0x0000
	char pad_0008[128]; //0x0008
}; //Size: 0x0088

class UnkGame7
{
public:
	char pad_0000[32]; //0x0000
	class GamePlayer* GameObject_ptr; //0x0020
	char pad_0028[96]; //0x0028
}; //Size: 0x0088

class GamePlayer
{
public:
	char pad_0000[16]; //0x0000
	class GamePlayer* PlayerObject_ptr; //0x0010
	char pad_0018[56]; //0x0018
	class N000009ED* ChildrenGameObjs; //0x0050 actually an array but reclass.net ig? lmao
	char pad_0058[24]; //0x0058
	Matrix3x4 playerMatrix; //0x0070
	float PlayerX; //0x00A0
	float PlayerY; //0x00A4
	float PlayerZ; //0x00A8
	float N000001FD; //0x00AC
	float N000001F8; //0x00B0
	float N000001FF; //0x00B4
	float N000001F9; //0x00B8
	float N0000034A; //0x00BC
	float N00000338; //0x00C0
	float N0000034C; //0x00C4
	float N0000029B; //0x00C8
	float N0000034E; //0x00CC
	float N0000029C; //0x00D0
	float N00000350; //0x00D4
	float N0000029D; //0x00D8
	float N00000352; //0x00DC
	float N0000029E; //0x00E0
	float unk2; //0x00E4
	float posX; //0x00E8
	float posY; //0x00EC
	float posZ; //0x00F0
	float unk; //0x00F4
	float unk3; //0x00F8
	char pad_00FC[108]; //0x00FC
	wchar_t N000001DA[48]; //0x0168
	char pad_01C8[48]; //0x01C8
	float MaxHealthPoints; //0x01F8
	float HealthPoints; //0x01FC
	char pad_0200[8]; //0x0200
	float MaxBoosterCharge; //0x0208
	float BoosterCharge; //0x020C
	char pad_0210[4]; //0x0210
	float SecondsElapsed; //0x0214
	char pad_0218[388]; //0x0218
	wchar_t N000001E8[40]; //0x039C
	char pad_03EC[96]; //0x03EC
	float N000001E2; //0x044C
	float N000003BE; //0x0450
	float N000001E3; //0x0454
	float N00000333; //0x0458
	float N000003B5; //0x045C
	float N000001E4; //0x0460
	float N000003B7; //0x0464
	float N000001E5; //0x0468
	float N000003B9; //0x046C
	char pad_0470[1144]; //0x0470
	int32_t CurrentAmmo; //0x08E8
	char pad_08EC[244]; //0x08EC
}; //Size: 0x09E0

class N000003F5
{
public:
	class GamePlayer* N000003F6; //0x0000
	char pad_0008[64]; //0x0008
	class GamePlayer* N0000042F; //0x0048
	char pad_0050[56]; //0x0050
}; //Size: 0x0088

class N00000411
{
public:

	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual void Function3();
	virtual void Function4();
	virtual void Function5();
	virtual void Function6();
	virtual void Function7();
	virtual void Function8();
	virtual void Function9();
}; //Size: 0x0008

class N0000045B
{
public:
	char pad_0000[256]; //0x0000
	float N0000047C; //0x0100
	float N00000488; //0x0104
	float N0000047D; //0x0108
	float N0000048A; //0x010C
	char pad_0110[608]; //0x0110
	int64_t IsInAir; //0x0370
	char pad_0378[464]; //0x0378
}; //Size: 0x0548

class N000009ED
{
public:
	class GameBooster* N000009EE; //0x0000
	class GameBooster* N000009EF; //0x0008
	class N0000113E* N000009F0; //0x0010
	char pad_0018[48]; //0x0018
	class GameWeapon* PlayerWeapon1_ptr; //0x0048
	class GameWeapon* PlayerWeapon2_ptr; //0x0050
	char pad_0058[112]; //0x0058
}; //Size: 0x00C8

class PlayerWeapon
{
public:
	char pad_0000[192]; //0x0000
	class GamePlayer* PlayerObject_ptr; //0x00C0
	char pad_00C8[144]; //0x00C8
	wchar_t* WeaponIconTexturePath; //0x0158
	char pad_0160[1704]; //0x0160
}; //Size: 0x0808

class GameWeapon
{
public:
	char pad_0000[222]; //0x0000
	bool IsEquipped; //0x00DE
	char pad_00DF[81]; //0x00DF
	float N00000C31; //0x0130
	float N00000C8F; //0x0134
	float N00000C32; //0x0138
	float N00000C91; //0x013C
	char pad_0140[24]; //0x0140
	wchar_t* TexturePath; //0x0158
	char pad_0160[1928]; //0x0160
	uint32_t CurrentAmmo; //0x08E8
	char pad_08EC[3892]; //0x08EC
}; //Size: 0x1820

class N0000113E
{
public:
	char pad_0000[16]; //0x0000
	void* N00001141; //0x0010
	char pad_0018[208]; //0x0018
}; //Size: 0x00E8

class N000011E2
{
public:
	class N00001208* N000011E3; //0x0000
}; //Size: 0x0008

class N00001208
{
public:
	char pad_0000[8]; //0x0000
}; //Size: 0x0008

class GameBooster
{
public:
	char pad_0000[168]; //0x0000
	float N0000124A; //0x00A8
	char pad_00AC[52]; //0x00AC
	float N00001251; //0x00E0
	float N000012DF; //0x00E4
	float N00001252; //0x00E8
	float N000012E1; //0x00EC
	char pad_00F0[128]; //0x00F0
	bool IsInAir; //0x0170
	char pad_0171[443]; //0x0171
	Matrix4x4 N00001299; //0x032C
	char pad_036C[488]; //0x036C
}; //Size: 0x0554

class N0000131D
{
public:
	char pad_0000[1032]; //0x0000
}; //Size: 0x0408

