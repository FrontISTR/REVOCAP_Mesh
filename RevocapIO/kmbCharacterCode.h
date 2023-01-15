#pragma once

#include <string>
#include <vector>
#include <codecvt>

namespace kmb {
	class CharacterCode {
	private:
		static std::wstring utf8_to_wide(std::string const& src)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(src);
		}
		static std::string wide_to_multi(std::wstring const& src)
		{
			std::size_t converted{};
			std::vector<char> dest(src.size() * sizeof(wchar_t) + 1, '\0');
			if (::_wcstombs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
				throw std::system_error{ errno, std::system_category() };
			}
			dest.resize(std::char_traits<char>::length(dest.data()));
			dest.shrink_to_fit();
			return std::string(dest.begin(), dest.end());
		}
	public:
		static std::string utf8_to_multi(std::string const& src)
		{
			std::wstring const wide = utf8_to_wide(src);
			return wide_to_multi(wide);
		}
	};
}