#pragma once
#include <cstdint>
#include <cstddef>

namespace globals
{
	inline std::uintptr_t clientAddress = 0;

	inline std::uintptr_t engineAddress = 0;
	

	inline bool glow = false;
	inline float enemyColor[] = { 1.f, 0.f, 0.f, 1.f };

	inline bool glowteam = false;
	inline float teamColor[] = { 1.f, 0.f, 0.f, 1.f };

	inline bool radar = false;

	inline bool noflash = false;

	inline bool bhop = false;

	inline bool triggerbot = false;

	inline bool aimbot = false;

	inline bool norecoil = false;
}

namespace offsets
{
	constexpr ::std::ptrdiff_t dwLocalPlayer = 0xDE997C;
	constexpr ::std::ptrdiff_t dwGlowObjectManager = 0x5359988;
	constexpr ::std::ptrdiff_t dwEntityList = 0x4DFEF0C;
	constexpr ::std::ptrdiff_t dwForceJump = 0x52BAC68;
	constexpr ::std::ptrdiff_t dwForceAttack = 0x322CD48;
	constexpr ::std::ptrdiff_t dwClientState = 0x59F19C;
	constexpr ::std::ptrdiff_t dwClientState_ViewAngles = 0x4D90;
	constexpr ::std::ptrdiff_t dwClientState_GetLocalPlayer = 0x180;

	constexpr ::std::ptrdiff_t m_flFlashDuration = 0x10470;
    constexpr ::std::ptrdiff_t m_iTeamNum = 0xF4;
	constexpr ::std::ptrdiff_t m_lifeState = 0x25F;
	constexpr ::std::ptrdiff_t m_iGlowIndex = 0x10488;
	constexpr ::std::ptrdiff_t m_bSpotted = 0x93D;
    constexpr ::std::ptrdiff_t m_fFlags = 0x104;
	constexpr ::std::ptrdiff_t m_iHealth = 0x100;
	constexpr ::std::ptrdiff_t m_iCrosshairId = 0x11838;
	constexpr ::std::ptrdiff_t m_dwBoneMatrix = 0x26A8;
	constexpr ::std::ptrdiff_t m_bDormant = 0xED;
	constexpr ::std::ptrdiff_t m_vecOrigin = 0x138;
	constexpr ::std::ptrdiff_t m_vecViewOffset = 0x108;
	constexpr ::std::ptrdiff_t m_aimPunchAngle = 0x303C;
	constexpr ::std::ptrdiff_t m_bSpottedByMask = 0x980;
	constexpr ::std::ptrdiff_t m_iShotsFired = 0x103E0;
}