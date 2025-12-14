#pragma once
/***********************************************
* @headerfile VE_Component.h
* @date 30 / 08 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <type_traits>
#include "rendering/components/VE_IComponent.h"
#include "rendering/pipelines/VE_GraphicalPipeline.h"

/*@brief base class for renderable class*/
template<typename Pipeline> requires std::is_base_of_v<VE_GraphicalPipeline, Pipeline>
class VE_Component : public VE_IComponent
{
private:
	static std::shared_ptr<Pipeline> s_pipeline;

protected:
	/*@brief get device context static pielin must be created*/
	const VE_DeviceContext& context()const final { return s_pipeline->context(); }

	std::shared_ptr<VE_GraphicalPipeline> pipeline()const final
	{
		return s_pipeline;
	}

	void createPipeline(const VE_DeviceContext& a_ctxt) final
	{
		if(!s_pipeline)
			VE_Component<Pipeline>::s_pipeline = std::make_shared<Pipeline>(a_ctxt);
	}
};
