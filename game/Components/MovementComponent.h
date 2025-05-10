#pragma once
#include "Component.h"
#include "vec2.hpp"

namespace kob
{
	class Animator;
}

namespace bt
{
	class MovementComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit MovementComponent(kob::GameObject& parent, float speed);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		void Move(const glm::vec2& direction);
		void SetSpeed(float speed);


	private:
		float m_Speed{};
		glm::vec2 m_Dir{};
		kob::Animator* m_pAnimator;
	};
}
