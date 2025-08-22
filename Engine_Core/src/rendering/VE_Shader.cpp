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

void VE_Shader::addDescriptorSetBinding(const VkDescriptorSetLayoutBinding& a_binding)
{
	m_layoutBindings.emplace_back(a_binding);
}

void VE_Shader::createDescriptorLayouts()
{
	VkDescriptorSetLayoutCreateInfo descriptorLayoutCI = Vulkan::Initializers::descriptorSetLayoutCreateInfo(m_layoutBindings);
	vkCreateDescriptorSetLayout(m_vkCtxt.m_logicalDevice, &descriptorLayoutCI, nullptr, &m_descriptorSetLayout);
}