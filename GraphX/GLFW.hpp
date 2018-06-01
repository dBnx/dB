
#pragma once


#ifdef NO_STATIC
#define GLFW_DLL
#else
#define GLEW_STATIC
#endif // NO_STATIC


///////////////////////////////////////////////////////////////
// Includes												     //
///////////////////////////////////////////////////////////////

#include <chrono>
#include <thread>
#include <utility>

#include <dB\ERET.hpp>
#include <dB\Util.hpp>

///////////////////////////////////////////////////////////////
// Libraries                                                 //
///////////////////////////////////////////////////////////////

//#pragma comment(lib,"glfw\\glfw3.lib")
#ifndef GLFW_DLL
#pragma comment(lib,"glfw3.lib")
#else
#pragma comment(lib,"glfw3dll.lib")
#endif // GLFW_DLL

#ifdef OWN_USE_VULKAN
#pragma comment(lib,"vulkan\\vulkan-1.lib")
#endif // OWN_USE_VULKAN

#ifdef OWN_USE_GLEW
#pragma comment(lib,"opengl32.lib")

#ifdef NO_STATIC
#pragma comment(lib,"glew32.lib")
#else 
#pragma comment(lib,"glew32s.lib")
#endif // NO_STATIC

#endif // OWN_USE_GLEW

///////////////////////////////////////////////////////////////
// Switches												     //
///////////////////////////////////////////////////////////////

#if !defined(GLM_FORCE_RADIANS) && !defined(GLM_FORCE_DEPTH_ZERO_TO_ONE) 
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#endif // OWN_USE_GLEW


///////////////////////////////////////////////////////////////
// Includes												     //
///////////////////////////////////////////////////////////////

#ifdef OWN_USE_GLEW
#include <GL/glew.h>
#endif // OWN_USE_GLEW

#ifdef OWN_USE_VULKAN
#include <vulkan\vulkan.hpp>
#endif // OWN_USE_VULKAN

#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#ifdef OWN_USE_GLEW
#include "GL.hpp"
#endif 

#ifdef OWN_USE_VULKAN

#endif // OWN_USE_VULKAN

#include <string>
#include <string_view>

#ifndef __DB_GLFW_HPP
#define __DB_GLFW_HPP

#ifdef OWN_USE_GLEW
namespace dB::glew
{
	Eret Init()
	{
		glewExperimental = GL_TRUE;
		return Eret("GLEW Initialisation", !glewInit() ? 0 : -1);
	}

	Eret GetError()
	{
		int32_t code = static_cast<int32_t>(glGetError());
		std::string msg = (const char*)glewGetErrorString(code);
		return Eret(-code, std::string("OpenGL : ") + msg);
	}
}

#endif // OWN_USE_GLEW

namespace dB::glfw
{
	namespace callback // Default callback
	{
		static void Key(GLFWwindow* wnd, int key, int scancode, int action, int mods)
		{
			switch (key)
			{
			case GLFW_KEY_END:
				glfwSetWindowShouldClose(wnd, GLFW_TRUE);
				break;
			default:
				break;
			}
		}

		static void WindowSize(GLFWwindow* window, int width, int height)
		{

			const GLdouble aspectRatio = float(width) / float(height);
			GLdouble xSpan = 1.f;
			GLdouble ySpan = 1.f;

			//xSpan *= aspectRatio;
			/*
			if (aspectRatio < 1.)
			ySpan = xSpan / aspectRatio;
			else
			xSpan *= aspectRatio;
			/**/

			//glOrtho(-xSpan, xSpan, -ySpan, ySpan, -1., 1.);
			glViewport(0, 0, width, height);
		}

		void SetCallbacks(GLFWwindow* wnd)
		{
			assert(wnd);

			glfwSetKeyCallback(wnd, Key);
			glfwSetWindowSizeCallback(wnd, WindowSize);
		}
	}
}

namespace dB::glfw
{
	Eret Init()
	{
		return Eret( "GLFW Initialisation", glfwInit() ? 0 : -1);
	}

	Eret CreateWindow(GLFWwindow** wnd, const std::string_view title = "dBnx - GLFW", std::function<void(void)> setWindowHints = [](void)->void {},
		const glm::ivec2 res = { 1024, 768 }, GLFWmonitor* mon = nullptr, GLFWwindow* share = nullptr)
	{
		if (*wnd)
			glfwDestroyWindow(*wnd);

		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (setWindowHints)
			setWindowHints();

		*wnd = glfwCreateWindow(res.x, res.y, std::string(title).c_str(), mon, share);
		glfwMakeContextCurrent(*wnd);

		glfwSetInputMode(*wnd, GLFW_STICKY_KEYS, GL_TRUE); 
		callback::SetCallbacks(*wnd);

		return Eret("GLFW Window creation", *wnd ? 0 : -1);
	}

	void Terminate()
	{
		glfwTerminate();
	}

	/*
	void CorrectAspectRatio(GLFWwindow* wnd) // Do not use
	{
		assert(0);
		int width, height;
		glfwGetWindowSize(wnd, &width, &height);

		const GLdouble aspectRatio = float(width) / float(height);
		GLdouble xSpan = 1.f;
		GLdouble ySpan = 1.f;

		xSpan *= aspectRatio;
		
		//if (aspectRatio < 1.)
		//ySpan = xSpan / aspectRatio;
		//else
		//xSpan *= aspectRatio;
		
		glLoadIdentity(); // The OpenGL state machine is bullshit
		glOrtho(-xSpan, xSpan, -ySpan, ySpan, -1., 1.);
	}
	/**/
	

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

/*
// GLFW/GLEW Simple Window 
#include <dB/dB.hpp>

#define OWN_USE_GLEW
#include <dB/GraphX/GLFW.hpp>

void Init()
{

}

void Render()
{

}

int main()
{
	GLFWwindow* wnd{};

	dB::glfw::Init();
	dB::glfw::CreateWindow(&wnd).Log().Exit_If_Error();

	glfwMakeContextCurrent(wnd);

	dB::glew::Init().Log().Exit_If_Error();

	Init();

	while (!glfwWindowShouldClose(wnd))
	{
		glfwPollEvents();

		Render();

		glfwSwapBuffers(wnd);
	}

	dB::glfw::Terminate();

	return 0;
}
*/