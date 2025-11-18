#pragma once
/***********************************************
* @headerfile MemoryStructures.h
* @date 18 / 11 / 2025
* @author Roomain
************************************************/
#include <functional>
#include "vulkan/vulkan.hpp"
#include "vk_mem_alloc.h"

namespace Vulkan::Memory
{
	/*@brief vulkan buffer memory handles*/
	struct Buffer
	{
		VkBuffer m_buffer = VK_NULL_HANDLE;		/*!< vulkan buffer handle*/
		VmaAllocation m_Alloc = VK_NULL_HANDLE;	/*!< allocation handle*/
		VkDeviceSize m_memorySize = 0;			/*!< buffer size*/
	};

	/*@brief vulkan image memory handles*/
	struct Image
	{
		VkImage m_image = VK_NULL_HANDLE;       /*!< vulkan image handle*/
		VmaAllocation m_Alloc = VK_NULL_HANDLE; /*!< allocation handle*/
	};

	/******************************************************************************/
	/*@brief allocation parameters of Gpu/Device memory*/
	struct AllocationParameter
	{
		size_t m_size;								/*!< memory size to allocate (in bytes)*/
		VkBufferUsageFlags m_usage;					/*!< buffer usage flags*/
		VmaAllocationCreateFlags m_allocFlag;		/*!< allocation flag bits*/
		std::vector<uint32_t> m_sharedQueue;		/*!< buffer shared queue*/
	};

	/*@brief parameters for transfer staging memory to Gpu/Device memory*/
	struct TransferParameter
	{
		VulkanBuffer m_buffer;			/*!< buffer handle to transfer*/
		uint32_t m_srcOffset = 0;		/*!< source offset*/
		uint32_t m_destOffset = 0;		/*!< destionation offset*/
	};

	/*@brief staging copy parameter*/
	struct StagingCopyParameter
	{
		VkDeviceSize m_srcOffset = 0;	/*!< source offset in elements*/
		VkDeviceSize m_srcSize = 0;	/*!< source size in elements 0 means entire size*/
		VkDeviceSize m_dstOffset = 0;	/*!< destination offset in elements*/

		template<template<typename> class Container, typename Type>
		constexpr Type* data(const Container<Type>& a_dataContainer)const
		{
			return a_dataContainer.data() + m_srcOffset;
		}

		template<template<typename> class Container, typename Type>
		VkDeviceSize memorySize(const Container<Type>& a_dataContainer)const
		{
			if(m_srcSize == 0)
				return sizeof(Type) * (a_dataContainer.size() - m_srcOffset);
			return sizeof(Type) * m_srcSize;
		}

		template<typename Type>
		constexpr VkDeviceSize destinationOffset()const
		{
			return sizeof(Type) * m_dstOffset;
		}
	};
}