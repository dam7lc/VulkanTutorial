//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/vec4.hpp>
//#include <glm/mat4x4.hpp>
//
//#include <iostream>
//
//int main() {
//	glfwInit();
//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//
//	GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);
//	uint32_t extensionCount = 0;
//	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
//
//	std::cout << extensionCount << " extensions supported" << std::endl;
//
//	glm::mat4 matrix; 
//	glm::vec4 vec;
//	auto test = matrix * vec;
//
//	while (!glfwWindowShouldClose(window)) {
//		glfwPollEvents();
//	}
//
//	glfwDestroyWindow(window);
//
//	glfwTerminate();
//
//	return 0;
//
//
//}

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	VkInstance instance;
	int intWidth = 800;
	int intHeight = 600;
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_LUNARG_standard_validation"
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else 
		const bool enableValidationLayers = true;
	#endif


	GLFWwindow* window;
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	uint32_t extensionCount = 0;
	

	void initWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		window = glfwCreateWindow(intWidth, intHeight, "Vulkan Triangle Tutorial", nullptr, nullptr);
	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	bool checkValidationLayerSupport(){
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

	void createInstance() {
		if(enableValidationLayers && !checkValidationLayerSupport()){
			throw std::runtime_error("Validation layers requested, but unavailable!");
		}

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "DEngine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

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
};

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