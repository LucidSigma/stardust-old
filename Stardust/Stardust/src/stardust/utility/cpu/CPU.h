#pragma once
#ifndef CPU_H
#define CPU_H

namespace stardust
{
	namespace cpu
	{
		extern unsigned int GetL1CacheLineSize();
		extern unsigned int GetCPUCount();
		extern unsigned int GetSystemRAMCount();
	}
}

#endif