//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2018 Ryo Suzuki
//	Copyright (c) 2016-2018 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include <Siv3D/Platform.hpp>
# if defined(SIV3D_TARGET_MACOS) || defined(SIV3D_TARGET_LINUX)

# include <GL/glew.h>
# include "../../../ThirdParty/GLFW/include/GLFW/glfw3.h"
# include <Siv3D/Fwd.hpp>
# include <Siv3D/Optional.hpp>
# include <Siv3D/Logger.hpp>
# include <unordered_map>

namespace s3d
{
	class PixelShader_GL
	{
	private:
		
		GLuint m_shader = 0;
		
		Optional<GLint> m_textureIndex;
		
		bool m_initialized = false;
        
        std::unordered_map<std::string, GLuint> m_bindings;
		
	public:
		
		struct Null {};
		
		PixelShader_GL() = default;
		
		PixelShader_GL(Null)
		{
			m_initialized = true;
		}
		
		~PixelShader_GL()
		{
            if (m_shader)
            {
                ::glDeleteShader(m_shader);
            }
		}
		
		PixelShader_GL(const String& source)
		{
			const std::string sourceUTF8 = source.toUTF8();
			
			const char* pSource = sourceUTF8.c_str();
			
            m_shader = ::glCreateShader(GL_FRAGMENT_SHADER);
            ::glShaderSource(m_shader, 1, &pSource, NULL);
            ::glCompileShader(m_shader);
			
			GLint status = GL_FALSE;
			
            ::glGetShaderiv(m_shader, GL_COMPILE_STATUS, &status);
            
            GLint logLen = 0;
            
            ::glGetShaderiv(m_shader, GL_INFO_LOG_LENGTH, &logLen);
			
			if (logLen > 4)
			{
				std::string log(logLen + 1, '\0');
				
				::glGetShaderInfoLog(m_shader, logLen, &logLen, &log[0]);
				
				LOG_FAIL(U"❌ Pixel shader compilation failed: {0}"_fmt(Unicode::Widen(log)));
			}
			
			if (status == GL_FALSE)
			{
				::glDeleteShader(m_shader);
				
				m_shader = 0;
			}
			
			m_initialized = m_shader != 0;
		}
		
		bool isInitialized() const noexcept
		{
			return m_initialized;
		}
		
		GLint getShader() const
		{
			return m_shader;
		}
        
		void setUniformBlockBinding(const char* const name, GLuint index)
		{
            std::string nameStr(name);
            m_bindings[name] = index;
		}
        
        std::unordered_map<std::string, GLuint>& getBindings()
        {
            return m_bindings;
        }
		
		ByteArrayView getView() const;
	};
}

# endif
