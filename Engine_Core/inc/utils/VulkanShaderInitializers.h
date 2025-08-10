#pragma once
/***********************************************
* @headerfile VulkanShaderInitializers.h
* @date 06 / 08 / 2026
* @author Roomain
************************************************/
#include "vulkan/vulkan.h"
#include "utils/VulkanDefines.h"

/*@brief functions for structure initialisation*/
namespace Vulkan::Initializers
{
	struct DescriptorSetParameters
	{
		VkDescriptorSet descriptorSet;
		VkDescriptorType descriptorType;
		uint32_t dstBinding = 0;
		uint32_t dstArrayElement = 0;
		uint32_t descriptorCount = 1;
		VkDescriptorImageInfo* imgInfo = nullptr;
		VkDescriptorBufferInfo* bufferInfo = nullptr;
		VkBufferView* pTexelBufferView = nullptr;
	};

	[[nodiscard]] constexpr VkWriteDescriptorSet writeDescriptorSet(const DescriptorSetParameters& params)
	{
		return VkWriteDescriptorSet{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = params.descriptorSet,
			.dstBinding = params.dstBinding,
			.dstArrayElement = params.dstArrayElement,
			.descriptorCount = params.descriptorCount,
			.descriptorType = params.descriptorType,
			.pImageInfo = params.imgInfo,
			.pBufferInfo = params.bufferInfo,
			.pTexelBufferView = params.pTexelBufferView
		};
	}

	[[nodiscard]] constexpr VkDescriptorBufferInfo descriptorBufferInfo(const VkBuffer a_buffer, const VkDeviceSize a_offset = 0, const VkDeviceSize a_range = VK_WHOLE_SIZE)
	{
		return VkDescriptorBufferInfo{
			.buffer = a_buffer,
			.offset = a_offset,
			.range = a_range,
		};
	}

	[[nodiscard]] constexpr VkDescriptorImageInfo descriptorImageInfo(const VkSampler a_sampler, const VkImageView a_imageView, const VkImageLayout a_imageLayout)
	{
		return VkDescriptorImageInfo{
			.sampler = a_sampler,
			.imageView = a_imageView,
			.imageLayout = a_imageLayout
		};
	}

	[[nodiscard]] constexpr VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(
		VkDescriptorType a_type,
		VkShaderStageFlags a_stageFlags,
		uint32_t a_binding,
		uint32_t a_descriptorCount = 1)
	{
		return VkDescriptorSetLayoutBinding{
			.binding = a_binding,
			.descriptorType = a_type,
			.descriptorCount = a_descriptorCount,
			.stageFlags = a_stageFlags
		};
	}

	[[nodiscard]] constexpr VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const VkDescriptorSetLayoutBinding* a_pBindings,
		uint32_t a_bindingCount)
	{
		return VkDescriptorSetLayoutCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.bindingCount = a_bindingCount,
		.pBindings = a_pBindings };
	}

	template<typename BindingContainer>
	[[nodiscard]] constexpr VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(
		const BindingContainer& a_bindings)
	{
		return VkDescriptorSetLayoutCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.pNext = nullptr,
		.bindingCount = static_cast<uint32_t>(a_bindings.size()),
		.pBindings = a_bindings.data() };
	}

	[[nodiscard]] constexpr VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(
		VkDescriptorPool a_descriptorPool,
		const VkDescriptorSetLayout* a_pSetLayouts,
		uint32_t a_descriptorSetCount)
	{
		return VkDescriptorSetAllocateInfo{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.pNext = nullptr,
		.descriptorPool = a_descriptorPool,
		.descriptorSetCount = a_descriptorSetCount,
		.pSetLayouts = a_pSetLayouts };
	}

	[[nodiscard]] inline VkShaderModuleCreateInfo shaderModuleCreateInfo(std::vector<char>& a_shaderCode)
	{
		return VkShaderModuleCreateInfo{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pNext = nullptr,
		.codeSize = a_shaderCode.size(),
		.pCode = reinterpret_cast<uint32_t*>(a_shaderCode.data()) };
	}

	struct WriteDescriptorSetParameters
	{
		VkDescriptorSet dstSet;
		VkDescriptorType descriptorType;
		uint32_t dstBinding = 0;
		uint32_t dstArrayElement = 0;
		uint32_t descriptorCount = 1;
		const VkDescriptorImageInfo* pImageInfo = nullptr;
		const VkDescriptorBufferInfo* pBufferInfo = nullptr;
		const VkBufferView* pTexelBufferView = nullptr;
	};

	[[nodiscard]] constexpr VkWriteDescriptorSet writeDescriptorSet(const WriteDescriptorSetParameters& params)
	{
		return VkWriteDescriptorSet{
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.pNext = nullptr,
			.dstSet = params.dstSet,
			.dstBinding = params.dstBinding,
			.dstArrayElement = params.dstArrayElement,
			.descriptorCount = params.descriptorCount,
			.descriptorType = params.descriptorType,
			.pImageInfo = params.pImageInfo,
			.pBufferInfo = params.pBufferInfo,
			.pTexelBufferView = params.pTexelBufferView
		};
	}


	[[nodiscard]] constexpr VkPushConstantRange pushConstantRange(
		VkShaderStageFlags a_stageFlags,
		uint32_t a_size,
		uint32_t a_offset)
	{
		return VkPushConstantRange{
		.stageFlags = a_stageFlags,
		.offset = a_offset,
		.size = a_size };
	}

	[[nodiscard]] constexpr VkBindSparseInfo bindSparseInfo()
	{
		return VkBindSparseInfo{ .sType = VK_STRUCTURE_TYPE_BIND_SPARSE_INFO, .pNext = nullptr };
	}

	/** @brief Initialize a map entry for a shader specialization constant */
	[[nodiscard]] constexpr VkSpecializationMapEntry specializationMapEntry(uint32_t a_constantID, uint32_t a_offset, size_t a_size)
	{
		return VkSpecializationMapEntry{
		.constantID = a_constantID,
		.offset = a_offset,
		.size = a_size };
	}

	/** @brief Initialize a specialization constant info structure to pass to a shader stage */
	[[nodiscard]] constexpr VkSpecializationInfo specializationInfo(uint32_t a_mapEntryCount,
		const VkSpecializationMapEntry* a_mapEntries, size_t a_dataSize, const void* a_data)
	{
		return VkSpecializationInfo{
		.mapEntryCount = a_mapEntryCount,
		.pMapEntries = a_mapEntries,
		.dataSize = a_dataSize,
		.pData = a_data };
	}

	/** @brief Initialize a specialization constant info structure to pass to a shader stage */
	[[nodiscard]] constexpr VkSpecializationInfo specializationInfo(const std::vector<VkSpecializationMapEntry>& a_mapEntries,
		size_t a_dataSize, const void* a_data)
	{
		return VkSpecializationInfo{
		.mapEntryCount = static_cast<uint32_t>(a_mapEntries.size()),
		.pMapEntries = a_mapEntries.data(),
		.dataSize = a_dataSize,
		.pData = a_data };
	}

	// Ray tracing related
	[[nodiscard]] constexpr VkAccelerationStructureGeometryKHR accelerationStructureGeometryKHR()
	{
		return VkAccelerationStructureGeometryKHR{
		.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR,
		.pNext = nullptr };
	}
}