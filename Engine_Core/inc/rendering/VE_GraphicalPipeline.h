#pragma once
/***********************************************
* @headerfile VE_GraphicalPipeline.h
* @date 17 / 09 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_Pipeline.h"

/*@brief base class of graphica pipeline*/
class VE_GraphicalPipeline : public VE_Pipeline
{
protected:
	explicit VE_GraphicalPipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile);

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
	~VE_GraphicalPipeline() override = default;
	bool setup(const VE_ShaderPtr& a_shader, const std::span<VkPushConstantRange>& a_pushConst);
};