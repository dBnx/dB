
#pragma once

#include <filesystem>

#ifndef __DB_GL_HPP
#define __DB_GL_HPP

//#include <dB\GraphX\GLFW.hpp>

namespace dB::gl
{
	typedef enum {
		PIXEL = 0,
		FRAGMENT = PIXEL,
		VERTEX,
		GEOMETRY,
		COMPUTE

	} SHADERTYPE;

	// Todo:
	// - Add shader from ram
	// - Load spir-v shader program
	class SHADER
	{
	private:
		int	program{ -1 };
		int	id_vert{ -1 }, id_frag{ -1 }, id_geom{ -1 };
	public:
		SHADER() {};
		~SHADER() {};

		void Use()
		{
			glUseProgram(this->program);
		}

		GLuint ID() const
		{
			return static_cast<GLuint>(this->program);
		}

		Eret AddShaderFromDisk(SHADERTYPE type, const std::experimental::filesystem::path filepath)//std::string filename)
		{
			int *id{ nullptr };
			Eret eret("Shader creation");

			switch (type)
			{
			case SHADERTYPE::VERTEX:
				id = &(this->id_vert);
				*id = glCreateShader(GL_VERTEX_SHADER);
				break;
			case SHADERTYPE::GEOMETRY:
				id = &(this->id_geom);
				*id = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			case SHADERTYPE::FRAGMENT:
				id = &(this->id_frag);
				*id = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			default:
				return Eret(eret, -1);
				break;
			}

			std::ifstream	fstream(filepath, std::ios::in | std::ios::ate);
			std::string		code{ "" };

			if (!fstream.is_open())
				return Eret(eret, -2);

			size_t filesize = fstream.tellg();
			fstream.seekg(0);
			code.resize(filesize);
			fstream.read(&code[0], filesize);
			fstream.close();

			char const * code_p = code.c_str();
			glShaderSource(*id, 1, &code_p, NULL);
			glCompileShader(*id);

			// Check
			GLint Result{ GL_FALSE };
			int InfoLogLength{ 0 };
			glGetShaderiv(*id, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(*id, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0)
			{
				std::string VertexShaderErrorMessage;
				VertexShaderErrorMessage.resize(InfoLogLength + 1);
				glGetShaderInfoLog(*id, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				std::cerr << "\n### ERROR ###\n" << VertexShaderErrorMessage << "\n\n" << std::flush;
				if (VertexShaderErrorMessage[0])
				{	// 187 Bytes of Zeros as Error ? Quickfix
					return Eret(eret, -3);
				}
			}

			return eret;
		}

		Eret FinishCreation(std::function<void(int)> setAttribLocations = nullptr)
		{
			Eret eret("Shader finalisation");
			if (this->id_frag != -1)
				this->program = glCreateProgram();

			if (this->id_frag != -1)
				glAttachShader(this->program, id_frag);
			if (this->id_geom != -1)
				glAttachShader(this->program, id_geom);
			if (this->id_vert != -1)
				glAttachShader(this->program, id_vert);

			if (setAttribLocations)
				setAttribLocations(ID());
			else
			{
				glBindAttribLocation(ID(), 0, "uResolution");
				glBindAttribLocation(ID(), 1, "uCenter");
				glBindAttribLocation(ID(), 2, "uScale");
				glBindAttribLocation(ID(), 3, "uTime");
				glBindAttribLocation(ID(), 4, "uIterations");
			}

			glLinkProgram(this->program);

			// Check
			GLint Result = GL_FALSE;
			int InfoLogLength = 0;
			glGetProgramiv(this->program, GL_LINK_STATUS, &Result);
			glGetProgramiv(this->program, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if (InfoLogLength > 0 && Result == GL_FALSE)
			{
				std::string VertexShaderErrorMessage;
				VertexShaderErrorMessage.resize(InfoLogLength + 1);
				char * msg_ptr = &VertexShaderErrorMessage[0];
				glGetProgramInfoLog(this->program, InfoLogLength, NULL, msg_ptr);
				std::cerr << "\n### ERROR ###\n" << VertexShaderErrorMessage << "\n\n" << std::flush;

				return Eret(eret, -3);
			}

			auto Destroy = [](int program, int& id)
			{
				if (id != -1)
				{
					glDetachShader(program, id);
					glDeleteShader(id);
				}
				id = -1;
			};

			Destroy(this->program, this->id_frag);
			Destroy(this->program, this->id_geom);
			Destroy(this->program, this->id_vert);
			
			return eret;
		}
	};

	std::string FormatedInformations()
	{
		std::stringstream ss;
		ss << "GL_RENDERER = " << glGetString(GL_RENDERER) << "\n"
			<< "GL_VERSION  = " << glGetString(GL_VERSION) << "\n";
		return ss.str();
	}

	void CreateDummyVAO()
	{
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);
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

			GLuint ID() const
			{
				return bufferId;
			}

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
			/*
			template < typename T, size_t SIZE >
			void Write(std::array<T, SIZE> &data, bool bindBuffer = true)
			{
				if (bindBuffer)
					this->Bind();
				const auto dataSize = data.size() * (sizeof data[0]);
				glBufferDataARB(target, dataSize, &(data[0]), usetype);
			}*/

			template < typename DATA_TYPE, size_t SIZE >
			void Write(const std::array<DATA_TYPE, SIZE> &data, const bool bindBuffer = true)
			{
				if (bindBuffer)
					this->Bind();
				const auto dataSize = data.size() * (sizeof data[0]);
				glBufferDataARB(target, dataSize, &data[0], usetype);
			}
		};
	}
}
#endif // !__DB_GL_HPP
