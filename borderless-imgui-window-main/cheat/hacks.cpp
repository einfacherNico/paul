#include "hacks.h"
#include "gui.h"
#include "globals.h"
#include <thread>
#include "vector.h"

constexpr Vector3 CalculateAngle(
	const Vector3& localPosition,
	const Vector3& enemyPosition,
	const Vector3& viewAngles) noexcept
{
	return ((enemyPosition - localPosition).ToAngle() - viewAngles);
}

struct Vector2
{
	float x = {}, y = {};
};

auto oldPunch = Vector2{ };





void hacks::Movement(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

		if (globals::bhop)
			if (localPlayer)
			{
				const auto onGround = mem.Read<bool>(localPlayer + offsets::m_fFlags);

				if (GetAsyncKeyState(VK_SPACE) && onGround & (1 << 0))
					mem.Write<BYTE>(globals::clientAddress + offsets::dwForceJump, 6);
			}
	}
		
}

void hacks::VisualsThread(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);

		if (!localPlayer)
			continue;

		const auto glowManager = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwGlowObjectManager);

		if (!glowManager)
			continue;

		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		for (auto i = 1; i <= 32; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

			if (!player)
				continue;

			const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);

			const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);

			if (lifeState != 0)
				continue;

			const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

			if (globals::glowteam && team == localTeam)
			{
				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::teamColor[0]);   // red
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::teamColor[1]);   // green
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::teamColor[2]);  // blue
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::teamColor[3]);  // alpha
			}
			else if (globals::glow && team != localTeam)
			{
				mem.Write(glowManager + (glowIndex * 0x38) + 0x8, globals::enemyColor[0]);  // red
				mem.Write(glowManager + (glowIndex * 0x38) + 0xC, globals::enemyColor[1]);  // green
				mem.Write(glowManager + (glowIndex * 0x38) + 0x10, globals::enemyColor[2]); // blue
				mem.Write(glowManager + (glowIndex * 0x38) + 0x14, globals::enemyColor[3]); // alpha
			}

			mem.Write(glowManager + (glowIndex * 0x38) + 0x28, true);
			mem.Write(glowManager + (glowIndex * 0x38) + 0x29, false);

			if (globals::radar)
				mem.Write(player + offsets::m_bSpotted, true);

			// no flash
			const auto flashDur = mem.Read<int>(localPlayer + offsets::m_flFlashDuration);

			if (globals::noflash)
				if (flashDur > 0)
					mem.Write<int>(localPlayer + offsets::m_flFlashDuration, 0);
		}

	}
}

void hacks::Triggerbot(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!globals::triggerbot)
			continue;

		// skip if trigger isnt on
		if (!GetAsyncKeyState(VK_RBUTTON))
			continue;

		const auto& localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
		const auto& localHealth = mem.Read<std::int32_t>(localPlayer + offsets::m_iHealth);

		// skip if local player is dead
		if (!localHealth)
			continue;

		const auto& crosshairId = mem.Read<std::int32_t>(localPlayer + offsets::m_iCrosshairId);

		if (!crosshairId || crosshairId > 64)
			continue;

		const auto& player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + (crosshairId - 1) * 0x10);

		// skipf if player is dead
		if (!mem.Read<std::int32_t>(player + offsets::m_iHealth)) // Liest Player + Health aus
			continue;

		// skip if player is in our team
		if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) ==
			mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum)) // Wenn Player die gleiche TeamNum hat wie Localplayer (wir) dann = Team
			continue;

		// Wenn alles bereit ist und wir schieﬂen m¸ssen

		mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 6); // Wir schreiben 6 zur Memory in Client und Force Attack
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 20 Milliseconds warten
		mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 4); // Hier kann man wieder schieﬂen
		mem.Write<std::uintptr_t>(globals::clientAddress + offsets::dwForceAttack, 6); // Erneut schieﬂen damit die Schleife klappt
	}
}

