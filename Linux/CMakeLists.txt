cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

project(OpenSiv3D_Linux CXX)
enable_language(C)

set(CMAKE_CXX_COMPILER "clang++")
#set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -Wno-unknown-pragmas -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

set(CMAKE_C_COMPILER "clang")
#set(CMAKE_C_COMPILER "gcc")
set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unknown-pragmas -fPIC")
set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG -march=x86-64")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-g3 -Og -pg")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -march=x86-64")

#set(CMAKE_BUILD_TYPE Release)

pkg_check_modules(LIBSIV3D REQUIRED glib-2.0 gobject-2.0 gio-2.0 gl libpng libturbojpeg x11 xi xinerama xcursor xrandr gl glu freetype2 openal)
include_directories(
	"/usr/include"

	"."
	"../Siv3D/include"
	"../Siv3D/include/ThirdParty"
	"../Siv3D/src/ThirdParty"

	"../Siv3D/src/ThirdParty/zstd"
	"../Siv3D/src/ThirdParty/zstd/common"

	${LIBSIV3D_INCLUDE_DIRS}
)

set(SOURCE_FILES
	"../Siv3D/src/Siv3D/Asset/AssetFactory.cpp"
	"../Siv3D/src/Siv3D/Asset/CAsset.cpp"
	"../Siv3D/src/Siv3D/Asset/SivAsset.cpp"
	"../Siv3D/src/Siv3D/Audio/AL/CAudio_AL.cpp"
	"../Siv3D/src/Siv3D/Audio/AudioControlManager.hpp"
	"../Siv3D/src/Siv3D/Audio/AudioFactory.cpp"
	"../Siv3D/src/Siv3D/Audio/SivAudio.cpp"
	"../Siv3D/src/Siv3D/Audio/Null/CAudio_Null.cpp"
	"../Siv3D/src/Siv3D/AudioAsset/SivAudioAsset.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/AudioFormatFactory.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/CAudioFormat.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/SivAudioFormat.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/AAC/AudioFormat_AAC_Linux.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/MP3/AudioFormat_MP3_mpg123.cpp"
	"../Siv3D/src/Siv3D/AudioFormat/WAVE/AudioFormat_WAVE.cpp"
	"../Siv3D/src/Siv3D/Base64/SivBase64.cpp"
	"../Siv3D/src/Siv3D/Bezier2/SivBezier2.cpp"
	"../Siv3D/src/Siv3D/Bezier3/SivBezier3.cpp"
	"../Siv3D/src/Siv3D/BigFloat/SivBigFloat.cpp"
	"../Siv3D/src/Siv3D/BigInt/CBigInt.cpp"
	"../Siv3D/src/Siv3D/BigInt/SivBigInt.cpp"
	"../Siv3D/src/Siv3D/BinaryReader/CBinaryReader_Linux.cpp"
	"../Siv3D/src/Siv3D/BinaryReader/SivBinaryReader.cpp"
	"../Siv3D/src/Siv3D/BinaryWriter/CBinaryWriter_Linux.cpp"
	"../Siv3D/src/Siv3D/BinaryWriter/SivBinaryWriter.cpp"
	"../Siv3D/src/Siv3D/BlendState/SivBlendState.cpp"
	"../Siv3D/src/Siv3D/BoolFormat/SivBoolFormat.cpp"
	"../Siv3D/src/Siv3D/Byte/SivByte.cpp"
	"../Siv3D/src/Siv3D/ByteArray/CByteArray.cpp"
	"../Siv3D/src/Siv3D/ByteArray/SivByteArray.cpp"
	"../Siv3D/src/Siv3D/ByteArrayView/SivByteArrayView.cpp"
	"../Siv3D/src/Siv3D/CPU/CCPU.cpp"
	"../Siv3D/src/Siv3D/CPU/CPUFactory.cpp"
	"../Siv3D/src/Siv3D/CPU/SivCPU.cpp"
	"../Siv3D/src/Siv3D/CSVData/SivCSVData.cpp"
	"../Siv3D/src/Siv3D/Circle/SivCircle.cpp"
	"../Siv3D/src/Siv3D/Clipboard/CClipboard_Linux.cpp"
	"../Siv3D/src/Siv3D/Clipboard/ClipboardFactory.cpp"
	"../Siv3D/src/Siv3D/Clipboard/SivClipboard.cpp"
	"../Siv3D/src/Siv3D/Codec/CodecFactory.cpp"
	"../Siv3D/src/Siv3D/Codec/MF/CCodec_MF.cpp"
	"../Siv3D/src/Siv3D/Codec/Null/CCodec_Null.cpp"
	"../Siv3D/src/Siv3D/Color/SivColor.cpp"
	"../Siv3D/src/Siv3D/Compression/SivCompression.cpp"
	"../Siv3D/src/Siv3D/Console/CConsole.cpp"
	"../Siv3D/src/Siv3D/Console/ConsoleFactory.cpp"
	"../Siv3D/src/Siv3D/Console/SivConsole.cpp"
	"../Siv3D/src/Siv3D/ConstantBuffer/GL/GLConstantBuffer.cpp"
	"../Siv3D/src/Siv3D/ConstantBuffer/SivConstantBuffer.cpp"
	"../Siv3D/src/Siv3D/Cursor/CCursor_Linux.cpp"
	"../Siv3D/src/Siv3D/Cursor/CursorFactory.cpp"
	"../Siv3D/src/Siv3D/Cursor/SivCursor.cpp"
	"../Siv3D/src/Siv3D/Date/SivDate.cpp"
	"../Siv3D/src/Siv3D/DateTime/SivDateTime.cpp"
	"../Siv3D/src/Siv3D/DragDrop/CDragDrop_Linux.cpp"
	"../Siv3D/src/Siv3D/DragDrop/DragDropFactory.cpp"
	"../Siv3D/src/Siv3D/DragDrop/SivDragDrop.cpp"
	"../Siv3D/src/Siv3D/DynamicTexture/SivDynamicTexture.cpp"
	"../Siv3D/src/Siv3D/Effect/CEffect.cpp"
	"../Siv3D/src/Siv3D/Effect/EffectData.cpp"
	"../Siv3D/src/Siv3D/Effect/EffectFactory.cpp"
	"../Siv3D/src/Siv3D/Effect/SivEffect.cpp"
	"../Siv3D/src/Siv3D/Ellipse/SivEllipse.cpp"
	"../Siv3D/src/Siv3D/Emoji/SivEmoji.cpp"
	"../Siv3D/src/Siv3D/EmojiDictionary/CEmojiDictionary.cpp"
	"../Siv3D/src/Siv3D/EmojiDictionary/SivEmojiDictionary.cpp"
	"../Siv3D/src/Siv3D/EngineDirectory/EngineDirectory.cpp"
	"../Siv3D/src/Siv3D/Exif/SivExif.cpp"
	"../Siv3D/src/Siv3D/FFT/CFFT.cpp"
	"../Siv3D/src/Siv3D/FFT/FFTFactory.cpp"
	"../Siv3D/src/Siv3D/FFT/SivFFT.cpp"
	"../Siv3D/src/Siv3D/FileSystem/SivFileSystem.cpp"
	"../Siv3D/src/Siv3D/FileSystem/SivFileSystem_Linux.cpp"
	"../Siv3D/src/Siv3D/FloatFormat/SivFloatFormat.cpp"
	"../Siv3D/src/Siv3D/Font/CFont.cpp"
	"../Siv3D/src/Siv3D/Font/FontData.cpp"
	"../Siv3D/src/Siv3D/Font/FontFactory.cpp"
	"../Siv3D/src/Siv3D/Font/SivFont.cpp"
	"../Siv3D/src/Siv3D/FontAsset/SivFontAsset.cpp"
	"../Siv3D/src/Siv3D/Format/SivFormat.cpp"
	"../Siv3D/src/Siv3D/FormatLiteral/SivFormatLiteral.cpp"
	"../Siv3D/src/Siv3D/FormatUtility/SivFormatUtility.cpp"
	"../Siv3D/src/Siv3D/Gamepad/CGamepad_Linux.cpp"
	"../Siv3D/src/Siv3D/Gamepad/GamepadFactory.cpp"
	"../Siv3D/src/Siv3D/Gamepad/SivGamepad.cpp"
	"../Siv3D/src/Siv3D/Geometry2D/SivGeometry2D.cpp"
	"../Siv3D/src/Siv3D/Graphics/GL/BlendState/GLBlendState.cpp"
	"../Siv3D/src/Siv3D/Graphics/GL/CGraphics_GL.cpp"
	"../Siv3D/src/Siv3D/Graphics/GL/RasterizerState/GLRasterizerState.cpp"
	"../Siv3D/src/Siv3D/Graphics/GL/SamplerState/GLSamplerState.cpp"
	"../Siv3D/src/Siv3D/Graphics/GraphicsFactory.cpp"
	"../Siv3D/src/Siv3D/Graphics/SivGraphics.cpp"
	"../Siv3D/src/Siv3D/Graphics2D/SivGraphics2D.cpp"
	"../Siv3D/src/Siv3D/HSV/SivHSV.cpp"
	"../Siv3D/src/Siv3D/HTMLWriter/CHTMLWriter.cpp"
	"../Siv3D/src/Siv3D/HTMLWriter/SivHTMLWriter.cpp"
	"../Siv3D/src/Siv3D/INIData/SivINIData.cpp"
	"../Siv3D/src/Siv3D/Icon/SivIcon.cpp"
	"../Siv3D/src/Siv3D/Image/SivImage.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/CImageFormat.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/ImageFormatFactory.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/SivImageFormat.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/BMP/ImageFormat_BMP.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/GIF/ImageFormat_GIF.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/JPEG/ImageFormat_JPEG.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/PNG/ImageFormat_PNG.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/PPM/ImageFormat_PPM.cpp"
	"../Siv3D/src/Siv3D/ImageFormat/TGA/ImageFormat_TGA.cpp"
	"../Siv3D/src/Siv3D/ImageProcessing/SivImageProcessing.cpp"
	"../Siv3D/src/Siv3D/IntFormat/SivIntFormat.cpp"
	"../Siv3D/src/Siv3D/JSONReader/SivJSONReader.cpp"
	"../Siv3D/src/Siv3D/Key/SivKey.cpp"
	"../Siv3D/src/Siv3D/Keyboard/CKeyboard_Linux.cpp"
	"../Siv3D/src/Siv3D/Keyboard/KeyboardFactory.cpp"
	"../Siv3D/src/Siv3D/LicenseManager/CLicenseManager.cpp"
	"../Siv3D/src/Siv3D/LicenseManager/LicenseManagerFactory.cpp"
	"../Siv3D/src/Siv3D/LicenseManager/SivLicenseManager.cpp"
	"../Siv3D/src/Siv3D/Line/SivLine.cpp"
	"../Siv3D/src/Siv3D/LineString/SivLineString.cpp"
	"../Siv3D/src/Siv3D/Logger/CLogger.cpp"
	"../Siv3D/src/Siv3D/Logger/LoggerFactory.cpp"
	"../Siv3D/src/Siv3D/Logger/SivLogger.cpp"
	"../Siv3D/src/Siv3D/MD5/SivMD5.cpp"
	"../Siv3D/src/Siv3D/Mat3x2/SivMat3x2.cpp"
	"../Siv3D/src/Siv3D/MathParser/CMathParser.cpp"
	"../Siv3D/src/Siv3D/MathParser/SivMathParser.cpp"
	"../Siv3D/src/Siv3D/MemoryMapping/CMemoryMapping.cpp"
	"../Siv3D/src/Siv3D/MemoryMapping/SivMemoryMapping.cpp"
	"../Siv3D/src/Siv3D/MemoryWriter/CMemoryWriter.cpp"
	"../Siv3D/src/Siv3D/MemoryWriter/SivMemoryWriter.cpp"
	"../Siv3D/src/Siv3D/MessageBox/SivMessageBox_Linux.cpp"
	"../Siv3D/src/Siv3D/Monitor/SivMonitor.cpp"
	"../Siv3D/src/Siv3D/Mouse/CMouse_Linux.cpp"
	"../Siv3D/src/Siv3D/Mouse/MouseFactory.cpp"
	"../Siv3D/src/Siv3D/Mouse/SivMouse.cpp"
	"../Siv3D/src/Siv3D/Painting/PaintShape.cpp"
	"../Siv3D/src/Siv3D/Painting/SivPainting.cpp"
	"../Siv3D/src/Siv3D/ParseBool/SivParseBool.cpp"
	"../Siv3D/src/Siv3D/ParseFloat/SivParseFloat.cpp"
	"../Siv3D/src/Siv3D/ParseInt/SivParseInt.cpp"
	"../Siv3D/src/Siv3D/PerlinNoise/SivPerlinNoise.cpp"
	"../Siv3D/src/Siv3D/Physics2D/SivPhysics2D.cpp"
	"../Siv3D/src/Siv3D/PixelShader/SivPixelShader.cpp"
	"../Siv3D/src/Siv3D/Point/SivPoint.cpp"
	"../Siv3D/src/Siv3D/Polygon/CPolygon.cpp"
	"../Siv3D/src/Siv3D/Polygon/SivPolygon.cpp"
	"../Siv3D/src/Siv3D/PowerStatus/SivPowerStatus.cpp"
	"../Siv3D/src/Siv3D/PowerStatus/SivPowerStatus_Linux.cpp"
	"../Siv3D/src/Siv3D/Print/CPrint.cpp"
	"../Siv3D/src/Siv3D/Print/PrintFactory.cpp"
	"../Siv3D/src/Siv3D/Print/SivPrint.cpp"
	"../Siv3D/src/Siv3D/Profiler/CProfiler.cpp"
	"../Siv3D/src/Siv3D/Profiler/ProfilerFactory.cpp"
	"../Siv3D/src/Siv3D/Profiler/SivProfiler.cpp"
	"../Siv3D/src/Siv3D/Quad/SivQuad.cpp"
	"../Siv3D/src/Siv3D/RasterizerState/SivRasterizerState.cpp"
	"../Siv3D/src/Siv3D/Rectangle/SivRectangle.cpp"
	"../Siv3D/src/Siv3D/Resource/SivResource.cpp"
	"../Siv3D/src/Siv3D/RenderTexture/SivRenderTexture.cpp"
	"../Siv3D/src/Siv3D/Renderer2D/GL/CRenderer2D_GL.cpp"
	"../Siv3D/src/Siv3D/Renderer2D/Renderer2DFactory.cpp"
	"../Siv3D/src/Siv3D/RoundRect/SivRoundRect.cpp"
	"../Siv3D/src/Siv3D/SVM/CSVM.cpp"
	"../Siv3D/src/Siv3D/SVM/SivSVM.cpp"
	"../Siv3D/src/Siv3D/SamplerState/SivSamplerState.cpp"
	"../Siv3D/src/Siv3D/SamplerState/SivSamplerState.cpp"
	"../Siv3D/src/Siv3D/SVM/CSVM.cpp"
	"../Siv3D/src/Siv3D/SVM/SivSVM.cpp"
	"../Siv3D/src/Siv3D/ScreenCapture/CScreenCapture.cpp"
	"../Siv3D/src/Siv3D/ScreenCapture/ScreenCaptureFactory.cpp"
	"../Siv3D/src/Siv3D/ScreenCapture/SivScreenCapture.cpp"
	"../Siv3D/src/Siv3D/Script/AngelScript/scriptarray.cpp"
	"../Siv3D/src/Siv3D/Script/AngelScript/scriptbuilder.cpp"
	"../Siv3D/src/Siv3D/Script/AngelScript/scriptstdstring.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/ScriptBind.hpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Circle.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Color.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_ColorF.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Cursor.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Format.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Graphics.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_HSV.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Line.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Point.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Quad.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Random.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Rect.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_RectF.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_RoundRect.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Triangle.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Types.cpp"
	"../Siv3D/src/Siv3D/Script/Bind/Script_Vec2.cpp"
	"../Siv3D/src/Siv3D/Script/CScript.cpp"
	"../Siv3D/src/Siv3D/Script/ScriptData.cpp"
	"../Siv3D/src/Siv3D/Script/ScriptFactory.cpp"
	"../Siv3D/src/Siv3D/Script/SivScript.cpp"
	"../Siv3D/src/Siv3D/Shader/GL/CShader_GL.cpp"
	"../Siv3D/src/Siv3D/Shader/ShaderFactory.cpp"
	"../Siv3D/src/Siv3D/Shader/SivShader.cpp"
	"../Siv3D/src/Siv3D/Shape2D/SivShape2D.cpp"
	"../Siv3D/src/Siv3D/Sprite/SivSprite.cpp"
	"../Siv3D/src/Siv3D/Stopwatch/SivStopwatch.cpp"
	"../Siv3D/src/Siv3D/String/SivString.cpp"
	"../Siv3D/src/Siv3D/StringView/SivStringView.cpp"
	"../Siv3D/src/Siv3D/System/CSystem_Linux.cpp"
	"../Siv3D/src/Siv3D/System/SivSystem.cpp"
	"../Siv3D/src/Siv3D/System/SystemFactory.cpp"
	"../Siv3D/src/Siv3D/TOMLReader/SivTOMLReader.cpp"
	"../Siv3D/src/Siv3D/TextBox/CTextBox.cpp"
	"../Siv3D/src/Siv3D/TextBox/SivTextBox.cpp"
	"../Siv3D/src/Siv3D/TextEditor/SivTextEditor.cpp"
	"../Siv3D/src/Siv3D/TextEncoding/SivTextEncoding.cpp"
	"../Siv3D/src/Siv3D/TextInput/CTextInput_macOS.cpp"
	"../Siv3D/src/Siv3D/TextInput/SivTextInput.cpp"
	"../Siv3D/src/Siv3D/TextInput/TextInputFactory.cpp"
	"../Siv3D/src/Siv3D/TextReader/CTextReader.cpp"
	"../Siv3D/src/Siv3D/TextReader/CTextReader_Linux.cpp"
	"../Siv3D/src/Siv3D/TextReader/SivTextReader.cpp"
	"../Siv3D/src/Siv3D/TextWriter/CTextWriter.cpp"
	"../Siv3D/src/Siv3D/TextWriter/SivTextWriter.cpp"
	"../Siv3D/src/Siv3D/Texture/GL/CTexture_GL.cpp"
	"../Siv3D/src/Siv3D/Texture/GL/Texture_GL.cpp"
	"../Siv3D/src/Siv3D/Texture/SivTexture.cpp"
	"../Siv3D/src/Siv3D/Texture/TextureFactory.cpp"
	"../Siv3D/src/Siv3D/TextureAsset/SivTextureAsset.cpp"
	"../Siv3D/src/Siv3D/TextureRegion/SivTextureRegion.cpp"
	"../Siv3D/src/Siv3D/TexturedCircle/SivTexturedCircle.cpp"
	"../Siv3D/src/Siv3D/TexturedQuad/SivTexturedQuad.cpp"
	"../Siv3D/src/Siv3D/TexturedRoundRect/SivTexturedRoundRect.cpp"
	"../Siv3D/src/Siv3D/Threading/SivThreading.cpp"
	"../Siv3D/src/Siv3D/Triangle/SivTriangle.cpp"
	"../Siv3D/src/Siv3D/Time/SivTime.cpp"
	"../Siv3D/src/Siv3D/TimeProfiler/SivTimeProfiler.cpp"
	"../Siv3D/src/Siv3D/Twitter/SivTwitter.cpp"
	"../Siv3D/src/Siv3D/Unicode/SivUnicode.cpp"
	"../Siv3D/src/Siv3D/Vector2D/SivVector2D.cpp"
	"../Siv3D/src/Siv3D/Vector3D/SivVector3D.cpp"
	"../Siv3D/src/Siv3D/Vector4D/SivVector4D.cpp"
	"../Siv3D/src/Siv3D/VertexShader/SivVertexShader.cpp"
	"../Siv3D/src/Siv3D/Wave/SivWave.cpp"
	"../Siv3D/src/Siv3D/Window/CWindow_Linux.cpp"
	"../Siv3D/src/Siv3D/Window/SivWindow.cpp"
	"../Siv3D/src/Siv3D/Window/WindowFactory.cpp"
	"../Siv3D/src/Siv3D/WritableMemoryMapping/CWritableMemoryMapping.cpp"
	"../Siv3D/src/Siv3D/WritableMemoryMapping/SivWritableMemoryMapping.cpp"
	"../Siv3D/src/Siv3D/XInput/CXInput_NULL.cpp"
	"../Siv3D/src/Siv3D/XInput/SivXInput.cpp"
	"../Siv3D/src/Siv3D/XInput/XInputFactory.cpp"
	"../Siv3D/src/Siv3D/XXHash/SivXXHash.cpp"

	"../Siv3D/src/Siv3D/Siv3DEngine.cpp"
	"../Siv3D/src/Siv3D/Siv3DMain_Linux.cpp"

	"../Siv3D/src/ThirdParty/FID/FID.cpp"
	"../Siv3D/src/ThirdParty/FeatureDetector/cpu_x86.cpp"
	"../Siv3D/src/ThirdParty/FeatureDetector/cpu_x86_Linux.ipp"
	"../Siv3D/src/ThirdParty/clip2tri/clip2tri.cpp"
	"../Siv3D/src/ThirdParty/clipper/clipper.cpp"
	"../Siv3D/src/ThirdParty/double-conversion/bignum-dtoa.cc"
	"../Siv3D/src/ThirdParty/double-conversion/bignum.cc"
	"../Siv3D/src/ThirdParty/double-conversion/cached-powers.cc"
	"../Siv3D/src/ThirdParty/double-conversion/diy-fp.cc"
	"../Siv3D/src/ThirdParty/double-conversion/double-conversion.cc"
	"../Siv3D/src/ThirdParty/double-conversion/fast-dtoa.cc"
	"../Siv3D/src/ThirdParty/double-conversion/fixed-dtoa.cc"
	"../Siv3D/src/ThirdParty/double-conversion/strtod.cc"
	"../Siv3D/src/ThirdParty/easyexif/exif.cpp"
	"../Siv3D/src/ThirdParty/fmt/format.cc"
	"../Siv3D/src/ThirdParty/libsvm/svm.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParser.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserBase.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserBytecode.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserCallback.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserDLL.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserError.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserInt.cpp"
	"../Siv3D/src/ThirdParty/muparser/muParserTokenReader.cpp"
	"../Siv3D/src/ThirdParty/pffft/pffft.c"
	"../Siv3D/src/ThirdParty/poly2tri/common/shapes.cc"
	"../Siv3D/src/ThirdParty/poly2tri/sweep/advancing_front.cc"
	"../Siv3D/src/ThirdParty/poly2tri/sweep/cdt.cc"
	"../Siv3D/src/ThirdParty/poly2tri/sweep/sweep.cc"
	"../Siv3D/src/ThirdParty/poly2tri/sweep/sweep_context.cc"
	"../Siv3D/src/ThirdParty/zstd/common/entropy_common.c"
	"../Siv3D/src/ThirdParty/zstd/common/error_private.c"
	"../Siv3D/src/ThirdParty/zstd/common/fse_decompress.c"
	"../Siv3D/src/ThirdParty/zstd/common/pool.c"
	"../Siv3D/src/ThirdParty/zstd/common/threading.c"
	"../Siv3D/src/ThirdParty/zstd/common/xxhash.c"
	"../Siv3D/src/ThirdParty/zstd/common/zstd_common.c"
	"../Siv3D/src/ThirdParty/zstd/compress/fse_compress.c"
	"../Siv3D/src/ThirdParty/zstd/compress/huf_compress.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_compress.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_double_fast.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_fast.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_lazy.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_ldm.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstd_opt.c"
	"../Siv3D/src/ThirdParty/zstd/compress/zstdmt_compress.c"
	"../Siv3D/src/ThirdParty/zstd/decompress/huf_decompress.c"
	"../Siv3D/src/ThirdParty/zstd/decompress/zstd_decompress.c"
	"../Siv3D/src/ThirdParty/zstd/dictBuilder/cover.c"
	"../Siv3D/src/ThirdParty/zstd/dictBuilder/divsufsort.c"
	"../Siv3D/src/ThirdParty/zstd/dictBuilder/zdict.c"

	#"../Siv3D/src/ThirdParty/Box2D/Collision/Shapes/b2ChainShape.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/Shapes/b2CircleShape.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/Shapes/b2EdgeShape.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/Shapes/b2PolygonShape.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2BroadPhase.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2CollideCircle.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2CollideEdge.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2CollidePolygon.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2Collision.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2Distance.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2DynamicTree.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Collision/b2TimeOfImpact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2BlockAllocator.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2Draw.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2Math.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2Settings.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2StackAllocator.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Common/b2Timer.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2CircleContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2Contact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2ContactSolver.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Contacts/b2PolygonContact.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2DistanceJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2FrictionJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2GearJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2Joint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2MotorJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2MouseJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2PulleyJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2RopeJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2WeldJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/Joints/b2WheelJoint.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2Body.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2ContactManager.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2Fixture.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2Island.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2World.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Dynamics/b2WorldCallbacks.cpp"
	#"../Siv3D/src/ThirdParty/Box2D/Rope/b2Rope.cpp"

	"../Siv3D/src/ThirdParty/GLFW/deps/glad.c"
	"../Siv3D/src/ThirdParty/GLFW/src/context.c"
	"../Siv3D/src/ThirdParty/GLFW/src/init.c"
	"../Siv3D/src/ThirdParty/GLFW/src/input.c"
	"../Siv3D/src/ThirdParty/GLFW/src/monitor.c"
	"../Siv3D/src/ThirdParty/GLFW/src/vulkan.c"
	"../Siv3D/src/ThirdParty/GLFW/src/window.c"
	"../Siv3D/src/ThirdParty/GLFW/src/linux_joystick.c"
	"../Siv3D/src/ThirdParty/GLFW/src/posix_time.c"
	"../Siv3D/src/ThirdParty/GLFW/src/posix_tls.c"
	"../Siv3D/src/ThirdParty/GLFW/src/xkb_unicode.c"
	"../Siv3D/src/ThirdParty/GLFW/src/egl_context.c"
	"../Siv3D/src/ThirdParty/GLFW/src/x11_init.c"
	"../Siv3D/src/ThirdParty/GLFW/src/x11_monitor.c"
	"../Siv3D/src/ThirdParty/GLFW/src/x11_window.c"
	"../Siv3D/src/ThirdParty/GLFW/src/glx_context.c"
)


add_library(Siv3D STATIC ${SOURCE_FILES})

target_link_libraries(Siv3D
)

