
#pragma once

///////////////////////////////////////////////////////////////
// Includes												     //
///////////////////////////////////////////////////////////////

#include <chrono>
#include <thread>
#include <utility>

#include <dB\ERET.hpp>

///////////////////////////////////////////////////////////////
// Libraries                                                 //
///////////////////////////////////////////////////////////////

#pragma comment(lib,"glfw\\glfw3.lib")

#ifdef OWN_USE_VULKAN
#pragma comment(lib,"vulkan\\vulkan-1.lib")
#endif // OWN_USE_VULKAN


///////////////////////////////////////////////////////////////
// Switches												     //
///////////////////////////////////////////////////////////////

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


///////////////////////////////////////////////////////////////
// Includes												     //
///////////////////////////////////////////////////////////////

#ifdef OWN_USE_VULKAN
#include <vulkan\vulkan.hpp>
#endif // OWN_USE_VULKAN

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

/*
#include <glm/common.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
*/


#ifdef OWN_USE_VULKAN

#endif // OWN_USE_VULKAN

#ifndef __DB_GLFW_HPP
#define __DB_GLFW_HPP

namespace dB::glfw
{
	Eret Init()
	{
		return Eret( "GLFW Initialisation", glfwInit() ? 0 : -1);
	}

	template<typename Functor>
	Eret CreateWindow(GLFWwindow** wnd, const std::string title, const glm::ivec2 res,
		Functor setWindowHints = [](void)->void {}, GLFWmonitor* mon = nullptr, GLFWwindow* share = nullptr)
	{
		if (*wnd)
			glfwDestroyWindow(*wnd);

		glfwDefaultWindowHints();
		if (setWindowHints)
			setWindowHints();

		*wnd = glfwCreateWindow(res.x, res.y, title.c_str(), mon, share);

		return Eret("GLFW Window creation", *wnd ? 0 : -1);
	}
	/*
	template<typename Functor>
	GLFWwindow* CreateWindow(std::string_view title, const glm::ivec2 res,
		Functor setWindowHints = [](void)->void {}, GLFWmonitor* mon = nullptr, GLFWwindow* share = nullptr)
	{
		GLFWwindow* wnd{};
		CreateWindow(&wnd, title, res, setWindowHints, mon, share);
		return wnd;
	}*/

	/*
	template<typename Functor>
	class _Window
	{
	private:
		GLFWwindow* wnd{};

		Functor SetWndHints;
	public:
		_Window(Functor setWindowHints = []() {}) : wnd(nullptr), SetWndHints(setWindowHints)
		{

		}

		//Window();
		~_Window();

		Eret Create()
		{
			SetWndHints();
		}

		GLFWwindow* Ptr() const
		{
			return this->wnd;
		}
	};

	template<typename T>
	typedef _Window<decltype(T)> Window (T);
	/**/
}

#endif // !__DB_GLFW_HPP

