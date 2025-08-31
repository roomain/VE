#pragma once
/***********************************************
* @headerfile VE_Shader.h
* @date 04 / 08 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <memory>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "utils/VulkanContext.h"
#include "notCopiable.h"

/*@brief represents a shader in vulkan engine*/
class VE_Shader : public VulkanObject<VE_DeviceContext>
{
    friend class VE_Pipeline;
private:
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStageCreateInfo;
    std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_mpShaderModules;

	using DescriptorSetLayoutBindings = std::vector<VkDescriptorSetLayoutBinding>;
    std::unordered_map<int, DescriptorSetLayoutBindings> m_layoutBindingsPerSet;
    std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;

public:
    NOT_COPIABLE(VE_Shader)
    explicit VE_Shader(const VE_DeviceContext& a_ctxt);
    ~VE_Shader()override;
    void cleanup();
    void loadShaderSpirV(const std::string& a_filename, const VkShaderStageFlagBits a_stage);    
    void addDescriptorSetBinding(const int a_setIndex, const VkDescriptorSetLayoutBinding& a_binding);
    void createDescriptorLayouts();
};
