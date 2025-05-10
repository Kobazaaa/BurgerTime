#pragma once
#include "Component.h"
#include "vec2.hpp"

namespace bt
{
	class RotateComponent : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit RotateComponent(kob::GameObject& parent, float speed, float radius);


		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;

	private:
		float m_Speed			{};
		float m_Radius			{};
	};
}
