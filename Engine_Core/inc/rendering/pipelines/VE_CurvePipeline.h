#pragma once
/***********************************************
* @headerfile VE_CurvePipeline.h
* @date 21 / 09 / 2025
* @author Roomain
************************************************/
#include "rendering/pipelines/VE_GraphicalPipeline.h"
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

public:
	bool setup(const VE_ShaderPtr& a_shader, const PipelineContext& a_renderingCtxt) override;
};