#pragma once
/***********************************************
* @headerfile VE_CurvePipeline.h
* @date 21 / 09 / 2025
* @author Roomain
************************************************/
#include "rendering/VE_GraphicalPipeline.h"
#include "utils/VulkanDefines.h"
#include "rendering/VE_VertexTypes.h"

/*@brief rendering curve pipeline*/
class VE_CurvePipeline : public VE_GraphicalPipeline
{
private:
	static inline VertexInputBindingDescArray<1> s_vertexInputDesc = {
		VkVertexInputBindingDescription
		{
			.binding = 0,
			.stride = CurveVertexSize,
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		}
	};


protected:
	[[nodiscard]] virtual VkPipelineVertexInputStateCreateInfo internalCreateVertexInput() const override;
	[[nodiscard]] virtual VkPipelineInputAssemblyStateCreateInfo internalCreateInputAssembly()const override;
	[[nodiscard]] virtual VkPipelineTessellationStateCreateInfo internalCreateTesselation()const override;
	[[nodiscard]] virtual VkPipelineViewportStateCreateInfo internalCreateViewportState()const override;
	[[nodiscard]] virtual VkPipelineRasterizationStateCreateInfo internalCreateRasterization()const override;
	[[nodiscard]] virtual VkPipelineMultisampleStateCreateInfo internalCreateMultisampleState()const override;
	[[nodiscard]] virtual VkPipelineDepthStencilStateCreateInfo internalCreateDepthStencilState()const override;
	[[nodiscard]] virtual VkPipelineColorBlendStateCreateInfo internalCreateColorBlendState()const override;
	[[nodiscard]] virtual VkPipelineDynamicStateCreateInfo internalCreateDynamicState()const override;
};