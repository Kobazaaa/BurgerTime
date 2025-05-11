#pragma once
#include "Component.h"
#include "LevelComponent.h"
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
		void SetCurrentLevel(const LevelComponent& level);

	private:
		// Functions
		bool CanMoveUp() const;
		bool CanMoveDown() const;
		bool CanMoveLeft() const;
		bool CanMoveRight() const;

		// Data
		float m_Speed{};
		glm::vec2 m_Dir{};
		float m_AlignmentMargin{ 2.f };
		kob::Animator* m_pAnimator;
		const LevelComponent* m_pCurrentLevel;
	};
}
