#pragma once
/***********************************************
* @headerfile VE_GraphicalPipeline.h
* @date 17 / 09 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <type_traits>
#include <concepts>
#include <utility>
#include "notCopiable.h"
#include "rendering/VE_Pipeline.h"


struct PipelineContext
{
	VkSampleCountFlagBits m_multisample = VK_SAMPLE_COUNT_1_BIT;
	uint32_t m_ctrlPoints = 1;
	uint32_t m_viewportCount = 1;
	uint32_t m_scissorCount = 1;
	VkBool32 m_blendEnable = VK_FALSE;
	VkLogicOp m_blendOperator = VK_LOGIC_OP_CLEAR;
	std::vector<VkPushConstantRange> m_pushConstant;
	std::vector<VkDynamicState> m_dynStates; 
	std::vector<VkPipelineColorBlendAttachmentState> m_colorAttachments;
};

/*@brief base class of graphica pipeline*/
class VE_GraphicalPipeline : public VE_Pipeline
{
public:explicit 
	VE_GraphicalPipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile);
	~VE_GraphicalPipeline() override = default;
	virtual bool setup(const VE_ShaderPtr& a_shader, const PipelineContext& a_renderingCtxt) = 0;
};


/*@brief specific shared pointer accepting only graphical pipeline*/
template<typename Type> requires std::is_base_of_v<VE_GraphicalPipeline, Type>
class SharedPipeline : public std::shared_ptr<Type>
{
public:
	SharedPipeline() = default;
	SharedPipeline(std::shared_ptr<Type>&& a_pointer)noexcept : std::shared_ptr<Type>(std::move(a_pointer)) {}
};


template<typename T, typename Base>
concept IsPointerOf = requires {
	typename T::element_type;
}&& std::derived_from<typename T::element_type, Base>;


template<typename T>
concept HasPipeline = requires{
	{ T::GetPipeline() } -> IsPointerOf<VE_GraphicalPipeline>;
};


/*#define DECLARE_PIPELINE(pipeline) \
private:\
	static SharedPipeline<pipeline> s_pipeline;\
public: \
	static SharedPipeline<pipeline> GetPipeline(){ return s_pipeline; }


#define IMPL_PIPELINE(pipeline, classname) \
SharedPipeline<pipeline> classname::s_pipeline = std::make_shared<pipeline>();*/