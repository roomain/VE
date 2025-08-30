#pragma once
/***********************************************
* @headerfile VE_Pipeline.h
* @date 30 / 08 / 2025
* @author Roomain
************************************************/
#include "utils/VulkanContext.h"

class VE_PipelineCache;

/*@brief base class of pipeline*/
class VE_Pipeline : public VulkanObject<VE_DeviceContext>
{
private:
	// todo

	VE_Pipeline(const VE_DeviceContext& a_ctxt);

public:
	virtual ~VE_Pipeline();
	void loadFromCache(const VE_PipelineCache& a_cache);
};

