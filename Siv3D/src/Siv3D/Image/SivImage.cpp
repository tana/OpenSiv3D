﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2018 Ryo Suzuki
//	Copyright (c) 2016-2018 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# include "../Siv3DEngine.hpp"
# include "../ImageFormat/IImageFormat.hpp"

# include <opencv2/imgproc.hpp>
# include <Siv3D/Image.hpp>
# include <Siv3D/ImageProcessing.hpp>
# include <Siv3D/BinaryWriter.hpp>
# include <Siv3D/MemoryWriter.hpp>
# include <Siv3D/Number.hpp>
# include <Siv3D/Logger.hpp>
# include <Siv3D/Emoji.hpp>
# include <Siv3D/Icon.hpp>

namespace s3d
{
	namespace detail
	{
		static constexpr bool IsValidSize(const size_t width, const size_t height)
		{
			return width <= Image::MaxWidth && height <= Image::MaxHeight;
		}

		static constexpr int32 ConvertBorderType(const BorderType borderType)
		{
			switch (borderType)
			{
			case BorderType::Replicate:
				return cv::BORDER_REPLICATE;
			//case BorderType::Wrap:
			//	return cv::BORDER_WRAP;
			case BorderType::Reflect:
				return cv::BORDER_REFLECT;
			case BorderType::Reflect_101:
				return cv::BORDER_REFLECT101;
			default:
				return cv::BORDER_DEFAULT;
			}
		}

		static void MakeSepia(const double levr, const double levg, const double levb, Color& pixel)
		{
			const double y = (0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
			const double r = levr + y;
			const double g = levg + y;
			const double b = levb + y;
			pixel.r = r >= 255.0 ? 255 : r <= 0.0 ? 0 : static_cast<uint8>(r);
			pixel.g = g >= 255.0 ? 255 : g <= 0.0 ? 0 : static_cast<uint8>(g);
			pixel.b = b >= 255.0 ? 255 : b <= 0.0 ? 0 : static_cast<uint8>(b);
		}

		static void SetupPostarizeTable(const int32 level, uint8 table[256])
		{
			const int32 levN = Clamp(level, 2, 256) - 1;

			for (size_t i = 0; i < 256; ++i)
			{
				table[i] = static_cast<uint8>(std::floor(i / 255.0 * levN + 0.5) / levN * 255);
			}
		}

		static void ToBinaryFromR(const Image& from, cv::Mat_<uint8>& to, const uint32 threshold)
		{
			assert(from.width() == to.cols);
			assert(from.height() == to.rows);

			const Color* pSrc = from[0];

			const int32 height = from.height(), width = from.width();

			for (int32 y = 0; y < height; ++y)
			{
				uint8* line = &to(y, 0);

				for (int32 x = 0; x < width; ++x)
				{
					line[x] = (pSrc->r <= threshold ? 0 : 255);

					++pSrc;
				}
			}
		}

		static void ToBinaryFromA(const Image& from, cv::Mat_<uint8>& to, const uint32 threshold)
		{
			assert(from.width() == to.cols);
			assert(from.height() == to.rows);

			const Color* pSrc = from[0];

			const int32 height = from.height(), width = from.width();

			for (int32 y = 0; y < height; ++y)
			{
				uint8* line = &to(y, 0);

				for (int32 x = 0; x < width; ++x)
				{
					line[x] = (pSrc->a <= threshold ? 0 : 255);

					++pSrc;
				}
			}
		}
	}

	Image Image::Generate(const size_t width, const size_t height, std::function<Color(void)> generator)
	{
		Image new_image(width, height);

		if (!new_image.isEmpty())
		{
			Color* pDst = new_image.data();
			const Color* pDstEnd = pDst + new_image.num_pixels();

			while (pDst != pDstEnd)
			{
				(*pDst++) = generator();
			}
		}

		return new_image;
	}

	Image Image::Generate0_1(const size_t width, const size_t height, std::function<Color(Vec2)> generator)
	{
		Image new_image(width, height);

		if (!new_image.isEmpty())
		{
			const double sx = 1.0 / (width - 1);
			const double sy = 1.0 / (height - 1);

			Color* pDst = new_image.data();

			for (uint32 y = 0; y < height; ++y)
			{
				for (uint32 x = 0; x < width; ++x)
				{
					(*pDst++) = generator({ sx * x, sy * y });
				}
			}
		}

		return new_image;
	}

