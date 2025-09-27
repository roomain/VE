#include "pch.h"
#include <utility>
#include "rendering/VE_ShadersDatabase.h"
#include "rendering/VE_Shader.h"

void VE_ShadersDatabase::cleanup()
{
    for (const auto& [shaderId, shader] : m_mpShaderDatabase)
        shader->releaseDescriptoSet();

    for (auto& pool : m_descriptorPool)
        vkDestroyDescriptorPool(m_vkCtxt.m_logicalDevice, pool, nullptr);
}

VE_ShaderPtr VE_ShadersDatabase::registerShaderProgram(const uint32_t& a_shaderId, const std::span<ShaderFile>& a_shaderFiles)
{
    auto shader = std::make_shared<VE_Shader>(m_vkCtxt);
    for (const auto& shaderFile : a_shaderFiles)
        shader->loadShaderSpirV(shaderFile);
    m_mpShaderDatabase[a_shaderId] = shader;
    return shader;
}

void VE_ShadersDatabase::createDesciptorSets()
{

    std::unordered_map<VkDescriptorType, VkDescriptorPoolSize> mpPoolSizes;
    if (!m_shadersWithoutDescriptorset.empty())
    {
        // get pool sizes of shader without descriptorset
        for (const auto& shaderId : m_shadersWithoutDescriptorset)
            m_mpShaderDatabase[shaderId]->fill(mpPoolSizes);        
    }
    else if(m_descriptorPool.empty())
    {
        // get pool sizes of all shaders
        for (const auto& [shaderId, shader] : m_mpShaderDatabase)
            shader->fill(mpPoolSizes);        
    }


    // convert to vector
    std::vector<VkDescriptorPoolSize> vPoolSizes;
    for (const auto& [key, sizes] : mpPoolSizes)
        vPoolSizes.emplace_back(sizes);


    std::vector<uint32_t> shadersWithoutDescriptor;
    if (!vPoolSizes.empty())
    {
        auto& descriptorPool = m_descriptorPool.emplace_back(VkDescriptorPool{});
        VkDescriptorPoolCreateInfo descriptorPoolCI{
            .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
            .maxSets = static_cast<uint32_t>(vPoolSizes.size()),
            .poolSizeCount = static_cast<uint32_t>(vPoolSizes.size()),
            .pPoolSizes = vPoolSizes.data()
        };
        VK_CHECK_EXCEPT(vkCreateDescriptorPool(m_vkCtxt.m_logicalDevice, &descriptorPoolCI, nullptr, &descriptorPool));


        // create descriptorset of shaders
        if (!m_shadersWithoutDescriptorset.empty())
        {
            for (const auto& shaderId : m_shadersWithoutDescriptorset)
            {
                if (!m_mpShaderDatabase[shaderId]->createDescriptorSets(descriptorPool))
                    shadersWithoutDescriptor.emplace_back(shaderId);
            }
        }
        else
        {
            for (const auto& [shaderId, shader] : m_mpShaderDatabase)
            {
                if(!shader->createDescriptorSets(descriptorPool))
                    shadersWithoutDescriptor.emplace_back(shaderId);
            }
        }
    }

    std::swap(m_shadersWithoutDescriptorset, shadersWithoutDescriptor);
}