void hacks::Aimbot(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!globals::aimbot)
			continue;

		if (!GetAsyncKeyState(VK_XBUTTON2))
			continue;

		// get local player
		const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
		const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);

		// eye position = origin + viewOffset
		const auto localEyePosition = mem.Read<Vector3>(localPlayer + offsets::m_vecOrigin) +
			mem.Read<Vector3>(localPlayer + offsets::m_vecViewOffset);

		const auto clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);

		const auto localPlayerId =
			mem.Read<std::int32_t>(clientState + offsets::dwClientState_GetLocalPlayer);

		const auto viewAngles = mem.Read<Vector3>(clientState + offsets::dwClientState_ViewAngles);
		const auto aimPunch = mem.Read<Vector3>(localPlayer + offsets::m_aimPunchAngle) * 2;

		// aimbot fov
		auto bestFov = gui::aimbotfov;
		auto bestAngle = Vector3{ };

		for (auto i = 1; i <= 32; ++i)
		{
			const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);

			if (mem.Read<std::int32_t>(player + offsets::m_iTeamNum) == localTeam)
				continue;

			if (mem.Read<bool>(player + offsets::m_bDormant))
				continue;

			if (mem.Read<std::int32_t>(player + offsets::m_lifeState))
				continue;

			if (mem.Read<std::int32_t>(player + offsets::m_bSpottedByMask) & (1 << localPlayerId))
			{
				const auto boneMatrix = mem.Read<std::uintptr_t>(player + offsets::m_dwBoneMatrix);

				// pos of player head in 3d space
				// 8 is the head bone index :)
				const auto playerHeadPosition = Vector3{
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x0C),
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x1C),
					mem.Read<float>(boneMatrix + 0x30 * 8 + 0x2C)
				};

				const auto angle = CalculateAngle(
					localEyePosition,
					playerHeadPosition,
					viewAngles + aimPunch
				);

				const auto fov = std::hypot(angle.x, angle.y);

				if (fov < bestFov)
				{
					bestFov = fov;
					bestAngle = angle;
				}
			}
		}
		
		// if we have a best angle, do aimbot
		if (!bestAngle.IsZero())
			mem.Write<Vector3>(clientState + offsets::dwClientState_ViewAngles, viewAngles + bestAngle /  gui::aimbotfloat); // smoothing
	}
}

void hacks::NoRecoil(const Memory& mem) noexcept
{
	while (gui::isRunning)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (!globals::norecoil)
			continue;

		const auto& localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
		const auto& shotsFired = mem.Read<std::int32_t>(localPlayer + offsets::m_iShotsFired);

		if (shotsFired)
		{
			const auto& clientState = mem.Read<std::uintptr_t>(globals::engineAddress + offsets::dwClientState);
			const auto& viewAngles = mem.Read<Vector2>(clientState + offsets::dwClientState_ViewAngles);

			const auto& aimPunch = mem.Read<Vector2>(localPlayer + offsets::m_aimPunchAngle);

			auto newAngles = Vector2
			{
				viewAngles.x + oldPunch.x - aimPunch.x * 2.f, // 2.f Valve Server nutzen das
				viewAngles.y + oldPunch.y - aimPunch.y * 2.f,
			};

			if (newAngles.x > 89.f)
				newAngles.x = -89.f;

			if (newAngles.x < -89.f)
				newAngles.x = 89.f;

			while (newAngles.y > 180.f)
				newAngles.y += 360.f;

			while (newAngles.y > -180.f)
				newAngles.y -= 360.f;

			mem.Write<Vector2>(clientState + offsets::dwClientState_ViewAngles, newAngles);

			oldPunch.x = aimPunch.x * 2.f;
			oldPunch.y = aimPunch.y * 2.f;
		}
		else
		{
			oldPunch.x = oldPunch.y = 0.f; // Falls ich nicht schieﬂe passiert nichts
		}
	}
	
}

