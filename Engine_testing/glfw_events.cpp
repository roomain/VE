#include "GLFW/glfw3.h"

int eventLoop(GLFWwindow* a_window)
{
	int iRet = 0;
	while (!glfwWindowShouldClose(a_window))
	{
		glfwPollEvents(); // get event
		// process events here if needed
	}
	return iRet;
}