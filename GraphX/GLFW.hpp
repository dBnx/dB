
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

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE


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

#ifdef OWN_USE_GLEW
namespace dB::glew
{
	Eret Init()
	{
		glewExperimental = GL_TRUE;
		return Eret("GLEW Initialisation", !glewInit() ? 0 : -1);
	}
}

namespace dB::gl
{
	namespace imm
	{
		void Color(glm::vec3 color = glm::vec3{ 0.0f, 0.4f, 1.0f })
		{
			glColor3f(color.x, color.y, color.z);
		}

		void Vertex(glm::vec2 pos)
		{
			glVertex2f(pos.x, pos.y);
		}
	}

	namespace buffer
	{
		enum class BufferType : int8_t
		{
			VERTEX = 0x01,
			COLOR = VERTEX,
			NORMAL = VERTEX,
			INDEX = 0x02,
		};

		inline bool operator & (BufferType lhs, BufferType rhs)
		{
			return std::underlying_type_t <BufferType>(std::underlying_type_t <BufferType>(lhs) & std::underlying_type_t <BufferType>(rhs));
		}

		inline bool operator | (BufferType lhs, BufferType rhs)
		{
			return std::underlying_type_t <BufferType>(std::underlying_type_t <BufferType>(lhs) | std::underlying_type_t <BufferType>(rhs));
		}

		constexpr GLenum EnumToGlEnum(BufferType type)
		{
			return	type & BufferType::INDEX ? GL_ELEMENT_ARRAY_BUFFER_ARB : GL_ARRAY_BUFFER_ARB;
		}

		enum class Usage : int16_t
		{
			STATIC_DRAW = 0x00'01,
			STATIC_READ = 0x00'02,
			STATIC_DRAW_READ = 0x00'04,
			STATIC_COPY = STATIC_DRAW_READ,
			DYNAMIC_DRAW = 0x00'10,
			DYNAMIC_READ = 0x00'20,
			DYNAMIC_DRAW_READ = 0x00'40,
			DYNAMIC_COPY = DYNAMIC_DRAW_READ,
			STREAM_DRAW = 0x01'00,
			STREAM_READ = 0x02'00,
			STREAM_DRAW_READ = 0x04'00,
			STREAM_COPY = STREAM_DRAW_READ,
		};

		inline bool operator & (Usage lhs, Usage rhs)
		{
			return std::underlying_type_t <Usage>(std::underlying_type_t <Usage>(lhs) & std::underlying_type_t <Usage>(rhs));
		}

		inline bool operator | (Usage lhs, Usage rhs)
		{
			return std::underlying_type_t <Usage>(std::underlying_type_t <Usage>(lhs) | std::underlying_type_t <Usage>(rhs));
		}

		constexpr GLenum EnumToGlEnum(Usage type)
		{
			return
				type & Usage::STATIC_DRAW ? GL_STATIC_DRAW_ARB
				: type & Usage::STATIC_READ ? GL_STATIC_READ_ARB
				: type & Usage::STATIC_COPY ? GL_STATIC_COPY_ARB
				: type & Usage::DYNAMIC_DRAW ? GL_DYNAMIC_DRAW_ARB
				: type & Usage::DYNAMIC_READ ? GL_DYNAMIC_READ_ARB
				: type & Usage::DYNAMIC_COPY ? GL_DYNAMIC_COPY_ARB
				: type & Usage::STREAM_DRAW ? GL_STREAM_DRAW_ARB
				: type & Usage::STREAM_READ ? GL_STREAM_READ_ARB
				: type & Usage::STREAM_COPY ? GL_STREAM_COPY_ARB
				: GL_DYNAMIC_DRAW_ARB;
		}

		template < BufferType type = BufferType::VERTEX, Usage usage = Usage::DYNAMIC_DRAW >
		class Buffer
		{
		private:

			GLuint bufferId{};
			const GLenum target{ EnumToGlEnum(type) };
			const GLenum usetype{ EnumToGlEnum(usage) };

		public:

			void Bind()
			{
				glBindBufferARB(target, bufferId);
			}

			void Create(bool initialise = true)
			{
				glGenBuffersARB(1, &bufferId);

				if (initialise)
					this->Bind();		// First binding initialises the buffer with zero
			}

			Buffer( /* BufferType type = BufferType::Vertex, */ bool create = true, bool initialise = true)
			{
				if (create)
					this->Create(initialise);
			}

			~Buffer()
			{
				glDeleteBuffersARB(1, &bufferId);
			}

			template < typename T, size_t SIZE >
			void Write(std::array<T, SIZE> data, bool bindBuffer = true)
			{
				if (bindBuffer)
					this->Bind();
				const auto dataSize = data.size() * (sizeof data[0]);
				glBufferDataARB(target, dataSize, &(data[0]), usetype);
			}
		};
	}
}
#endif // OWN_USE_GLEW


namespace dB::glfw
{
	Eret Init()
	{
		return Eret( "GLFW Initialisation", glfwInit() ? 0 : -1);
	}

	Eret CreateWindow(GLFWwindow** wnd, const std::string title = "dBnx - GLFW", std::function<void(void)> setWindowHints = [](void)->void {},
		const glm::ivec2 res = { 1024, 768 }, GLFWmonitor* mon = nullptr, GLFWwindow* share = nullptr)
	{
		if (*wnd)
			glfwDestroyWindow(*wnd);

		glfwDefaultWindowHints();
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		//glfwWindowHint(GLFW_SAMPLES, 4);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		if (setWindowHints)
			setWindowHints();

		*wnd = glfwCreateWindow(res.x, res.y, title.c_str(), mon, share);
		glfwMakeContextCurrent(*wnd);

		return Eret("GLFW Window creation", *wnd ? 0 : -1);
	}

	void Terminate()
	{
		glfwTerminate();
	}

	void CorrectAspectRatio(GLFWwindow* wnd) // Do not use
	{
		assert(0);
		int width, height;
		glfwGetWindowSize(wnd, &width, &height);

		const GLdouble aspectRatio = float(width) / float(height);
		GLdouble xSpan = 1.f;
		GLdouble ySpan = 1.f;

		xSpan *= aspectRatio;
		/*
		if (aspectRatio < 1.)
		ySpan = xSpan / aspectRatio;
		else
		xSpan *= aspectRatio;
		/**/
		glLoadIdentity(); // The OpenGL state machine is bullshit
		glOrtho(-xSpan, xSpan, -ySpan, ySpan, -1., 1.);
	}

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

		static void window_size(GLFWwindow* window, int width, int height)
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
			glfwSetWindowSizeCallback(wnd, window_size);
		}
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

