#pragma once

class TextMgr
{
public:
	using ID = font::ID;

	TextMgr();
	~TextMgr();

private:
	array<sf::Font, enum_count<ID>()> _fonts;
	optional<sf::Text> _text;

public:
	auto & GetFont(const ID id) { return _fonts[Under(id)]; }
	auto & GetFont(const ID id) const { return _fonts[Under(id)]; }

	void DrawText(const string & text, const Vector2f & pos);
};
