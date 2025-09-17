#include "pch.h"
#include <fstream>
#include "rendering/VE_Shader.h"


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

	for(const auto& layout : m_descriptorSetLayouts)
		vkDestroyDescriptorSetLayout(m_vkCtxt.m_logicalDevice, layout, nullptr);

	m_descriptorSetLayouts.clear();
	m_shaderStageCreateInfo.clear();
	m_mpShaderModules.clear();
}


std::span<VkPipelineShaderStageCreateInfo> VE_Shader::pipelineShaderStages() noexcept
{
	return std::span<VkPipelineShaderStageCreateInfo>{m_shaderStageCreateInfo};
}

std::span<VkDescriptorSetLayout> VE_Shader::pipelineDescriptorSetLayouts() noexcept
{
	return std::span<VkDescriptorSetLayout>{m_descriptorSetLayouts};
}

void VE_Shader::loadShaderSpirV(const std::string_view& a_filename, const VkShaderStageFlagBits a_stage)
{
	if (std::ifstream fileStream(std::string(a_filename), std::ios::binary | std::ios::in | std::ios::ate); fileStream.is_open())
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

void VE_Shader::addDescriptorSetBinding(const int a_setIndex, const VkDescriptorSetLayoutBinding& a_binding)
{
	m_layoutBindingsPerSet[a_setIndex].emplace_back(a_binding);
}

void VE_Shader::createDescriptorLayouts()
{
	for(const auto& [setIndex, bindings] : m_layoutBindingsPerSet)
	{
		VkDescriptorSetLayoutCreateInfo descriptorLayoutCI = Vulkan::Initializers::descriptorSetLayoutCreateInfo(bindings);
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		vkCreateDescriptorSetLayout(m_vkCtxt.m_logicalDevice, &descriptorLayoutCI, nullptr, &descriptorSetLayout);
		m_descriptorSetLayouts.push_back(descriptorSetLayout);
	}
}