	Image::Image(Image&& image) noexcept
		: m_data(std::move(image.m_data))
		, m_width(image.m_width)
		, m_height(image.m_height)
	{
		image.m_width = image.m_height = 0;
	}

	Image::Image(size_t width, size_t height)
		: m_data(detail::IsValidSize(width, height) ? width * height : 0)
		, m_width(detail::IsValidSize(width, height) ? static_cast<uint32>(width) : 0)
		, m_height(detail::IsValidSize(width, height) ? static_cast<uint32>(height) : 0)
	{
	
	}

	Image::Image(size_t width, size_t height, const Color& color)
		: m_data(detail::IsValidSize(width, height) ? width * height : 0, color)
		, m_width(detail::IsValidSize(width, height) ? static_cast<uint32>(width) : 0)
		, m_height(detail::IsValidSize(width, height) ? static_cast<uint32>(height) : 0)
	{
	
	}

	Image::Image(const FilePath& path)
		: Image(Siv3DEngine::GetImageFormat()->load(path))
	{

	}

	Image::Image(IReader&& reader, ImageFormat format)
		: Image(Siv3DEngine::GetImageFormat()->decode(std::move(reader), format))
	{

	}

	Image::Image(const FilePath& rgb, const FilePath& alpha)
		: Image(rgb)
	{
		applyAlphaFromRChannel(alpha);
	}

	Image::Image(const Color& rgb, const FilePath& alpha)
		: Image(alpha)
	{
		for (auto& pixel : *this)
		{
			pixel.a = pixel.r;
			pixel.r = rgb.r;
			pixel.g = rgb.g;
			pixel.b = rgb.b;
		}
	}

	Image::Image(const Emoji& emoji)
	{
		*this = Emoji::LoadImage(emoji.codePoints);
	}

	Image::Image(const Icon& icon)
	{
		*this = Icon::LoadImage(icon.code, icon.size);
	}

	Image::Image(const Grid<Color>& grid)
		: Image(grid.width(), grid.height())
	{
		if (m_data.empty())
		{
			return;
		}

		::memcpy(m_data.data(), grid.data(), grid.size_bytes());
	}

	Image::Image(const Grid<ColorF>& grid)
		: Image(grid.width(), grid.height())
	{
		if (m_data.empty())
		{
			return;
		}

		const ColorF* pSrc = grid.data();
		const ColorF* const pSrcEnd = pSrc + grid.size_elements();
		Color* pDst = m_data.data();

		while (pSrc != pSrcEnd)
		{
			*pDst++ = *pSrc++;
		}
	}

	Image& Image::operator =(Image&& image)
	{
		m_data = std::move(image.m_data);
		m_width = image.m_width;
		m_height = image.m_height;

		image.m_width = image.m_height = 0;

		return *this;
	}

	void Image::resize(const size_t width, const size_t height)
	{
		if (!detail::IsValidSize(width, height))
		{
			return clear();
		}

		if (width == m_width && height == m_height)
		{
			return;
		}

		m_data.resize(width * height);

		m_width = static_cast<uint32>(width);

		m_height = static_cast<uint32>(height);
	}

	void Image::resize(const size_t width, const size_t height, const Color& fillColor)
	{
		if (!detail::IsValidSize(width, height))
		{
			return clear();
		}

		if (width == m_width && height == m_height)
		{
			return;
		}

		m_data.assign(width * height, fillColor);

		m_width = static_cast<uint32>(width);

		m_height = static_cast<uint32>(height);
	}

	void Image::resizeRows(const size_t rows, const Color& fillColor)
	{
		if (rows == m_height)
		{
			return;
		}

		if (!detail::IsValidSize(m_width, rows))
		{
			return clear();
		}

		if (rows < m_height)
		{
			m_data.resize(m_width * rows);
		}
		else
		{
			m_data.insert(m_data.end(), m_width * (rows - m_height), fillColor);
		}

		m_height = static_cast<uint32>(rows);
	}

