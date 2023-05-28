#pragma once
#include "memory.h"

namespace hacks
{
	// run visual hacks
	void VisualsThread(const Memory& mem) noexcept;

	void Movement(const Memory& mem) noexcept;

	void Triggerbot(const Memory& mem) noexcept;

	void Aimbot(const Memory& mem) noexcept;

	void NoRecoil(const Memory& mem) noexcept;
}