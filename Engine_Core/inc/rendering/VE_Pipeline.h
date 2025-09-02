#pragma once
/***********************************************
* @headerfile VE_Pipeline.h
* @date 30 / 08 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <vulkan/vulkan.h>
#include "utils/VulkanContext.h"
#include "VE_FwdDecl.h"

class VE_PipelineCache;

/*@brief base class of pipeline*/
class VE_Pipeline : public VulkanObject<VE_DeviceContext>
{
private:
	VkPipeline m_pipeline = VK_NULL_HANDLE;					/*!< vulkan pipeline handle*/
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;		/*!< vulkla pipeline layout handle*/

	VE_Pipeline(const VE_DeviceContext& a_ctxt);

protected:
	[[nodiscard]] virtual VkPipelineVertexInputStateCreateInfo internalCreateVertexInput() const = 0;
	[[nodiscard]] virtual VkPipelineInputAssemblyStateCreateInfo internalCreateInputAssembly()const = 0;
	[[nodiscard]] virtual VkPipelineTessellationStateCreateInfo internalCreateTesselation()const = 0;
	[[nodiscard]] virtual VkPipelineViewportStateCreateInfo internalCreateViewportState()const = 0;
	[[nodiscard]] virtual VkPipelineRasterizationStateCreateInfo internalCreateRasterization()const = 0;
	[[nodiscard]] virtual VkPipelineMultisampleStateCreateInfo internalCreateMultisampleState()const = 0;
	[[nodiscard]] virtual VkPipelineDepthStencilStateCreateInfo internalCreateDepthStencilState()const = 0;
	[[nodiscard]] virtual VkPipelineColorBlendStateCreateInfo internalCreateColorBlendState()const = 0;
	[[nodiscard]] virtual VkPipelineDynamicStateCreateInfo internalCreateDynamicState()const = 0;

public:
	virtual ~VE_Pipeline();
	virtual void cleanup();
	virtual void setup(VE_ShaderPtr a_shader);
	[[nodiscard]] constexpr bool isValid()const noexcept { return m_pipeline != VK_NULL_HANDLE; }
	void loadFromCache(const VE_PipelineCache& a_cache);
};

