#include "pch.h"
#include "memory/Memory.h"


namespace Vulkan::Memory
{
    /***********************************************
    * ALLOCATIONS
    ************************************************/

    Buffer allocateBuffer(const VE_DeviceContext& a_context, const AllocationParameter& a_allocParameters)
    {
        const bool isShared = !a_allocParameters.m_sharedQueue.empty();
        VkBufferCreateInfo stagingBufInfo = Vulkan::Initializers::bufferCreateInfo(
            Vulkan::Initializers::BufferCreateInfoParameters{
            .flags = 0,
            .size = a_allocParameters.m_size,
            .usage = a_allocParameters.m_usage,
            .sharingMode = isShared ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
            .familyCount = static_cast<uint32_t>(a_allocParameters.m_sharedQueue.size()),
            .familyIndex = isShared ? a_allocParameters.m_sharedQueue.data() : nullptr
            });

        const VmaAllocationCreateInfo bufAllocCreateInfo{
            .flags = a_allocParameters.m_allocFlag,
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
        };

        Buffer buffer;
        VK_CHECK_EXCEPT(vmaCreateBuffer(a_context.m_memAllocator,
            &stagingBufInfo,
            &bufAllocCreateInfo,
            &buffer.m_buffer,
            &buffer.m_Alloc,
            &buffer.m_AllocInfo))

            buffer.m_memorySize = a_allocParameters.m_size;
        return buffer;
    }

    Buffer allocateStagingBuffer(const VE_DeviceContext& a_context, const size_t& a_bufferByteSize)
    {
        VkBufferCreateInfo stagingBufInfo = Vulkan::Initializers::bufferCreateInfo(Vulkan::Initializers::BufferCreateInfoParameters{
            .flags = 0,
            .size = a_bufferByteSize,
            .usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .familyCount = 0,
            .familyIndex = nullptr
            });

        const VmaAllocationCreateInfo stagingBufAllocCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        Buffer stagingBuffer;
        VK_CHECK_EXCEPT(vmaCreateBuffer(a_context.m_memAllocator,
            &stagingBufInfo,
            &stagingBufAllocCreateInfo,
            &stagingBuffer.m_buffer,
            &stagingBuffer.m_Alloc,
            &stagingBuffer.m_AllocInfo))

            stagingBuffer.m_memorySize = a_bufferByteSize;
        return stagingBuffer;
    }

    Image allocateStagingImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo)
    {
        Image image;

        const VmaAllocationCreateInfo alloCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO
        };

        VK_CHECK_EXCEPT(vmaCreateImage(a_context.m_memAllocator, &a_imageCreateInfo, &alloCreateInfo, &image.m_image, &image.m_Alloc, &image.m_AllocInfo))
            return image;
    }

    Image allocateImage(const VE_DeviceContext& a_context, const VkImageCreateInfo& a_imageCreateInfo)
    {
        Image image;

        const VmaAllocationCreateInfo alloCreateInfo{
            .flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT,
            .usage = VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE
        };

        VK_CHECK_EXCEPT(vmaCreateImage(a_context.m_memAllocator, &a_imageCreateInfo, &alloCreateInfo, &image.m_image, &image.m_Alloc, &image.m_AllocInfo))
            return image;
    }


    /***********************************************
    * RELEASE MEMORY
    ************************************************/
    void releaseBuffer(const VE_DeviceContext& a_context, Buffer& a_buffer)
    {
        vmaDestroyBuffer(a_context.m_memAllocator, a_buffer.m_buffer, a_buffer.m_Alloc);
        a_buffer.m_buffer = VK_NULL_HANDLE;
        a_buffer.m_Alloc = VK_NULL_HANDLE;
        a_buffer.m_memorySize = 0;
    }

    void releaseImage(const VE_DeviceContext& a_context, Image& a_image)
    {
        vmaDestroyImage(a_context.m_memAllocator, a_image.m_image, a_image.m_Alloc);
        a_image.m_image = VK_NULL_HANDLE;
        a_image.m_Alloc = VK_NULL_HANDLE;
    }
}