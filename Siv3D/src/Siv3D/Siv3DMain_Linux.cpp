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
# if defined(SIV3D_TARGET_LINUX)

# include <emscripten.h>

# include <iostream>
# include <unistd.h>
# include <Siv3D/String.hpp>
# include <Siv3D/FileSystem.hpp>
# include <Siv3D/Unicode.hpp>
# include <Siv3D/Logger.hpp>
# include "Siv3DEngine.hpp"
# include "System/ISystem.hpp"

// void Main();
void OnStart();
bool OnUpdate();
void OnExit();

namespace s3d
{
	namespace detail
	{
		namespace init
		{
			void SetModulePath(const FilePath& path);
		}
	}
}

s3d::Siv3DEngine* engine;

void mainLoop();

int main(int, char* argv[])
{
	using namespace s3d;

	std::cout << "Siv3D for Linux\n";

	const FilePath path = Unicode::Widen(argv[0]);
	FilePath modulePath = FileSystem::ParentPath(path, 0);

	if (modulePath.ends_with(U'/'))
	{
		modulePath.pop_back();
	}

	detail::init::SetModulePath(modulePath);

	chdir(FileSystem::ParentPath(path, 0).narrow().c_str());

	// Siv3DEngine engine;
    engine = new Siv3DEngine();

	if (!Siv3DEngine::GetSystem()->init())
	{
		return -1;
	}

	Logger.writeRawHTML_UTF8(u8"<hr width=\"99%\">");
// 	
// 	Main();
// 	
// 	Logger.writeRawHTML_UTF8(u8"<hr width=\"99%\">");
// }

    OnStart();
    
    emscripten_set_main_loop(mainLoop, 0, true);
}

void mainLoop()
{
    using namespace s3d;

    if (!OnUpdate())
    {
      // finished
      OnExit();

   	  Logger.writeRawHTML_UTF8(u8"<hr width=\"99%\">");

      delete engine;
      
      exit(0);
    }
}

# endif
