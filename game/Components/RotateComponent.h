#pragma once
#include "Component.h"
#include "vec2.hpp"

namespace kob
{
	class RotateComponent : public Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit RotateComponent(GameObject& parent, float speed, float radius);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;

	private:
		float m_Speed			{};
		float m_Radius			{};
	};
}
