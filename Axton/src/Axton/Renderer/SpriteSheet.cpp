#include "axpch.h"
#include "SpriteSheet.h"
#include "Axton/Core/Time.h"

namespace Axton
{
	SpriteSheet::SpriteSheet()
		: m_FPS(0.0f)
	{
	}

	SpriteSheet::SpriteSheet(float fps)
		: m_FPS(fps)
	{
		m_CurrentFrameTime = 1000.0f / m_FPS;
	}

	void SpriteSheet::AddSprite(const std::string& name, const Ref<Sprite>& sprite)
	{
		m_Sprites[name] = sprite;
		m_SpriteNames.push_back(name);
	}

	void SpriteSheet::RemoveSprite(const std::string& name)
	{
		m_Sprites.erase(name);

		for (int i = 0; i < m_SpriteNames.size(); i++)
		{
			if (m_SpriteNames[i] == name)
			{
				m_SpriteNames.erase(m_SpriteNames.begin() + i);
				break;
			}
		}
	}

	Ref<Sprite> SpriteSheet::GetSprite(const std::string& name)
	{
		return m_Sprites[name];
	}

	Ref<Sprite> SpriteSheet::GetSprite(int index)
	{
		return m_Sprites[m_SpriteNames[index]];
	}

	Ref<Sprite> SpriteSheet::GetNextSprite()
	{
		if (m_FPS == 0.0f)
			return nullptr;

		if (m_CurrentFrameTime <= 0)
		{
			m_CurrentFrameTime = 1000.0f / m_FPS;
			m_CurrentSpriteIndex++;
			if (m_CurrentSpriteIndex >= m_Sprites.size())
				m_CurrentSpriteIndex = 0;
		}
		else
		{
			m_CurrentFrameTime -= Time::GetDeltaTime();
		}
		
		return GetSprite(m_CurrentSpriteIndex);
	}
}