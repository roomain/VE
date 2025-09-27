#pragma once
/***********************************************
* @headerfile VE_Shader.h
* @date 04 / 08 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "utils/VulkanContext.h"
#include "notCopiable.h"


struct ShaderFile
{
    VkShaderStageFlagBits m_shaderStage;
    std::string m_shaderSpirVFile;
};

/*@brief represents a shader in vulkan engine*/
/*@brief Usage is :
*   1 - loadShaderSpirV
*   2 - addDescriptorSetBinding
*   3 - createDescriptorLayouts
*   4 - createDescriptorSets 
*/
class VE_Shader : public VulkanObject<VE_DeviceContext>
{
    friend class VE_ShadersDatabase;
private:
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStageCreateInfo;
    std::unordered_map<VkShaderStageFlagBits, VkShaderModule> m_mpShaderModules;    /*!< shader per type*/

	using DescriptorSetLayoutBindings = std::vector<VkDescriptorSetLayoutBinding>;
    std::unordered_map<int, DescriptorSetLayoutBindings> m_mpLayoutBindingsPerSet;  /*!< layout bindings per layout id*/

    VkDescriptorPool m_sharedpool = VK_NULL_HANDLE;                                 /*!< shared descriptor pool*/
    std::unordered_map<int, VkDescriptorSet> m_mpDescriptorSet;                     /*!< descriptor set per id*/

    std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;


    void loadShaderSpirV(const ShaderFile& a_file);
    [[nodiscard]] bool createDescriptorSets(VkDescriptorPool a_descPool);
    void fill(std::unordered_map<VkDescriptorType, VkDescriptorPoolSize>& a_poolSize)const;

public:
    NOT_COPIABLE(VE_Shader)
    explicit VE_Shader(const VE_DeviceContext& a_ctxt);
    ~VE_Shader()override;
    void cleanup();
    void releaseDescriptoSet();
    [[nodiscard]] std::span<VkPipelineShaderStageCreateInfo> pipelineShaderStages() noexcept;
    [[nodiscard]] std::span<VkDescriptorSetLayout> pipelineDescriptorSetLayouts() noexcept;

    void addDescriptorSetBinding(const int a_setIndex, const VkDescriptorSetLayoutBinding& a_binding);
    void createDescriptorLayouts();
};
