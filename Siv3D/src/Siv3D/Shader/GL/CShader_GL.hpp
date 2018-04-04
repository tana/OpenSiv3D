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

# include "../IShader.hpp"
# include <Siv3D/Array.hpp>
# include <Siv3D/ByteArrayView.hpp>
# include "VertexShader_GL.hpp"
# include "PixelShader_GL.hpp"
# include "../../AssetHandleManager/AssetHandleManager.hpp"
# include <map>
# include <tuple>
# include <memory>

namespace s3d
{
    class ShaderProgram
    {
    private:
        GLuint m_program;
        bool m_initialized;
        Optional<GLint> m_textureIndex;
        
    public:
        ShaderProgram(GLuint vs, GLuint ps)
        {
            m_program = ::glCreateProgram();
            
            ::glAttachShader(m_program, vs);
            ::glAttachShader(m_program, ps);
            
            ::glLinkProgram(m_program);
            
            GLint status = GL_FALSE;
            ::glGetProgramiv(m_program, GL_LINK_STATUS , &status);
            
            GLint logLen = 0;
            ::glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLen);
            
            if (logLen > 4)
            {
                std::string log(logLen + 1, '\0');
                ::glGetProgramInfoLog(m_program, logLen, &logLen, &log[0]);
                LOG_FAIL(U"❌ Shader link failed: {0}"_fmt(Unicode::Widen(log)));
            }
            
            if (status == GL_FALSE)
            {
                // link failed
                ::glDeleteProgram(m_program);
                m_program = 0;
            }
            
            if (m_program)
            {
                const int32 t = ::glGetUniformLocation(m_program, "Tex0");
                if (t != -1)
                {
                    m_textureIndex = t;
                }
            }
            
            m_initialized = m_program != 0;
        }
        
        void setPSSamplerUniform()
        {
            if (m_textureIndex)
            {
                ::glUniform1i(m_textureIndex.value(), 0);
            }
        }
        
        ~ShaderProgram()
        {
            if (m_initialized)
            {
                LOG_DEBUG(U"deleting shader program {0}"_fmt(m_program));
                ::glDeleteProgram(m_program);
            }
        }
        
        GLuint getProgram() const
        {
            return m_program;
        }
    };
    
	class CShader_GL : public ISiv3DShader
	{
	private:

		Array<VertexShader> m_standardVSs;

		Array<PixelShader> m_standardPSs;
		
		AssetHandleManager<VertexShaderID, VertexShader_GL> m_vertexShaders{ U"VertexShader" };
		
		AssetHandleManager<PixelShaderID, PixelShader_GL> m_pixelShaders{ U"PixelShader" };
        
        // cache for linked programs
        std::map<std::tuple<GLuint, GLuint>, std::shared_ptr<ShaderProgram>> m_programs; // TODO unordered_map
        
        VertexShaderID m_currentVS;
        PixelShaderID m_currentPS;
        
        std::shared_ptr<ShaderProgram> m_currentProgram;

	public:

		CShader_GL();

		~CShader_GL() override;

		bool init();

		VertexShaderID createVS(ByteArray&&) override { return VertexShaderID::NullAsset(); }

		VertexShaderID createVSFromFile(const FilePath& path, const Array<BindingPoint>&) override;
		
		VertexShaderID createVSFromSource(const String& source, const Array<BindingPoint>& bindingPoints) override;

		PixelShaderID createPS(ByteArray&&) override { return PixelShaderID::NullAsset(); }

		PixelShaderID createPSFromFile(const FilePath& path, const Array<BindingPoint>&) override;
		
		PixelShaderID createPSFromSource(const String& source, const Array<BindingPoint>& bindingPoints) override;

		void releaseVS(VertexShaderID handleID) override;

		void releasePS(PixelShaderID handleID) override;
		
		ByteArrayView getBinaryViewVS(VertexShaderID) override
		{
			return ByteArrayView();
		}

		ByteArrayView getBinaryViewPS(PixelShaderID) override
		{
			return ByteArrayView();
		}

		const VertexShader& getStandardVS(size_t index) const override
		{
			return m_standardVSs[index];
		}

		const PixelShader& getStandardPS(size_t index) const override
		{
			return m_standardPSs[index];
		}
		
        void setVS(VertexShaderID handleID) override;

        void setPS(PixelShaderID handleID) override;
        
        void changeShaders(VertexShaderID vsID, PixelShaderID psID);
		
		GLuint getVS(VertexShaderID handleID);
		
		GLuint getPS(PixelShaderID handleID);
		
        void setPSSamplerUniform();
	};
}

# endif
