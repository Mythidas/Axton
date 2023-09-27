#pragma once

#include "Sprite.h"
#include "Axton/Core/Defines.h"

namespace Axton
{
	class AX_API SpriteSheet
	{
	public:
		SpriteSheet();
		SpriteSheet(float fps);

		void AddSprite(const std::string& name, const Ref<Sprite>& sprite);
		void RemoveSprite(const std::string& name);
		Ref<Sprite> GetSprite(const std::string& name);
		Ref<Sprite> GetSprite(int index);
		Ref<Sprite> GetNextSprite();

	private:
		std::unordered_map<std::string, Ref<Sprite>> m_Sprites;
		std::vector<std::string> m_SpriteNames;

		float m_FPS{ 0.0f };
		int m_CurrentFrameTime{ 0 };
		int m_CurrentSpriteIndex{ 0 };
	};
}