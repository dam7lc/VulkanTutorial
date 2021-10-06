#include <optional>

class HelloTriangleApplication {
public:
	void run();

private:


	GLFWwindow* window;
	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	#ifdef NDEBUG
		const bool enableValidationLayers = false;
	#else 
		const bool enableValidationLayers = true;
	#endif

	struct QueueFamilyIndices{
		std::optional<uint32_t> graphicsFamily;
		bool isComplete(){
			return graphicsFamily.has_value();
		}
	};

	bool checkValidationLayerSupport();

	void cleanup();

	void createInstance();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

	void initWindow();

	void initVulkan();

	void mainLoop();
	
	void pickPhysicalDevice();
	
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	
	int rateDeviceSuitability(VkPhysicalDevice device);
	
	void setupDebugMessenger();
	
};
