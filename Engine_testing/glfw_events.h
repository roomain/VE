#pragma once
/***********************************************
* @headerfile glfw_events.h
* @date 01 / 08 / 2025
* @author Roomain
************************************************/
#include "GLFW/glfw3.h"

class VE_RenderingCtx;

int eventLoop(GLFWwindow* a_window, VE_RenderingCtx& a_ctx);
