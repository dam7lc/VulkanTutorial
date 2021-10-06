#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <map>
#include "Source.h"


int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger){
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(func != nullptr){
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	} else{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData) {
		if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT){
			std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		}
		
		return VK_FALSE;
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator){
	auto func =  (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if(func != nullptr){
		func(instance, debugMessenger, pAllocator);
	}
}

bool HelloTriangleApplication::checkValidationLayerSupport(){
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers){
		bool layerFound = false;

		for(const auto& layerProperties : availableLayers){
			if(strcmp(layerName, layerProperties.layerName) == 0){
				layerFound = true;
				break;
			}
		}

		if(!layerFound){
			return false;
		}
	}
	return true;
}

void HelloTriangleApplication::cleanup() {
	if(enableValidationLayers){
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroyInstance(instance, nullptr);
	glfwDestroyWindow(window);
	glfwTerminate();
}

void HelloTriangleApplication::createInstance() {
	if(enableValidationLayers && !checkValidationLayerSupport()){
		throw std::runtime_error("Validation layers requested, but unavailable!");
	}

	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "DEngine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	auto req_extensions = getRequiredExtensions();

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(req_extensions.size());
	createInfo.ppEnabledExtensionNames = req_extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

	if(enableValidationLayers){
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	} else{
		createInfo.enabledLayerCount = 0;
		createInfo.pNext = nullptr;
	}
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	//VkResult result = ;

	// std::cout << "available extensions:" << std::endl;

	// for(const auto& extension : extensions){
	// 	std::cout << "\t" << extension.extensionName << std::endl;
	// }

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}
}


HelloTriangleApplication::QueueFamilyIndices HelloTriangleApplication::findQueueFamilies(VkPhysicalDevice device){
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
	
	int i = 0;
	for(const auto& queueFamily : queueFamilies){
		if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT){
			indices.graphicsFamily = i;
		}
		i++;
	}
	
	return indices;

}

std::vector<const char*> HelloTriangleApplication::getRequiredExtensions(){
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
	if(enableValidationLayers){
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	
	return extensions;
}

void HelloTriangleApplication::initVulkan(){
	createInstance();
	setupDebugMessenger();
	pickPhysicalDevice();
}

void HelloTriangleApplication::initWindow() {
	int intWidth = 800;
	int intHeight = 600;
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(intWidth, intHeight, "Vulkan Triangle Tutorial", nullptr, nullptr);
}


void HelloTriangleApplication::mainLoop(){
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void HelloTriangleApplication::pickPhysicalDevice(){
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	if(deviceCount == 0){
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	} else{
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
		
		std::multimap<int, VkPhysicalDevice> candidates;

		for(const auto& device : devices){
			int score = rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}
		if(candidates.rbegin()->first > 0){
			physicalDevice = candidates.rbegin()->second;
		} else{
			throw std::runtime_error("failed to find a suitable GPU");
		}

		if(physicalDevice == VK_NULL_HANDLE){
			throw std::runtime_error("failed to find a suitable GPU!");
		}
	}

	


}

void HelloTriangleApplication::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo){
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

int HelloTriangleApplication::rateDeviceSuitability(VkPhysicalDevice device){
	VkPhysicalDeviceProperties deviceProperties;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);


	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	int score = 0;
	if(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
		score += 1000;
	}
	score += deviceProperties.limits.maxImageDimension2D;

	if(!deviceFeatures.geometryShader){
		return 0;
	} 
	
	QueueFamilyIndices indices = findQueueFamilies(device);
	if(!indices.isComplete()){
		return 0;
	}

	return score;
}

void HelloTriangleApplication::run(){
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

void HelloTriangleApplication::setupDebugMessenger(){
	if(!enableValidationLayers) return;
	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if(CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS){
		throw std::runtime_error("failed to set up debug messenger");
	}
}
