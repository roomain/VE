#include "pch.h"
#include <fstream>
#include "rendering/VE_Shader.h"
#include "utils/VulkanShaderInitializers.h"


VE_Shader::VE_Shader(const VE_DeviceContext& a_ctxt) : VulkanObject{ a_ctxt }
{
	// todo
}

VE_Shader::~VE_Shader()
{
	cleanup();
	// todo
}

void VE_Shader::cleanup()
{
	// release shader modules
	for (const auto& shaderInfo : m_shaderStageCreateInfo)
		vkDestroyShaderModule(m_vkCtxt.m_logicalDevice, shaderInfo.module, nullptr);

	vkDestroyDescriptorSetLayout(m_vkCtxt.m_logicalDevice, m_descriptorSetLayout, nullptr);
	m_shaderStageCreateInfo.clear();
	m_mpShaderModules.clear();
}

void VE_Shader::loadShaderSpirV(const std::string& a_filename, const VkShaderStageFlagBits a_stage)
{
	if (std::ifstream fileStream(a_filename, std::ios::binary | std::ios::in | std::ios::ate); fileStream.is_open())
	{
		const size_t fileSize = fileStream.tellg();
		fileStream.seekg(0, std::ios::beg);

		if (fileSize > 0)
		{
			std::vector<char> fileData(fileSize);
			fileStream.read(fileData.data(), fileSize);
			fileStream.close();

			// Create a new shader module that will be used for pipeline creation
			VkShaderModuleCreateInfo shaderModuleCI = Vulkan::Initializers::shaderModuleCreateInfo(fileData);
			VkShaderModule shaderModule = VK_NULL_HANDLE;
			VK_CHECK_EXCEPT(vkCreateShaderModule(m_vkCtxt.m_logicalDevice, &shaderModuleCI, nullptr, &shaderModule))
			m_shaderStageCreateInfo.emplace_back(Vulkan::Initializers::pipelineShaderStageCreateInfo(a_stage, shaderModule));
		}

	}
}

void VE_Shader::createDescriptorUniformTexelBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorStorageTexelBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorUniformBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorStorageBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorUniformBufferDynBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorStorageBufferDynBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorSampledImageBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorStorageImageBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount)
{
	m_layoutBindings.emplace_back(VkDescriptorSetLayoutBinding{
		.binding = a_bind,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
		.descriptorCount = a_descriptorCount,
		.stageFlags = a_shaderFlag,
		.pImmutableSamplers = nullptr
	});
}

void VE_Shader::createDescriptorLayouts()
{
	VkDescriptorSetLayoutCreateInfo descriptorLayoutCI = Vulkan::Initializers::descriptorSetLayoutCreateInfo(m_layoutBindings);
	vkCreateDescriptorSetLayout(m_vkCtxt.m_logicalDevice, &descriptorLayoutCI, nullptr, &m_descriptorSetLayout);
}