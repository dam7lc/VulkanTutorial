CFLAGS = -std=c++17 -O2
LDFLAGS = -lglfw -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi -lvulkan
VulkanTest: Source.cpp
	g++ $(CFLAGS) -o VulkanTest Source.cpp $(LDFLAGS)
	
.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
