#pragma once

#include <Axton.h>

using namespace Axton;

class ExampleLayer : public Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnUpdate() override;

private:
};