	Image Image::clipped(const Rect& rect) const
	{
		if (!detail::IsValidSize(rect.w, rect.h))
		{
			return Image();
		}

		Image tmp(rect.size, Color(0, 0));

		const int32 h = static_cast<int32>(m_height);
		const int32 w = static_cast<int32>(m_width);

		// [Siv3D ToDo] 最適化
		for (int32 y = 0; y < rect.h; ++y)
		{
			const int32 sy = y + rect.y;

			if (0 <= sy && sy < h)
			{
				for (int32 x = 0; x < rect.w; ++x)
				{
					const int32 sx = x + rect.x;

					if (0 <= sx && sx < w)
					{
						tmp[y][x] = operator[](sy)[sx];
					}
				}
			}
		}

		return tmp;
	}

	ColorF Image::sample_Repeat(const double x, const double y) const
	{
		const int32 ix = static_cast<int32>(x);
		const int32 iy = static_cast<int32>(y);

		const Color& c1 = getPixel_Repeat(iy, ix);
		const Color& c2 = getPixel_Repeat(iy, ix + 1);
		const Color& c3 = getPixel_Repeat(iy + 1, ix);
		const Color& c4 = getPixel_Repeat(iy + 1, ix + 1);

		const double xr1 = x - ix;
		const double yr1 = y - iy;

		const double r = Biliner(c1.r, c2.r, c3.r, c4.r, xr1, yr1);
		const double g = Biliner(c1.g, c2.g, c3.g, c4.g, xr1, yr1);
		const double b = Biliner(c1.b, c2.b, c3.b, c4.b, xr1, yr1);
		const double a = Biliner(c1.a, c2.a, c3.a, c4.a, xr1, yr1);

		return{ r / 255.0, g / 255.0, b / 255.0, a / 255.0 };
	}

	ColorF Image::sample_Clamp(const double x, const double y) const
	{
		const int32 ix = static_cast<int32>(x);
		const int32 iy = static_cast<int32>(y);

		const Color& c1 = getPixel_Clamp(iy, ix);
		const Color& c2 = getPixel_Clamp(iy, ix + 1);
		const Color& c3 = getPixel_Clamp(iy + 1, ix);
		const Color& c4 = getPixel_Clamp(iy + 1, ix + 1);

		const double xr1 = x - ix;
		const double yr1 = y - iy;

		const double r = Biliner(c1.r, c2.r, c3.r, c4.r, xr1, yr1);
		const double g = Biliner(c1.g, c2.g, c3.g, c4.g, xr1, yr1);
		const double b = Biliner(c1.b, c2.b, c3.b, c4.b, xr1, yr1);
		const double a = Biliner(c1.a, c2.a, c3.a, c4.a, xr1, yr1);

		return{ r / 255.0, g / 255.0, b / 255.0, a / 255.0 };
	}

	ColorF Image::sample_Mirror(const double x, const double y) const
	{
		const int32 ix = static_cast<int32>(x);
		const int32 iy = static_cast<int32>(y);

		const Color& c1 = getPixel_Mirror(iy, ix);
		const Color& c2 = getPixel_Mirror(iy, ix + 1);
		const Color& c3 = getPixel_Mirror(iy + 1, ix);
		const Color& c4 = getPixel_Mirror(iy + 1, ix + 1);

		const double xr1 = x - ix;
		const double yr1 = y - iy;

		const double r = Biliner(c1.r, c2.r, c3.r, c4.r, xr1, yr1);
		const double g = Biliner(c1.g, c2.g, c3.g, c4.g, xr1, yr1);
		const double b = Biliner(c1.b, c2.b, c3.b, c4.b, xr1, yr1);
		const double a = Biliner(c1.a, c2.a, c3.a, c4.a, xr1, yr1);

		return{ r / 255.0, g / 255.0, b / 255.0, a / 255.0 };
	}

	bool Image::applyAlphaFromRChannel(const FilePath& alpha)
	{
		if (isEmpty())
		{
			return false;
		}

		const Image alphaImage(alpha);

		if (alphaImage.isEmpty())
		{
			return false;
		}

		Color* pDst = data();
		const size_t dstStep = m_width;

		const Color* pSrc = alphaImage.data();
		const size_t srcStep = alphaImage.m_width;

		const uint32 w = std::min(m_width, alphaImage.m_width);
		const uint32 h = std::min(m_height, alphaImage.m_height);

		for (uint32 y = 0; y < h; ++y)
		{
			Color* pDstLine = pDst;
			const Color* pSrcLine = pSrc;

			for (uint32 x = 0; x < w; ++x)
			{
				(*pDstLine++).a = (*pSrcLine++).r;
			}

			pSrc += srcStep;
			pDst += dstStep;
		}

		return true;
	}
	
