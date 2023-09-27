#pragma once

#include "Texture2D.h"
#include "Axton/Core/Defines.h"
#include "Axton/Math/Math.h"

namespace Axton
{
	class Sprite
	{
	public:
		Sprite(const Ref<Texture2D>& texture, const Vector2& min, const Vector2& max);

		const Vector2* GetTexCoords() const { return m_TexCoords; }
		const Ref<Texture2D> GetTexture() const { return m_Texture; }

		static Ref<Sprite> Create(const Ref<Texture2D>& texture, const Vector2& coords, const Vector2& size);

	private:
		Ref<Texture2D> m_Texture;
		Vector2 m_TexCoords[4];
	};
}