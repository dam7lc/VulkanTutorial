
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>
#include <vector>

class HelloTriangleApplication {
public:
	void run();

private:


	GLFWwindow* window;
	VkInstance instance;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else 
		const bool enableValidationLayers = true;
	#endif

	bool checkValidationLayerSupport();

	void cleanup();

	void createInstance();

    std::vector<const char*> getRequiredExtensions();

	void initWindow();

	void initVulkan();

	void mainLoop();
};
