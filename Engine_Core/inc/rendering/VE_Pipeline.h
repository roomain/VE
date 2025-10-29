#pragma once
/***********************************************
* @headerfile VE_Pipeline.h
* @date 30 / 08 / 2025
* @author Roomain
************************************************/
#include <vector>
#include <string_view>
#include <vulkan/vulkan.h>
#include "utils/VulkanContext.h"
#include "VE_FwdDecl.h"

/*@brief base class of pipeline*/
class VE_Pipeline : public VulkanObject<VE_DeviceContext>
{
private:
	struct VkPipelineCacheHeader
	{
		uint32_t headerSize = 0;
		uint32_t headerVersion = 0;
		uint32_t vendorID = 0;
		uint32_t deviceID = 0;
		std::array<uint8_t, 16> pipelineCacheUUID;
	};
	VkPipelineCacheHeader m_header;

protected:
	VkPipelineCache m_cache = VK_NULL_HANDLE;
	VkPipeline m_pipeline = VK_NULL_HANDLE;					/*!< vulkan pipeline handle*/
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;		/*!< vulkla pipeline layout handle*/

	/*@brief create cache from file*/
	bool loadCache(const std::string_view& a_filename);
	bool checkCache()const;

public:
	explicit VE_Pipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile = "");
	virtual ~VE_Pipeline();
	virtual void cleanup();
	[[nodiscard]] constexpr bool isValid()const noexcept { return m_pipeline != VK_NULL_HANDLE; }
	/*@brief save cache*/
	bool saveCache(const std::string_view& a_filename);
};

#define IMPL_PIPELINE_FILE(classname) \
	static const inline std::string_view s_cacheFile = std::string(#classname) + ".cache";
