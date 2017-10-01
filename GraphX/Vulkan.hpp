
#ifndef OWN_USE_VULKAN
#define OWN_USE_VULKAN

#include <dB\ERET.hpp>
#include <dB\GraphX\GLFW.hpp>

#endif // !OWN_USE_VULKAN

#pragma once

///////////////////////////////////////////////////////////////
// Helper												     //
///////////////////////////////////////////////////////////////

namespace dB::vulkan
{
	// Used by vulkan::Init
	// https://vulkan-tutorial.com/Drawing_a_triangle/Setup/Validation_layers
	namespace validation_layer
	{	
		bool CheckSupport(const std::vector<const char*> layers)
		{
			// Get all available layers
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			// Check the requested ones
			for (const char* layerName : layers)
			{
				bool layerFound = false;

				//std::find(availableLayers.begin(), availableLayers.end(), layerName);

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound) {
					return false;
				}
			}

			return true;
		}

		const std::vector<const char*> defaultValidationLayers =
		{
			"VK_LAYER_LUNARG_standard_validation",
			"VK_LAYER_LUNARG_parameter_validation",
		};
	}

	namespace extensions
	{
		std::vector<const char*> GetRequired()
		{
			std::vector<const char*> extensions;

			unsigned int glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			for (unsigned int i = 0; i < glfwExtensionCount; i++) {
				extensions.push_back(glfwExtensions[i]);
			}

#ifndef NDEBUG
			extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif // !NDEBUG

			return extensions;
		}

		void Print()
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> extensions(extensionCount);

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			std::cout << "Vulkan - Available extensions:" << std::endl;

			for (const auto& extension : extensions)
			{
				std::cout << "\t> " << extension.extensionName << "\n";
			}
			std::cout << std::flush;
		}
	}
}

#include <algorithm>

namespace dB::vulkan
{
	dB::Eret inline Init(VkInstance* instance, const std::vector<const char*>& debugValidationLayers = validation_layer::defaultValidationLayers)
	{
		Eret eret("Vulkan initialisation");

		// -------------------------------
		// ---     Create Instance     ---
		// -------------------------------
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Set Extensions
		auto extensions = extensions::GetRequired();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		// -------------------------------
		// ---  Set Validation Layers  ---
		// -------------------------------
#ifndef NDEBUG
		createInfo.ppEnabledLayerNames = debugValidationLayers.data();
		createInfo.enabledLayerCount   = static_cast<uint32_t>(debugValidationLayers.size());

		if (!validation_layer::CheckSupport(debugValidationLayers))
		{
			return Eret(eret.msg.append(" - validation layer not available "), -2);
		}
#endif // !NDEBUG

		VkResult result = vkCreateInstance(&createInfo, nullptr, instance);
		if(result != VK_SUCCESS)
			return Eret(eret, result);



		// -------------------------------
		// ---  Create Logical Device  ---
		// -------------------------------



		// -------------------------------
		// ---    Create Swap Chain    ---
		// -------------------------------



		return Eret(eret, result);
	}

	int32_t devRatingFunctionDefault(VkPhysicalDevice dev)
	{
		VkPhysicalDeviceProperties deviceProperties{};
		VkPhysicalDeviceFeatures deviceFeatures{};
		vkGetPhysicalDeviceProperties(dev, &deviceProperties);
		vkGetPhysicalDeviceFeatures(dev, &deviceFeatures);

		if (!deviceFeatures.geometryShader)
			return -1;

		int32_t score = 0;

		score += deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ? 1000 : 0;

		return 10;
	}

	// -------------------------------
	// ---   Get Physical Device   ---
	// -------------------------------
	template <typename Functor>
	dB::Eret SelectPhysicalDevice(VkInstance instance, VkPhysicalDevice* dev, Functor devRatingFunction = dB::vulkan::devRatingFunctionDefault)
	{
		Eret eret("Vulkan physical device selection");

		// Get Devices
		uint32_t devCount = 0;
		vkEnumeratePhysicalDevices(instance, &devCount, nullptr);
		if(unlikely(devCount == 0))
			return Eret(eret, -1);

		std::vector<VkPhysicalDevice> devices(devCount);
		vkEnumeratePhysicalDevices(instance, &devCount, devices.data());

		// Rate them
		std::vector<int32_t> deviceScores(devCount);
		std::transform(devices.begin(), devices.end(), deviceScores.begin(), devRatingFunctionDefault);
		auto elem = std::max_element(deviceScores.begin(), deviceScores.end());
		size_t maxScoreIndex = std::distance(deviceScores.begin(), elem);

		if(unlikely(deviceScores.at(maxScoreIndex) < 0))
		{
			*dev = nullptr;
			return Eret(eret, -2);
		}

		*dev = devices.at(maxScoreIndex);
		return Eret(eret, 0);
	}

	VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
		auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pCallback);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, 
		const VkAllocationCallbacks* pAllocator) 
	{
		auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
		if (func != nullptr) {
			func(instance, callback, pAllocator);
		}
	}

	template <typename Functor>
	dB::Eret inline SetupDebugCallback(VkDebugReportCallbackEXT* callBackHandler, VkInstance instance, Functor debugCallback)
	{
#ifndef NDEBUG
		VkDebugReportCallbackCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
			VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | 
			VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		createInfo.pfnCallback = debugCallback;

		VkResult result = CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, callBackHandler);
		return Eret("Vulkan - Setup debug callback", result);
#endif // !NDEBUG
		return Eret("", 0);
	}
}