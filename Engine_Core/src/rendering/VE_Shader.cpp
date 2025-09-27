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

void VE_Shader::releaseDescriptoSet()
{
	if (m_sharedpool != VK_NULL_HANDLE)
	{
		std::vector<VkDescriptorSet> tempDesc{ m_mpDescriptorSet.size() };
		int index = 0;
		for (const auto& [setId, descriptoSet] : m_mpDescriptorSet)
		{
			tempDesc[index] = descriptoSet;
			++index;
		}

		vkFreeDescriptorSets(m_vkCtxt.m_logicalDevice, m_sharedpool, static_cast<uint32_t>(tempDesc.size()), tempDesc.data());
		m_mpDescriptorSet.clear();
		m_sharedpool = VK_NULL_HANDLE;
	}
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
	releaseDescriptoSet();
}


std::span<VkPipelineShaderStageCreateInfo> VE_Shader::pipelineShaderStages() noexcept
{
	return std::span<VkPipelineShaderStageCreateInfo>{m_shaderStageCreateInfo};
}

std::span<VkDescriptorSetLayout> VE_Shader::pipelineDescriptorSetLayouts() noexcept
{
	return std::span<VkDescriptorSetLayout>{m_descriptorSetLayouts};
}

void VE_Shader::loadShaderSpirV(const ShaderFile& a_file)
{
	if (std::ifstream fileStream(a_file.m_shaderSpirVFile, std::ios::binary | std::ios::in | std::ios::ate); fileStream.is_open())
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
			m_shaderStageCreateInfo.emplace_back(Vulkan::Initializers::pipelineShaderStageCreateInfo(a_file.m_shaderStage, shaderModule));
		}

	}
}

void VE_Shader::addDescriptorSetBinding(const int a_setIndex, const VkDescriptorSetLayoutBinding& a_binding)
{
	m_mpLayoutBindingsPerSet[a_setIndex].emplace_back(a_binding);
}

void VE_Shader::createDescriptorLayouts()
{
	for(const auto& [setIndex, bindings] : m_mpLayoutBindingsPerSet)
	{
		VkDescriptorSetLayoutCreateInfo descriptorLayoutCI = Vulkan::Initializers::descriptorSetLayoutCreateInfo(bindings);
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		vkCreateDescriptorSetLayout(m_vkCtxt.m_logicalDevice, &descriptorLayoutCI, nullptr, &descriptorSetLayout);
		m_descriptorSetLayouts.push_back(descriptorSetLayout);
	}
}

bool VE_Shader::createDescriptorSets(VkDescriptorPool a_descPool)
{
	if (m_descriptorSetLayouts.empty())
		return false;

	int setIndex = 0;
	m_sharedpool = a_descPool;
	for (const auto& descSetLayout : m_descriptorSetLayouts)
	{
		auto allocInfo = Vulkan::Initializers::descriptorSetAllocateInfo(a_descPool, &descSetLayout, 1);
		VK_CHECK_EXCEPT(vkAllocateDescriptorSets(m_vkCtxt.m_logicalDevice, &allocInfo, &m_mpDescriptorSet[setIndex]));
		++setIndex;
	}

	return true;
}

void VE_Shader::fill(std::unordered_map<VkDescriptorType, VkDescriptorPoolSize>& a_poolSize)const
{
	for (const auto& [setIndex, bindings] : m_mpLayoutBindingsPerSet)
	{
		for (const auto& desc : bindings)
		{
			if (auto iter = a_poolSize.find(desc.descriptorType); iter != a_poolSize.cend())
			{
				iter->second.descriptorCount++;
			}
			else
			{
				a_poolSize[desc.descriptorType].descriptorCount = 1;
				a_poolSize[desc.descriptorType].type = desc.descriptorType;
			}
		}
	}
}