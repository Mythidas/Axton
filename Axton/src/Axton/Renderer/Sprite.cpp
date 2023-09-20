#include "axpch.h"
#include "Sprite.h"

namespace Axton
{
	Sprite::Sprite(const Ref<Texture2D>& texture, const Vector2& min, const Vector2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = Vector2(min.x, min.y);
		m_TexCoords[1] = Vector2(max.x, min.y);
		m_TexCoords[2] = Vector2(max.x, max.y);
		m_TexCoords[3] = Vector2(min.x, max.y);
	}

	Ref<Sprite> Sprite::Create(const Ref<Texture2D>& texture, const Vector2& coords, const Vector2& size)
	{
		Vector2 min = { coords.x * size.x / texture->GetWidth(), coords.y * size.y / texture->GetHeight() };
		Vector2 max = { (coords.x + 1) * size.x / texture->GetWidth(), (coords.y + 1) * size.y / texture->GetHeight() };
		return CreateRef<Sprite>(texture, min, max);
	}
}