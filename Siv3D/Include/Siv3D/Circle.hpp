﻿//-----------------------------------------------
//
//	This file is part of the Siv3D Engine.
//
//	Copyright (c) 2008-2017 Ryo Suzuki
//	Copyright (c) 2016-2017 OpenSiv3D Project
//
//	Licensed under the MIT License.
//
//-----------------------------------------------

# pragma once
# include "Fwd.hpp"
# include "PointVector.hpp"
# include "Line.hpp"
# include "NamedParameter.hpp"

namespace s3d
{
	struct Circle
	{
		using position_type = Vec2;

		using size_type = position_type::value_type;

		using value_type = position_type::value_type;

		S3D_DISABLE_MSVC_WARNINGS_PUSH(4201)

		union
		{
			/// <summary>
			/// 円の中心座標
			/// </summary>
			position_type center;

			struct
			{
				/// <summary>
				/// 円の中心の X 座標
				/// </summary>
				value_type x;

				/// <summary>
				/// 円の中心の Y 座標
				/// </summary>
				value_type y;
			};
		};

		/// <summary>
		/// 円の半径
		/// </summary>
		size_type r;

		S3D_DISABLE_MSVC_WARNINGS_POP()

		Circle() = default;

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		explicit constexpr Circle(size_type _r) noexcept
			: center(0.0, 0.0)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_x">
		/// 円の中心の X 座標
		/// </param>
		/// <param name="_y">
		/// 円の中心の Y 座標
		/// </param>
		constexpr Circle(value_type _x, value_type _y) noexcept
			: center(_x, _y)
			, r(0.0) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_x">
		/// 円の中心の X 座標
		/// </param>
		/// <param name="_y">
		/// 円の中心の Y 座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(value_type _x, value_type _y, size_type _r) noexcept
			: center(_x, _y)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_center">
		/// 円の中心の座標
		/// </param>
		explicit constexpr Circle(const position_type& _center) noexcept
			: center(_center)
			, r(0.0) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_center">
		/// 円の中心の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(const position_type& _center, size_type _r) noexcept
			: center(_center)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="_center">
		/// 円の中心の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(Arg::center_<position_type> _center, size_type _r) noexcept
			: center(_center.value())
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="topLeft">
		/// 円に外接する正方形の左上の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(Arg::topLeft_<position_type> topLeft, size_type _r) noexcept
			: center(topLeft->x + _r, topLeft->y + _r)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="topRight">
		/// 円に外接する正方形の右上の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(Arg::topRight_<position_type> topRight, size_type _r) noexcept
			: center(topRight->x + _r, topRight->y + _r)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="bottomLeft">
		/// 円に外接する正方形の左下の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(Arg::bottomLeft_<position_type> bottomLeft, size_type _r) noexcept
			: center(bottomLeft->x + _r, bottomLeft->y - _r)
			, r(_r) {}

		/// <summary>
		/// 円を作成します。
		/// </summary>
		/// <param name="bottomRight">
		/// 円に外接する正方形の右下の座標
		/// </param>
		/// <param name="_r">
		/// 円の半径
		/// </param>
		constexpr Circle(Arg::bottomRight_<position_type> bottomRight, size_type _r) noexcept
			: center(bottomRight->x - _r, bottomRight->y - _r)
			, r(_r) {}

		Circle(const position_type& p0, const position_type& p1) noexcept
			: center((p0 + p1) / 2.0)
			, r(p0.distanceFrom(p1) / 2.0) {}

		Circle(const position_type& p0, const position_type& p1, const position_type& p2) noexcept;

		explicit Circle(const Line& diameter) noexcept
			: Circle(diameter.begin, diameter.end) {}

		Circle(Arg::center_<position_type> _center, const position_type& p) noexcept
			: center(_center.value())
			, r(p.distanceFrom(_center.value())) {}
	};
}

//////////////////////////////////////////////////////////////////////////////
//
//	Formatting Circle
//
//	[x] Siv3D Formatter
//	[x] ostream
//	[x] wostream
//	[x] istream
//	[x] wistream
//	[x] fmtlib BasicFormatter<wchar>
//
namespace s3d
{
	inline void Formatter(FormatData& formatData, const Circle& value)
	{
		Formatter(formatData, Vec3(value.x, value.y, value.r));
	}

	/// <summary>
	/// 出力ストリームに円を渡します。
	/// </summary>
	/// <param name="os">
	/// 出力ストリーム
	/// </param>
	/// <param name="circle">
	/// 円
	/// </param>
	/// <returns>
	/// 渡した後の出力ストリーム
	/// </returns>
	template <class CharType>
	inline std::basic_ostream<CharType>& operator <<(std::basic_ostream<CharType>& os, const Circle& circle)
	{
		return	os << CharType('(')
			<< circle.x << CharType(',')
			<< circle.y << CharType(',')
			<< circle.r << CharType(')');
	}

	/// <summary>
	/// 入力ストリームに円を渡します。
	/// </summary>
	/// <param name="is">
	/// 入力ストリーム
	/// </param>
	/// <param name="circle">
	/// 円
	/// </param>
	/// <returns>
	/// 渡した後の入力ストリーム
	/// </returns>
	template <class CharType>
	inline std::basic_istream<CharType>& operator >>(std::basic_istream<CharType>& is, Circle& circle)
	{
		CharType unused;
		return	is >> unused
			>> circle.x >> unused
			>> circle.y >> unused
			>> circle.r >> unused;
	}
}

namespace fmt
{
	template <class ArgFormatter>
	void format_arg(BasicFormatter<s3d::wchar, ArgFormatter>& f, const s3d::wchar*& format_str, const s3d::Circle& circle)
	{
		const auto tag = s3d::detail::GetTag(format_str);

		const auto fmt = L"({" + tag + L"},{" + tag + L"},{" + tag + L"})";

		f.writer().write(fmt, circle.x, circle.y, circle.r);
	}
}
//
//////////////////////////////////////////////////////////////////////////////