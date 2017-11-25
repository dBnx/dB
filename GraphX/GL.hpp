
#pragma once

#include <dB\GraphX\GLFW.hpp>

namespace dB::gl
{
	typedef enum {
		PIXEL = 0,
		FRAGMENT = PIXEL,
		VERTEX,
		GEOMETRY,
		COMPUTE

	} SHADERTYPE;

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

		Eret AddShaderFromDisk(SHADERTYPE type, std::string filename)
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

			std::ifstream	fstream(filename, std::ios::in | std::ios::ate);
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

		Eret FinishCreation()
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

	Eret GetError()
	{
		int32_t code = static_cast<int32_t>(glGetError());
		std::string msg = (const char*) glewGetErrorString(code);
		return Eret(-code, std::string("OpenGL : ") + msg);
	}
}
