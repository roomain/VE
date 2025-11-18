// pch.h : Il s'agit d'un fichier d'en-tête précompilé.
// Les fichiers listés ci-dessous sont compilés une seule fois, ce qui améliore les performances de génération des futures builds.
// Cela affecte également les performances d'IntelliSense, notamment la complétion du code et de nombreuses fonctionnalités de navigation du code.
// Toutefois, les fichiers listés ici sont TOUS recompilés si l'un d'entre eux est mis à jour entre les builds.
// N'ajoutez pas de fichiers fréquemment mis à jour ici, car cela annule les gains de performance.

#ifndef PCH_H
#define PCH_H

// ajouter les en-têtes à précompiler ici
#include "framework.h"

#include "logger.h"
#include "bitOperators.h"
#include "string_utils.h"

#include "parameters/VE_Parameter.h"
#include "parameters/VE_QueueCreateInfo.h"

#include "Capabilities/VE_InstanceCapabilities.h"
#include "Capabilities/VE_DeviceCapabilities.h"
#include "Capabilities/VE_CapabilitiesVisitor.h"

#include "utils/Vulkan_macros.h"
#include "utils/vulkan_enum_to_string.h"
#include "utils/vulkan_string_to_enum.h"
#include "utils/VulkanContext.h"
#include "utils/VulkanInitializers.h"
#include "utils/VulkanPipelineInitializers.h"
#include "utils/VulkanCmdInitializers.h"
#include "utils/VulkanBufferInitializers.h"
#include "utils/VulkanImageInitializers.h"
#include "utils/VulkanShaderInitializers.h"

#include "memory/Memory.h"

#include "VE_Device.h"
#include "VE_Application.h"
#include "VE_GraphicalDevice.h"
#include "VE_SwapChain.h"

#endif //PCH_H
