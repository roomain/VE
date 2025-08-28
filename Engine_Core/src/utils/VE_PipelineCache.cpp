#include "pch.h"
#include <fstream>
#include "utils/VE_PipelineCache.h"

VE_PipelineCache::VE_PipelineCache(const VE_DeviceContext& a_ctxt) : VulkanObject<VE_DeviceContext>(a_ctxt)
{
    //
}

VE_PipelineCache::~VE_PipelineCache()
{
    cleanup();
}

void VE_PipelineCache::cleanup()
{
    if (m_cache != VK_NULL_HANDLE)
        vkDestroyPipelineCache(m_vkCtxt.m_logicalDevice, m_cache, nullptr);
    m_cache = VK_NULL_HANDLE;
}

bool VE_PipelineCache::checkCache(const VkPipelineCacheHeader& a_header)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(m_vkCtxt.m_physicalDevice, &properties);
    return (properties.vendorID == a_header.vendorID) && (properties.deviceID == a_header.deviceID);
}

bool VE_PipelineCache::loadCache(const std::string& a_filename)
{
    if (std::ifstream fileStream(a_filename, std::ios::binary | std::ios::in | std::ios::ate); fileStream.is_open())
    {
        const size_t fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        if (fileSize > 0)
        {
            // check
            // verify endianess
            VkPipelineCacheHeader header;
            fileStream.read(reinterpret_cast<char*>(&header), sizeof(VkPipelineCacheHeader));
            if (checkCache(header))
            {
                fileStream.seekg(0, std::ios::beg);
                std::vector<char> cacheData(fileSize);
                fileStream.read(cacheData.data(), fileSize);
                VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
                pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
                pipelineCacheCreateInfo.initialDataSize = cacheData.size();
                pipelineCacheCreateInfo.pInitialData = cacheData.data();
                VK_CHECK_EXCEPT(vkCreatePipelineCache(m_vkCtxt.m_logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_cache))
                    return true;
            }
        }

    }
    return false;
}

bool VE_PipelineCache::saveCache(const std::string& a_filename)
{
    if (m_cache != VK_NULL_HANDLE)
    {
        size_t cacheSize;
        VK_CHECK_EXCEPT(vkGetPipelineCacheData(m_vkCtxt.m_logicalDevice, m_cache, &cacheSize, nullptr))
        std::vector<char> cacheData(cacheSize);
        VK_CHECK_EXCEPT(vkGetPipelineCacheData(m_vkCtxt.m_logicalDevice, m_cache, &cacheSize, cacheData.data()))

        if (std::ofstream fileStream(a_filename, std::ios::binary | std::ios::out); fileStream.is_open())
        {
            fileStream.write(cacheData.data(), cacheSize);
            fileStream.flush();
            fileStream.close();
            return true;
        }
    }
    return false;
}