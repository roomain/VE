#include "GLFW/glfw3.h"
#include "rendering/VE_RenderingCtx.h"

int eventLoop(GLFWwindow* a_window, VE_RenderingCtx& a_ctx)
{
	int iRet = 0;
	while (!glfwWindowShouldClose(a_window))
	{
		glfwPollEvents(); // get event
		// process events here if needed
		a_ctx.processRendering();
	}
	return iRet;
}