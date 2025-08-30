#pragma once
/***********************************************
* @headerfile VE_PipelineCache.h
* @date 20 / 08 / 2025
* @author Roomain
************************************************/
#include <string>
#include <array>
#include "vulkan/vulkan.hpp"
#include "utils/VulkanContext.h"

/*@brief cache memory for vulkan pipeline*/
class VE_PipelineCache : public VulkanObject<VE_DeviceContext>
{
    friend class VE_Pipeline;
private:
    struct VkPipelineCacheHeader
    {
        uint32_t headerSize;
        uint32_t headerVersion;
        uint32_t vendorID;
        uint32_t deviceID;
        std::array<uint8_t, 16> pipelineCacheUUID;
    };


    VkPipelineCache m_cache = VK_NULL_HANDLE;
    bool checkCache(const VkPipelineCacheHeader& a_header);
    void cleanup();

public:
    VE_PipelineCache() = delete;
    explicit VE_PipelineCache(const VE_DeviceContext& a_ctxt);
    ~VE_PipelineCache()final;
    bool loadCache(const std::string& a_filename);
    bool saveCache(const std::string& a_filename);
};
