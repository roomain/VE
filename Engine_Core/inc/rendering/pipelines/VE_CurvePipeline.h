#pragma once
/***********************************************
* @headerfile VE_CurvePipeline.h
* @date 21 / 09 / 2025
* @author Roomain
************************************************/
#include "rendering/pipelines/VE_GraphicalPipeline.h"
#include "utils/VulkanDefines.h"
#include "rendering/VE_VertexTypes.h"
#include "core_globals.h"



#pragma warning(push)
#pragma warning(disable:4251) // disable warning for dll export/import

/*@brief rendering curve pipeline*/
class VE_CORE_LIB VE_CurvePipeline : public VE_GraphicalPipeline
{
private:
	static inline VertexInputBindingDescArray<1> s_vertexInputDesc = {
		VkVertexInputBindingDescription
		{
			.binding = 0,
			.stride = VertexTangentSize,
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
		}
	};

public:
	IMPL_PIPELINE_FILE(VE_CurvePipeline);
	explicit VE_CurvePipeline(const VE_DeviceContext& a_ctxt);
	bool setup(const VE_ShaderPtr& a_shader, const VE_GraphicalPipelineCtx& a_renderingCtxt) override;
};
#pragma warning(pop)