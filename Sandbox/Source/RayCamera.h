#pragma once

#include "Axton.h"

using namespace Axton;

class RayCamera : public Camera
{
	struct Buffer
	{
		Vector4 Position{ 0.0f };
		Vector4 BackgroundColor{ 1.0f };
		Matrix4 View{ 0.0f };
		Matrix4 Projection{ 0.0f };
		float RandomSeed;
		uint32_t SampleSettings;
		uint32_t RenderPass;
		uint32_t Algorithm;
	};

public:
	enum class RenderModes
	{
		Full = 0,
		Normals,
		Albedo,
		Difficulty,
		TestLighting
	};

	friend class RayTraceLayer;
public:
	RayCamera(const Camera::Specs& specs);

	void OnUpdate();
	Ray GetRay() const;

	virtual void OnResize(uint32_t width, uint32_t height) override;
	virtual void RecalculateProjection() override;
	virtual void RecalculateView() override;

	RenderModes RenderMode() { return m_RenderMode; }
	void RenderMode(RenderModes mode) { m_RenderMode = mode; }

	Vector3 BackgroundColor() { return m_BackgroundColor; }
	void BackgroundColor(Vector3 color) { m_BackgroundColor = color; }

	uint32_t Algorithm() { return m_Algorithm; }
	void Algorithm(uint32_t algorithm) { m_Algorithm = algorithm; }

	uint32_t SampleSettings() { return m_SampleSettings; }
	void SampleSettings(uint32_t settings) { m_SampleSettings = settings; }

private:
	void ProcessMovement();

private:
	Vector2 m_LastMousePosition{ 0.0f };
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	Vector3 m_BackgroundColor{ 1.0f };
	RenderModes m_RenderMode{ RenderModes::Full };
	uint32_t m_SampleSettings = Bit::U32_4x8(1, 2, 0, 0);
	uint32_t m_Algorithm{ 0 };

	float m_RotationSpeed = 0.3f;

	Ref<RenderBuffer> m_CameraBuffer;
};