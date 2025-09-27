#pragma once
/***********************************************
* @headerfile VE_ShadersDatabase.h
* @date 26 / 09 / 2025
* @author Roomain
************************************************/
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.hpp>
#include "utils/VulkanContext.h"
#include "notCopiable.h"

class VE_Shader;
using VE_ShaderPtr = std::shared_ptr<VE_Shader>;
struct ShaderFile;


/*@brief shader database for a device*/
/*@brief Usage is :
*   1 - registerShaderProgram
*   2 - addDescriptorSetBinding for all shaders
*   3 - createDescriptorLayouts for all shaders
*   4 - createDesciptorSets
*/

class VE_ShadersDatabase : public VulkanObject<VE_DeviceContext>
{
private:
	std::unordered_map<uint32_t, VE_ShaderPtr> m_mpShaderDatabase;  /*!< shader database*/
	std::vector<VkDescriptorPool> m_descriptorPool;					/*!< descriptor pools*/
	std::vector<uint32_t> m_shadersWithoutDescriptorset;			/*!< list of shader without descriptor set => need new descriptor pool*/

public:
	NOT_COPIABLE(VE_ShadersDatabase);

	void cleanup();

	/*@brief create shader program from files*/
	/*@return the created shader*/
	VE_ShaderPtr registerShaderProgram(const uint32_t& a_shaderId, const std::span<ShaderFile>& a_shaderFiles);
	/*@brief create descriptor pool for shader with no descriptorset an create descriptor set for all shaders*/
	void createDesciptorSets();
};