#include "pch.h"
#include <fstream>
#include <format>
#include <filesystem>
#include "rendering/VE_Pipeline.h"
#include "utils/VulkanPipelineInitializers.h"
#include "rendering/VE_Shader.h"

VE_Pipeline::VE_Pipeline(const VE_DeviceContext& a_ctxt, const std::string_view& a_cacheFile) : VulkanObject<VE_DeviceContext>{ a_ctxt }
{
    bool bCreateCache = !a_cacheFile.empty() && loadCache(a_cacheFile);
    if (!bCreateCache)
    {
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        pipelineCacheCreateInfo.initialDataSize = 0;
        pipelineCacheCreateInfo.pInitialData = nullptr;
        VK_CHECK_EXCEPT(vkCreatePipelineCache(m_vkCtxt.m_logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_cache))
    }
}

VE_Pipeline::~VE_Pipeline()
{
    m_bindingPoint = VK_PIPELINE_BIND_POINT_COMPUTE;
	cleanup();
}

void VE_Pipeline::cleanup()
{
	if (m_pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(m_vkCtxt.m_logicalDevice, m_pipelineLayout, nullptr);
		m_pipelineLayout = VK_NULL_HANDLE;
	}

	if(m_pipeline != VK_NULL_HANDLE )
	{
		vkDestroyPipeline(m_vkCtxt.m_logicalDevice, m_pipeline, nullptr);
		m_pipeline = VK_NULL_HANDLE;
	}

    if (m_cache != VK_NULL_HANDLE)
    {
        vkDestroyPipelineCache(m_vkCtxt.m_logicalDevice, m_cache, nullptr);
        m_cache = VK_NULL_HANDLE;
    }
}


bool VE_Pipeline::checkCache()const
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(m_vkCtxt.m_physicalDevice, &properties);
    return (properties.vendorID == m_header.vendorID) && (properties.deviceID == m_header.deviceID);
}

bool VE_Pipeline::loadCache(const std::string_view& a_filename)
{
    auto current = std::filesystem::current_path();
    const std::string strCurrent = current.generic_string() + "/pipeline";
    std::filesystem::path path(strCurrent);
    if (!std::filesystem::exists(path))
    {
        Logger::error(std::format("Folder does not exist: {}", strCurrent));
        return false;
    }

    std::string outputFile = path.generic_string() + "/" + std::string(a_filename);

    if (std::ifstream fileStream(outputFile, std::ios::binary | std::ios::in | std::ios::ate); fileStream.is_open())
    {
        const size_t fileSize = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        if (fileSize > 0)
        {
            // check
            // verify endianess
            fileStream.read(reinterpret_cast<char*>(&m_header), sizeof(VkPipelineCacheHeader));
            if (checkCache())
            {
                fileStream.seekg(0, std::ios::beg);
                std::vector<char> cacheData(fileSize);
                fileStream.read(cacheData.data(), fileSize);
                VkPipelineCacheCreateInfo pipelineCacheCreateInfo{};
                pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
                pipelineCacheCreateInfo.initialDataSize = cacheData.size();
                pipelineCacheCreateInfo.pInitialData = cacheData.data();
                VK_CHECK_LOG(vkCreatePipelineCache(m_vkCtxt.m_logicalDevice, &pipelineCacheCreateInfo, nullptr, &m_cache))
                return m_cache != VK_NULL_HANDLE;
            }
        }

    }
    return false;
}

bool VE_Pipeline::saveCache(const std::string_view& a_filename)
{
    if (m_cache != VK_NULL_HANDLE)
    {
        size_t cacheSize;
        VK_CHECK_EXCEPT(vkGetPipelineCacheData(m_vkCtxt.m_logicalDevice, m_cache, &cacheSize, nullptr))
        std::vector<char> cacheData(cacheSize);
        VK_CHECK_EXCEPT(vkGetPipelineCacheData(m_vkCtxt.m_logicalDevice, m_cache, &cacheSize, cacheData.data()))

        auto current = std::filesystem::current_path();
        const std::string strCurrent = current.generic_string() + "/pipeline";
        std::filesystem::path path(strCurrent);
        if (!std::filesystem::exists(path))
        {
            if (!std::filesystem::create_directory(path))
            {
                Logger::error(std::format("Can't create folder: {}", strCurrent));
                return false;
            }
        }

        std::string outputFile = path.generic_string() + "/" + std::string(a_filename);

        if (std::ofstream fileStream(std::string(a_filename), std::ios::binary | std::ios::out); fileStream.is_open())
        {
            fileStream.write(cacheData.data(), cacheSize);
            fileStream.flush();
            fileStream.close();
            return true;
        }
    }
    return false;
}

void VE_Pipeline::bind(VkCommandBuffer& a_cmdBuffer)
{
    vkCmdBindPipeline(a_cmdBuffer, m_bindingPoint, m_pipeline);
}