#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

struct VE_QueueCreateInfo
{
	uint32_t m_familyIndex = 0;
	std::vector<float> m_priorities;
};