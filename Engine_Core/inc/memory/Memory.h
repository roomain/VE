#pragma once
/***********************************************
* @headerfile Memory.h
* @date 18 / 11 / 2025
* @author Roomain
************************************************/
#include <vector>
#include "vk_mem_alloc.h"
#include "memory/MemoryStructures.h"
#include "utils/VulkanContext.h"

namespace Vulkan::Memory
{
    /***********************************************
    * ALLOCATIONS
    ************************************************/
    /*@brief create vulkan buffer in Gpu / Device Memory shared by queues */
    [[nodiscard]] Buffer allocateBuffer(const VE_DeviceContext& a_context, const AllocationParameter& a_allocParameters);

    /*@brief create temporary buffer before sending to Gpu*/
    [[nodiscard]] Buffer allocateStagingBuffer(const VE_DeviceContext& a_context, const size_t& a_bufferByteSize);

    /*@brief create image in Gpu / Device Memory*/
    [[nodiscard]] Image allocateImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo);

    /*@brief create temporary image before sending to Gpu*/
    [[nodiscard]] Image allocateStagingImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo);
    
    /***********************************************
    * WRITE STAGING
    ************************************************/
    /*@brief write cpu buffer to staging memory*/
    template<template<typename> class Container, typename Type>
    void writeStagingBuffer(const VE_DeviceContext& a_context, Buffer& a_stagingBuffer, const Container<Type>& inputData, const StagingCopyParameter& a_parameters)
    {
        VK_CHECK_EXCEPT(vmaCopyMemoryToAllocation(a_context.m_memAllocator, a_parameters.data(inputData.data()), 
            staging.m_Alloc, a_parameters.destinationOffset(), a_parameters.memorySize(inputData)))
    }

    /*@brief write cpu data to staging memory*/
    template<typename Type>
    void writeStagingBuffer(const VE_DeviceContext& a_context, Buffer& a_stagingBuffer, const Type& inputData)
    {
        const size_t memSize = sizeof(Type);
        VK_CHECK_EXCEPT(vmaCopyMemoryToAllocation(a_context.m_memAllocator, &inputData, staging.m_Alloc, 0, memSize))
    }
    

    /***********************************************
    * TRANSFER TO GPU/DEVICE
    ************************************************/
    /*@brief transfer staging data to gpu*/
    template<VkBufferUsageFlags BufferFlag>
    void copyToGpu(const TransferParameter& a_staging, const VE_DeviceContext& a_ctx, VkCommandBuffer& a_cmdBuffer, Buffer& a_gpuBuffer)
    {
        // create GPU device buffers
        const VE_AllocationParameter allocParam{
            .m_size = a_staging.m_buffer.m_memorySize,
            .m_usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | BufferFlag,
            .m_allocFlag = 0
        };

        // allocate gpu memory
        a_gpuBuffer = allocateBuffer(a_ctx, allocParam);
        a_gpuBuffer.m_memorySize = a_staging.m_buffer.m_memorySize;

        // copy buffer
        const VkBufferCopy bufferRegion
        {
            .srcOffset = a_staging.m_srcOffset,
            .dstOffset = a_staging.m_destOffset,
            .size = a_staging.m_buffer.m_memorySize,
        };

        vkCmdCopyBuffer(a_cmdBuffer, a_staging.m_buffer, a_gpuBuffer.m_buffer, 1, &bufferRegion);
    }

    
    /***********************************************
    * RELEASE MEMORY
    ************************************************/
   /*@brief release buffer*/
    void releaseBuffer(const VE_DeviceContext& a_context, Buffer& a_buffer);

    /*@brief release image*/
    void releaseImage(const VE_DeviceContext& a_context, Image& a_image);
}

