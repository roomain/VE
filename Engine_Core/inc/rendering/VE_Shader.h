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
    std::vector<VkDescriptorSetLayoutBinding> m_layoutBindings;
    VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;

public:
    NOT_COPIABLE(VE_Shader)
    explicit VE_Shader(const VE_DeviceContext& a_ctxt);
    virtual ~VE_Shader()override;
    void cleanup();
    void loadShaderSpirV(const std::string& a_filename, const VkShaderStageFlagBits a_stage);
    void createDescriptorUniformTexelBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorStorageTexelBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorUniformBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorStorageBufferBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorUniformBufferDynBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorStorageBufferDynBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorSampledImageBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorStorageImageBindings(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    
    //void updateDescriptorUniformBuffer(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    //void updateDescriptorStorageBuffer(const VkShaderStageFlags a_shaderFlag, const uint32_t a_bind, const uint32_t a_descriptorCount = 1);
    void createDescriptorLayouts();
};
