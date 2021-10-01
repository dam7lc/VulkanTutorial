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
	if(enableValidationLayers){
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	} else{
		createInfo.enabledLayerCount = 0;
	}
	
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	std::vector<VkExtensionProperties> extensions(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
	//VkResult result = ;

	std::cout << "available extensions:" << std::endl;

	for(const auto& extension : extensions){
		std::cout << "\t" << extension.extensionName << std::endl;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance");
	}
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

void HelloTriangleApplication::run(){
	initWindow();
	initVulkan();
	mainLoop();
	cleanup();
}

