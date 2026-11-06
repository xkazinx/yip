#include "stdafx.h"
#include "TextMgr.h"
#include "App.h"
#include <filesystem>
#ifdef _WIN32
#include <windows.h>
#undef DrawText // windows.h's DrawTextW macro, not TextMgr::DrawText
#endif

namespace
{
	// The fonts sit next to the exe (deploy\), wherever it is started from; in a
	// browser at the root of the page's files (--preload-file, src\web\CMakeLists.txt)
	std::filesystem::path ExeDir()
	{
#ifdef _WIN32
		wchar_t path[MAX_PATH] = {};
		GetModuleFileNameW(nullptr, path, MAX_PATH);
		return std::filesystem::path(path).parent_path();
#else
		return "/";
#endif
	}
}

TextMgr::TextMgr()
{
	// Load fonts from font ids
	for (const auto id : enum_values<ID>()) {
		const auto path = ExeDir() / font::kNames[Under(id)];
		if (!_fonts[Under(id)].openFromFile(path))
			cout << "(!) cannot open the font " << path.string() << endl;
	}

	_text.emplace(GetFont(ID::Arial));

	_text->setCharacterSize(24); // in pixels, not points!
	_text->setFillColor(sf::Color::Red);
}

TextMgr::~TextMgr()
{
}

void TextMgr::DrawText(const string & text, const Vector2f & pos)
{
	_text->setString(text);
	_text->setPosition(pos);

	g_app->_window.draw(*_text);
}
