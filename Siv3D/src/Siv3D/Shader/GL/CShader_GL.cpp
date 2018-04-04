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

# include <Siv3D/Platform.hpp>
# if defined(SIV3D_TARGET_MACOS) || defined(SIV3D_TARGET_LINUX)

# include "CShader_GL.hpp"
# include <Siv3D/TextReader.hpp>
# include <Siv3D/Resource.hpp>
# include <map>
# include <tuple>

namespace s3d
{
	CShader_GL::CShader_GL()
	{

	}

	CShader_GL::~CShader_GL()
	{
		m_standardVSs.clear();
		
		m_standardPSs.clear();
		
		m_vertexShaders.destroy();
		
		m_pixelShaders.destroy();
        
        m_programs.clear();
	}

	bool CShader_GL::init()
	{		
		{
			const auto nullVertexShader = std::make_shared<VertexShader_GL>(VertexShader_GL::Null{});
			
			if (!nullVertexShader->isInitialized())
			{
				return false;
			}
			
			m_vertexShaders.setNullData(nullVertexShader);
		}
		
		{
			const auto nullPixelShader = std::make_shared<PixelShader_GL>(PixelShader_GL::Null{});
			
			if (!nullPixelShader->isInitialized())
			{
				return false;
			}
			
			m_pixelShaders.setNullData(nullPixelShader);
		}
		
		m_standardVSs.push_back(VertexShader(Resource(U"engine/shader/sprite.vert"), { { U"SpriteCB", 0 } }));
		m_standardPSs.push_back(PixelShader(Resource(U"engine/shader/shape.frag")));
		m_standardPSs.push_back(PixelShader(Resource(U"engine/shader/line_dot.frag")));
		m_standardPSs.push_back(PixelShader(Resource(U"engine/shader/line_round_dot.frag")));
		m_standardPSs.push_back(PixelShader(Resource(U"engine/shader/sprite.frag")));
		m_standardPSs.push_back(PixelShader(Resource(U"engine/shader/sprite_sdf.frag")));
		
		if (m_standardVSs.count_if(!Lambda::_) > 1)
		{
			LOG_FAIL(U"❌ CShader_GL: Failed to load standard vertex shaders");
			return false;
		}

		if (m_standardPSs.count_if(!Lambda::_) > 1)
		{
			LOG_FAIL(U"❌ CShader_GL: Failed to load standard pixel shaders");
			return false;
		}
        
        m_currentVS = VertexShaderID::NullAsset();
        m_currentPS = PixelShaderID::NullAsset();
		
		LOG_INFO(U"ℹ️ Shader initialized");
		
		return true;
	}
	
	VertexShaderID CShader_GL::createVSFromFile(const FilePath& path, const Array<BindingPoint>& bindingPoints)
	{
		TextReader reader(path);
		
		if (!reader)
		{
			return VertexShaderID::NullAsset();
		}
		
		return createVSFromSource(reader.readAll(), bindingPoints);
	}
	
	VertexShaderID CShader_GL::createVSFromSource(const String& source, const Array<BindingPoint>& bindingPoints)
	{
		const auto vertexShader = std::make_shared<VertexShader_GL>(source);
		
		if (!vertexShader->isInitialized())
		{
			return VertexShaderID::NullAsset();
		}
		
		for (const auto& bindingPoint : bindingPoints)
		{
			vertexShader->setUniformBlockBinding(bindingPoint.bufferName.narrow().c_str(), bindingPoint.index);
		}
		
		return m_vertexShaders.add(vertexShader);
	}
	
	PixelShaderID CShader_GL::createPSFromFile(const FilePath& path, const Array<BindingPoint>& bindingPoints)
	{
		TextReader reader(path);
		
		if (!reader)
		{
			return PixelShaderID::NullAsset();
		}
		
		return createPSFromSource(reader.readAll(), bindingPoints);
	}
	
	PixelShaderID CShader_GL::createPSFromSource(const String& source, const Array<BindingPoint>& bindingPoints)
	{
		const auto pixelShader = std::make_shared<PixelShader_GL>(source);
		
		if (!pixelShader->isInitialized())
		{
			return PixelShaderID::NullAsset();
		}
		
		for (const auto& bindingPoint : bindingPoints)
		{
			pixelShader->setUniformBlockBinding(bindingPoint.bufferName.narrow().c_str(), bindingPoint.index);
		}
		
		return m_pixelShaders.add(pixelShader);
	}
    
    void CShader_GL::setVS(const VertexShaderID handleID)
    {
        changeShaders(handleID, m_currentPS);
    }
    
    void CShader_GL::setPS(const PixelShaderID handleID)
    {
        changeShaders(m_currentVS, handleID);
    }
    
    void CShader_GL::changeShaders(VertexShaderID vsID, PixelShaderID psID)
    {
        m_currentVS = vsID;
        m_currentPS = psID;
        
        if (vsID.isNullAsset() || psID.isNullAsset())
        {
            return;
        }
        
        const GLuint vs = getVS(vsID);
        const GLuint ps = getPS(psID);
        const auto shaderTuple = std::make_tuple(vs, ps);
        
        if (m_programs.count(shaderTuple) == 0)
        {
            LOG_DEBUG(U"creating new program");
            // create new program and cache
            m_programs[shaderTuple] = std::make_shared<ShaderProgram>(vs, ps);
            
            GLuint program = m_programs[shaderTuple]->getProgram();
            
            // set uniform block binding
            for (auto binding : m_vertexShaders[vsID]->getBindings())
            {
                GLuint idx = ::glGetUniformBlockIndex(program, binding.first.c_str());
                ::glUniformBlockBinding(program, idx, binding.second);
            }
            
            for (auto binding : m_pixelShaders[psID]->getBindings())
            {
                GLuint idx = ::glGetUniformBlockIndex(program, binding.first.c_str());
                ::glUniformBlockBinding(program, idx, binding.second);
            }
            
            LOG_DEBUG(U"created shader program");
        }
        
        m_currentProgram = m_programs[shaderTuple];
        
        GLuint program = m_currentProgram->getProgram();
        ::glUseProgram(program);
    }
	
	void CShader_GL::releaseVS(const VertexShaderID handleID)
	{
		m_vertexShaders.erase(handleID);
	}
	
	void CShader_GL::releasePS(const PixelShaderID handleID)
	{
		m_pixelShaders.erase(handleID);
	}
	
	GLuint CShader_GL::getVS(const VertexShaderID handleID)
	{
		return m_vertexShaders[handleID]->getShader();
	}
	
	GLuint CShader_GL::getPS(const PixelShaderID handleID)
	{
		return m_pixelShaders[handleID]->getShader();
	}
	
    void CShader_GL::setPSSamplerUniform()
    {
        m_currentProgram->setPSSamplerUniform();
    }
}

# endif