	bool Image::save(const FilePath& path, ImageFormat format) const
	{
		if (isEmpty())
		{
			return false;
		}

		if (format == ImageFormat::Unspecified)
		{
			format = Siv3DEngine::GetImageFormat()->getFormatFromFilePath(path);
		}

		return Siv3DEngine::GetImageFormat()->save(*this, format, path);
	}

	bool Image::savePNG(const FilePath& path, const PNGFilter::Flag filterFlag) const
	{
		if (isEmpty())
		{
			return false;
		}

		BinaryWriter writer(path);

		if (!writer)
		{
			return false;
		}

		return Siv3DEngine::GetImageFormat()->encodePNG(writer, *this, filterFlag);
	}

	bool Image::saveJPEG(const FilePath& path, const int32 quality) const
	{
		if (isEmpty())
		{
			return false;
		}

		BinaryWriter writer(path);

		if (!writer)
		{
			return false;
		}

		return Siv3DEngine::GetImageFormat()->encodeJPEG(writer, *this, quality);
	}

	bool Image::savePPM(const FilePath& path, const PPMType format) const
	{
		if (isEmpty())
		{
			return false;
		}

		BinaryWriter writer(path);

		if (!writer)
		{
			return false;
		}

		return Siv3DEngine::GetImageFormat()->encodePPM(writer, *this, format);
	}

	MemoryWriter Image::encode(ImageFormat format) const
	{
		if (isEmpty())
		{
			return MemoryWriter();
		}

		if (format == ImageFormat::Unspecified)
		{
			format = ImageFormat::PNG;
		}

		return Siv3DEngine::GetImageFormat()->encode(*this, format);
	}

	Image& Image::negate()
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			for (auto& pixel : m_data)
			{
				pixel = ~pixel;
			}
		}

