// Engine_testing.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <filesystem>
#include "LoggerDisplay.h"
#include "VE_Application.h"
#include "CapabilitiesVisitor.h"
#include "ReflectionManager.h"
#include "ReflectionValue.h"
#include "parameters/VE_Parameter.h"
#include "reflection_converter.h"
#include "GLFW/glfw3.h"
#include "glfw_events.h"
#include "VE_GraphicalDevice.h"
#include "VE_QueueFamily.h"

void loadConfiguration()
{
	auto& deserializer = ReflectionManager::instance();
	ReflectionValue::registerCast<QueueFlag>(&convertEnum<QueueFlag>);
	auto localPath = std::filesystem::current_path();
	deserializer.load(localPath.string() + std::string(R"(\configuration)"), "configuration");
}

void initGLFW()
{
	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
	// Set GLFW error callback
	glfwSetErrorCallback([](int error, const char* description) {
		std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
	});

	// Set GLFW to NOT work with OpenGL
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

VE_Parameter vulkanParameters()
{
	// get glfw extensions names
	uint32_t glfwExtensionCount = 0;				// GLFW may require multiple extensions
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	VE_Parameter parameters;
	for(uint32_t i = 0; i < glfwExtensionCount; ++i)
	{
		parameters.extensions.emplace_back(glfwExtensions[i]);
	}
	parameters.bIsDebug = true;
	return parameters;
}

int deviceChoice(const std::vector<int>&)
{
	return 0;
}

int main()
{
	// load configuration
	loadConfiguration();

	// logger initialization
	ILogDisplayerPtr pLogDisplayer = std::make_shared<LoggerDisplay>();
	Logger::instance().addLogDisplayer(pLogDisplayer);

	// get capabilities
    CapabilitiesVisitor capVisitor;
    VE_Application::capabilities().visit(capVisitor);

	initGLFW();
	GLFWwindow* window = glfwCreateWindow(800, 800, "GLFW Vulkan", nullptr, nullptr);

	
	auto createSurfaceCallback = [window](VkInstance a_instance, const VkAllocationCallbacks* a_alloc, VkSurfaceKHR* a_surface)
		{
			glfwCreateWindowSurface(a_instance, window, a_alloc, a_surface);
		};	

	VE_Application app(vulkanParameters(), createSurfaceCallback);
	//-------------------------------------------------------------------------------------
	VE_DeviceParameter deviceParameters;

	auto device = app.createGraphicalDevice(&deviceChoice, deviceParameters);
#pragma warning(push)
#pragma warning( disable : 4189 )// because unsed local variable
	const auto swapchain = device->createNewSwapChain();
	const auto queue = device->createQueue(VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT);
	//VulkanBuffer buffer;
	//device->createStagingBuffer(50 * sizeof(int), buffer);
	//unsigned int numImages = swapchain->swapBufferCount();
#pragma warning(pop)
	//-------------------------------------------------------------------------------------


	int eventRet = eventLoop(window);
	glfwDestroyWindow(window);
	glfwTerminate();

	return eventRet;
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Astuces pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
