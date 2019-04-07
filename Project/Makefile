 
VULKAN_SDK_PATH = /home/dam7lc/VulkanSDK/x86_64/
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan
VulkanTest: Source.cpp
	g++ $(CFLAGS) -o VulkanTest Source.cpp $(LDFLAGS)
	
.PHONY: test clean

test: VulkanTest
	LD_LIBRARY_PATH=$(VULKAN_SDK_PATH)/lib VK_LAYER_PATH=$(VULKAN_SDK_PATH)/etc/explicit_layer.d ./VulkanTest
	
clean:
	rm -f VulkanTest
