#pragma once
/***********************************************
* @headerfile VulkanShaderInitializers.h
* @date 06 / 08 / 2025
* @author Roomain
************************************************/
#include "vulkan/vulkan.h"
#include "utils/VulkanDefines.h"

/*@brief functions for structure initialisation*/
namespace Vulkan::Initializers
{
	struct SubmitInfoParameters
	{
		VkSemaphore* waitSemaphores = nullptr;
		std::vector<VkPipelineStageFlags> pipelineStages;
		VkCommandBuffer* commandBuffers = nullptr;
		VkSemaphore* signalSemaphores = nullptr;
		uint32_t waitCount = 1;
		uint32_t bufferCount = 1;
		uint32_t signalCount = 1;
	};

	[[nodiscard]] constexpr VkSubmitInfo submitInfo(const SubmitInfoParameters& params)
	{
		return VkSubmitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = params.waitCount,
			.pWaitSemaphores = params.waitSemaphores,
			.pWaitDstStageMask = params.pipelineStages.data(),
			.commandBufferCount = params.bufferCount,
			.pCommandBuffers = params.commandBuffers,
			.signalSemaphoreCount = params.signalCount,
			.pSignalSemaphores = params.signalSemaphores
		};
	}

	template<typename BufferContainer, typename SemaphoreContainer>
	struct TSubmitInfoParameters
	{
		SemaphoreContainer waitSemaphores;
		std::vector<VkPipelineStageFlags> pipelineStages;
		BufferContainer commandBuffers;
		SemaphoreContainer signalSemaphores;
	};

	template<typename BufferContainer, typename SemaphoreContainer>
	[[nodiscard]] constexpr VkSubmitInfo submitInfo(const TSubmitInfoParameters<BufferContainer, SemaphoreContainer>& params)
	{
		return VkSubmitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = static_cast<uint32_t>(params.waitSemaphores.size()),
			.pWaitSemaphores = params.waitSemaphores.data(),
			.pWaitDstStageMask = params.pipelineStages.data(),
			.commandBufferCount = static_cast<uint32_t>(params.commandBuffers.size()),
			.pCommandBuffers = params.commandBuffers.data(),
			.signalSemaphoreCount = static_cast<uint32_t>(params.signalSemaphores.size()),
			.pSignalSemaphores = params.signalSemaphores.data(),
		};
	}

	struct SubmitInfoParametersWithWait
	{
		VkSemaphore* waitSemaphore = nullptr;
		std::vector<VkPipelineStageFlags> pipelineStages;
		VkCommandBuffer* cmdBuffers = nullptr;
		VkSemaphore* signalSemaphore = nullptr;
		uint32_t waitCount = 1;
		uint32_t bufferCount = 1;
		uint32_t signalCount = 1;
	};

	[[nodiscard]] constexpr VkSubmitInfo submitInfo(const SubmitInfoParametersWithWait& params)
	{
		return VkSubmitInfo{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.pNext = nullptr,
			.waitSemaphoreCount = params.waitCount,
			.pWaitSemaphores = params.waitSemaphore,
			.pWaitDstStageMask = params.pipelineStages.data(),
			.commandBufferCount = params.bufferCount,
			.pCommandBuffers = params.cmdBuffers,
			.signalSemaphoreCount = params.signalCount,
			.pSignalSemaphores = params.signalSemaphore,
		};
	}

	[[nodiscard]] constexpr VkCommandBufferAllocateInfo commandBufferCreateInfo(const VkCommandPool a_cmdPool,
		const VkCommandBufferLevel a_level, uint32_t a_bufferCount)
	{
		return VkCommandBufferAllocateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.pNext = nullptr,
			.commandPool = a_cmdPool,
			.level = a_level,
			.commandBufferCount = a_bufferCount
		};
	}

	[[nodiscard]] constexpr VkCommandPoolCreateInfo commandPoolCreateInfo(const VkCommandPoolCreateFlags a_flags,
		const uint32_t a_familyIndex)
	{
		return VkCommandPoolCreateInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = a_flags,
			.queueFamilyIndex = a_familyIndex
		};
	}

	[[nodiscard]] constexpr VkCommandBufferBeginInfo commandBufferBeginInfo(const VkCommandBufferUsageFlags a_flags,
		const VkCommandBufferInheritanceInfo* a_pBuffInheritInfo)
	{
		return VkCommandBufferBeginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.pNext = nullptr,
			.flags = a_flags,
			.pInheritanceInfo = a_pBuffInheritInfo
		};
	}

}
