#pragma once
/***********************************************
* @headerfile VE_RenderComponent.h
* @date 30 / 08 / 2025
* @author Roomain
************************************************/
#include <type_traits>
#include "rendering/components/VE_Component.h"
#include "rendering/pipelines/VE_GraphicalPipeline.h"

/*@brief base class for renderable class*/
template<typename Pipeline> requires std::is_base_of_v<VE_GraphicalPipeline, Pipeline>
class VE_RenderComponent
{
	friend class VE_RenderGraphTask;
private:
	std::shared_ptr<Pipeline> m_pipeline;

public:
	std::shared_ptr<VE_GraphicalPipeline> pipeline()const final
	{
		return m_pipeline;
	}

	void createPipeline(const VE_DeviceContext& a_ctxt)const
	{
		m_pipeline = std::make_shared<Pipeline>(a_ctxt);
	}
};
