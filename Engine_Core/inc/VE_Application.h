#pragma once
/***********************************************
* @headerfile VE_Application.h
* @date 14 / 07 / 2025
* @author Roomain
************************************************/
#include <memory>
#include <functional>
#include "vulkan/vulkan.hpp"
#include "logger.h"
#include "Capabilities/VE_InstanceCapabilities.h"
#include "core_globals.h"

using CreateSurfaceCallback = std::function<void(VkInstance, const VkAllocationCallbacks*, VkSurfaceKHR*)>;
using DeviceChoice = std::function<int(const std::vector<int>&)>;
class VE_GraphicalDevice;
class VE_ComputeDevice;
struct VE_DeviceContext;
struct VE_Parameter;

#pragma warning(push)
#pragma warning( disable : 4251 )
/*@brief base class for engine application*/
class VE_CORE_LIB VE_Application
{
private:
    static constexpr uint32_t ENGINE_VERSION = 1;
    
    VkInstance m_vulkanInstance = VK_NULL_HANDLE;                       /*!< vulkan instance*/
    std::vector<VkPhysicalDevice> m_physicalDevices;                    /*!< physical devices availables for m_vulkanInstance*/
    std::vector<std::shared_ptr<VE_GraphicalDevice>> m_graphicalDevices;/*!< graphical devices*/
    std::vector<std::shared_ptr<VE_ComputeDevice>> m_computeDevices;    /*!< compute devices*/
#pragma region Debug
    VkDebugUtilsMessengerEXT  m_debugMessenger = VK_NULL_HANDLE;
    static VkBool32 debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT a_messageSeverity,
        [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT a_messageTypes,
        const VkDebugUtilsMessengerCallbackDataEXT* a_pCallbackData,
        [[maybe_unused]] void* a_pUserData);
#pragma endregion
    static VE_InstanceCapabilities m_capabilities;                      /*!< capabilities*/
    CreateSurfaceCallback m_surfaceCreateCallback;                      /*!< surface creation callback depends of windows manager*/

    template<typename ChoiceFun> requires std::is_base_of_v<DeviceChoice, ChoiceFun>
    int chooseDevice(const ChoiceFun& a_chooseFun, const VE_DeviceParameter& a_devParameters, const VkSurfaceKHR a_surface = VK_NULL_HANDLE)const
    {
        std::vector<int> deviceIndice = m_capabilities.findSuitableDevice(a_devParameters, a_surface);
        if (deviceIndice.size() == 1)
        {
            return deviceIndice[0];
        }
        else if(deviceIndice.empty())
        {
            Logger::error("No suitable device found");
            return -1;
        }
        return a_chooseFun(deviceIndice);
    }

    void createVulkanDevice(VE_DeviceContext& a_ctxt, std::vector<int>& a_useQueuesFamilies, int& a_presentFamQueueIndex, const int a_devIndex, const VE_DeviceParameter& a_devParameters, VkSurfaceKHR a_surface = VK_NULL_HANDLE)const;

public:
    VE_Application(const VE_Parameter& a_parameter, const CreateSurfaceCallback a_surfCreate = nullptr);
    virtual ~VE_Application();

    [[nodiscard]] static const VE_InstanceCapabilities& capabilities();

    [[nodiscard]] uint32_t numGraphicalDevices()const noexcept;
    [[nodiscard]] std::shared_ptr<VE_GraphicalDevice> createGraphicalDevice(const DeviceChoice& a_choice, const VE_DeviceParameter& a_devParameters);
    [[nodiscard]] std::shared_ptr<VE_GraphicalDevice> getGraphicalDevice(const uint32_t a_index)const;

    [[nodiscard]] uint32_t numComputeDevices()const noexcept;
    [[nodiscard]] std::shared_ptr<VE_ComputeDevice> createComputeDevice(const DeviceChoice& a_choice, const VE_DeviceParameter& a_devParameters);
    [[nodiscard]] std::shared_ptr<VE_ComputeDevice> getComputeDevice(const uint32_t a_index)const;

};
#pragma warning(pop)