		return *this;
	}

	Image Image::negated() const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(*this);

		for (auto& pixel : image)
		{
			pixel = ~pixel;
		}

		return image;
	}

	Image& Image::grayscale()
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			for (auto& pixel : m_data)
			{
				pixel.r = pixel.g = pixel.b = pixel.grayscale0_255();
			}
		}

		return *this;
	}

	Image Image::grayscaled() const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(*this);

		for (auto& pixel : image)
		{
			pixel.r = pixel.g = pixel.b = pixel.grayscale0_255();
		}

		return image;
	}

	Image& Image::sepia(const int32 level)
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			const double levn = Clamp(level, 0, 255);
			const double levr = 0.956*levn;
			const double levg = 0.274*levn;
			const double levb = -1.108*levn;

			for (auto& pixel : m_data)
			{
				detail::MakeSepia(levr, levg, levb, pixel);
			}
		}

		return *this;
	}

	Image Image::sepiaed(const int32 level) const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(*this);

		const double levn = Clamp(level, 0, 255);
		const double levr = 0.956*levn;
		const double levg = 0.274*levn;
		const double levb = -1.108*levn;

		for (auto& pixel : image)
		{
			detail::MakeSepia(levr, levg, levb, pixel);
		}

		return image;
	}

	Image& Image::postarize(const int32 level)
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			uint8 colorTable[256];

			detail::SetupPostarizeTable(level, colorTable);

			for (auto& pixel : m_data)
			{
				pixel.r = colorTable[pixel.r];
				pixel.g = colorTable[pixel.g];
				pixel.b = colorTable[pixel.b];
			}
		}

		return *this;
	}

	Image Image::postarized(const int32 level) const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(*this);

		uint8 colorTable[256];

		detail::SetupPostarizeTable(level, colorTable);

		for (auto& pixel : image)
		{
			pixel.r = colorTable[pixel.r];
			pixel.g = colorTable[pixel.g];
			pixel.b = colorTable[pixel.b];
		}

		return image;
	}

	Image& Image::brighten(const int32 level)
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			if (level < 0)
			{
				for (auto& pixel : m_data)
				{
					pixel.r = std::max(static_cast<int32>(pixel.r) + level, 0);
					pixel.g = std::max(static_cast<int32>(pixel.g) + level, 0);
					pixel.b = std::max(static_cast<int32>(pixel.b) + level, 0);
				}
			}
			else if (level > 0)
			{
				for (auto& pixel : m_data)
				{
					pixel.r = std::min(static_cast<int32>(pixel.r) + level, 255);
					pixel.g = std::min(static_cast<int32>(pixel.g) + level, 255);
					pixel.b = std::min(static_cast<int32>(pixel.b) + level, 255);
				}
			}
		}

		return *this;
	}

	Image Image::brightened(const int32 level) const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(*this);

		if (level < 0)
		{
			for (auto& pixel : image)
			{
				pixel.r = std::max(static_cast<int32>(pixel.r) + level, 0);
				pixel.g = std::max(static_cast<int32>(pixel.g) + level, 0);
				pixel.b = std::max(static_cast<int32>(pixel.b) + level, 0);
			}
		}
		else if (level > 0)
		{
			for (auto& pixel : image)
			{
				pixel.r = std::min(static_cast<int32>(pixel.r) + level, 255);
				pixel.g = std::min(static_cast<int32>(pixel.g) + level, 255);
				pixel.b = std::min(static_cast<int32>(pixel.b) + level, 255);
			}
		}

		return image;
	}

	Image& Image::mirror()
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			const int32 h = m_height, w = m_width, wHalf = m_width / 2;
			Color* line = m_data.data();

			for (int32 y = 0; y < h; ++y)
			{
				Color* lineA = line;
				Color* lineB = line + w - 1;;

				for (int32 x = 0; x < wHalf; ++x)
				{
					std::swap(*lineA, *lineB);
					++lineA;
					--lineB;
				}

				line += w;
			}
		}

		return *this;
	}

	Image Image::mirrored() const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		Image image(size());

		const Color* pSrc = data();
		Color* pDst = image.data();
		const size_t width = m_width;
		
		for (int32 y = 0; y < m_height; ++y)
		{
			for (int32 x = 0; x < m_width; ++x)
			{
				*(pDst + width * y + x) = *(pSrc + width * y + width - x - 1);
			}
		}

		return image;
	}

	Image& Image::flip()
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}
		}

		// 2. 処理
		{
			const int32 h = m_height, s = stride();
			Array<Color> line(m_width);
			Color* lineU = m_data.data();
			Color* lineB = lineU + m_width * (h - 1);

			for (int32 y = 0; y < h / 2; ++y)
			{
				::memcpy(line.data(), lineU, s);
				::memcpy(lineU, lineB, s);
				::memcpy(lineB, line.data(), s);

				lineU += m_width;
				lineB -= m_width;
			}
		}

		return *this;
	}



	Image& Image::gaussianBlur(const int32 horizontal, const int32 vertical, const BorderType borderType)
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}

			if ((horizontal < 0 || vertical < 0) || (horizontal == 0 && vertical == 0))
			{
				return *this;
			}
		}

		// 2. 処理
		{
			Image tmp(m_width, m_height);

			cv::Mat_<cv::Vec4b> matSrc(m_height, m_width, static_cast<cv::Vec4b*>(static_cast<void*>(data())), stride());

			cv::Mat_<cv::Vec4b> matDst(tmp.height(), tmp.width(), static_cast<cv::Vec4b*>(static_cast<void*>(tmp.data())), tmp.stride());

			cv::GaussianBlur(matSrc, matDst, cv::Size(horizontal * 2 + 1, vertical * 2 + 1), 0.0, 0.0, detail::ConvertBorderType(borderType));

			swap(tmp);
		}

		return *this;
	}

	Image Image::gaussianBlurred(const int32 horizontal, const int32 vertical, const BorderType borderType) const
	{
		// 1. パラメータチェック
		{
			if (isEmpty())
			{
				return *this;
			}

			if ((horizontal < 0 || vertical < 0) || (horizontal == 0 && vertical == 0))
			{
				return *this;
			}
		}

		Image image(m_width, m_height);

		cv::Mat_<cv::Vec4b> matSrc(m_height, m_width, const_cast<cv::Vec4b*>(static_cast<const cv::Vec4b*>(static_cast<const void*>(data()))), stride());

		cv::Mat_<cv::Vec4b> matDst(image.height(), image.width(), static_cast<cv::Vec4b*>(static_cast<void*>(image.data())), image.stride());

		cv::GaussianBlur(matSrc, matDst, cv::Size(horizontal * 2 + 1, vertical * 2 + 1), 0.0, 0.0, detail::ConvertBorderType(borderType));

		return image;
	}


	namespace ImageProcessing
	{
		Polygon FindExternalContour(const Image& image, bool useAlpha, uint32 threshold)
		{
			const auto polygons = ImageProcessing::FindExternalContours(image, useAlpha, threshold);

			if (polygons.isEmpty())
			{
				return Polygon();
			}

			if (polygons.size() == 1)
			{
				return polygons.front();
			}

			double maxArea = 0.0;

			size_t index = 0;

			for (size_t i = 0; i < polygons.size(); ++i)
			{
				const double area = polygons[i].area();

				if (area > maxArea)
				{
					maxArea = area;

					index = i;
				}
			}

			return polygons[index];
		}

		Array<Polygon> FindExternalContours(const Image& image, bool useAlpha, uint32 threshold)
		{
			if (!image)
			{
				return{};
			}

			cv::Mat_<uint8> gray(image.height(), image.width());

			if (useAlpha)
			{
				detail::ToBinaryFromA(image, gray, threshold);
			}
			else
			{
				detail::ToBinaryFromR(image, gray, threshold);
			}

			std::vector<std::vector<cv::Point>> contours;

			try
			{
				cv::findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, { 0, 0 });
			}
			catch (cv::Exception&)
			{
				//LOG_FAIL(LogMessage::Image003);

				return{};
			}

			Array<Polygon> result;

			for (const auto& contour : contours)
			{
				Array<Vec2> external;

				external.reserve(contour.size());

				for (int i = static_cast<int>(contour.size()) - 1; i >= 0; --i)
				{
					external.emplace_back(contour[i].x, contour[i].y);
				}

				if (external.size() >= 3)
				{
					result.emplace_back(external);
				}
			}

			return result;
		}

		Polygon FindContour(const Image& image, bool useAlpha, uint32 threshold)
		{
			const auto polygons = ImageProcessing::FindContours(image, useAlpha, threshold);

			if (polygons.isEmpty())
			{
				return Polygon();
			}

			if (polygons.size() == 1)
			{
				return polygons.front();
			}

			double maxArea = 0.0;

			size_t index = 0;

			for (size_t i = 0; i < polygons.size(); ++i)
			{
				const double area = polygons[i].area();

				if (area > maxArea)
				{
					maxArea = area;

					index = i;
				}
			}

			return polygons[index];
		}

		Array<Polygon> FindContours(const Image& image, bool useAlpha, uint32 threshold)
		{
			if (!image)
			{
				return{};
			}

			cv::Mat_<uint8> gray(image.height(), image.width());

			if (useAlpha)
			{
				detail::ToBinaryFromA(image, gray, threshold);
			}
			else
			{
				detail::ToBinaryFromR(image, gray, threshold);
			}

			std::vector<std::vector<cv::Point>> contours;

			std::vector<cv::Vec4i> hierarchy;

			try
			{
				cv::findContours(gray, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE, { 0, 0 });
			}
			catch (cv::Exception&)
			{
				//LOG_FAIL(LogMessage::Image003);

				return{};
			}

			Array<Polygon> result;

			for (size_t i = 0; i < contours.size(); i = hierarchy[i][0])
			{
				const auto& contour = contours[i];

				Array<Vec2> external;

				external.reserve(contour.size());

				for (int k = static_cast<int>(contour.size()) - 1; k >= 0; --k)
				{
					external.emplace_back(contour[k].x, contour[k].y);
				}

				Array<Array<Vec2>> holes;

				for (int k = hierarchy[i][2]; k != -1; k = hierarchy[k][0])
				{
					const auto& contour2 = contours[k];

					Array<Vec2> hole;

					hole.reserve(contour2.size());

					for (const auto& p : contour2)
					{
						hole.emplace_back(p.x, p.y);
					}

					std::reverse(hole.begin(), hole.end());

					holes.push_back(std::move(hole));
				}

				if (external.size() >= 3)
				{
					result.emplace_back(external, holes);
				}
			}

			return result;
		}
	}
}
