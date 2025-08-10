#include "pch.h"
#include "memory/VE_MemoryCallbacks.h"


void* vulkanCustom_CpuAllocation(void* a_pUserData, size_t a_size, size_t a_alignment, VkSystemAllocationScope /*a_allocationScope*/)
{
	void* const result = _aligned_malloc(a_size, a_alignment);
	if (result && a_pUserData)
	{
		static_cast<VE_IDebugMemoryAllocation*>(a_pUserData)->allocateMemory(a_size, result);
	}
	return result;
}

void* vulkanCustom_CpuReallocation(void* a_pUserData, void* a_pOriginal, size_t a_size, size_t a_alignment, VkSystemAllocationScope /*a_allocationScope*/)
{
	void* const result = _aligned_realloc(a_pOriginal, a_size, a_alignment);
	if (a_pOriginal && !result)
	{
		static_cast<VE_IDebugMemoryAllocation*>(a_pUserData)->freeMemory(a_pOriginal);
	}
	else if (!a_pOriginal && result)
	{
		static_cast<VE_IDebugMemoryAllocation*>(a_pUserData)->reallocMemory(a_size, a_pOriginal, result);
	}
	return result;
}

void vulkanCustom_CpuFree(void* a_pUserData, void* a_pMemory)
{
	if (a_pMemory)
	{
		if (a_pUserData)
		{
			static_cast<VE_IDebugMemoryAllocation*>(a_pUserData)->freeMemory(a_pMemory);
		}
		_aligned_free(a_pMemory);
	}
}