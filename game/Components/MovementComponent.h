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
		explicit MovementComponent(kob::GameObject& parent, float speed, bool isEnemy = false);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Start() override;
		void Update() override;

		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		void Move(const glm::vec2& direction);
		glm::vec2 GetDirection() const;
		void SetSpeed(float speed);
		void SetCurrentLevel(const LevelComponent& level);
		void Immobilize(bool stopAnimation = true);
		void Mobilize();
		bool IsImmobile() const;

	private:
		// Functions
		bool CanMoveUp() const;
		bool CanMoveDown() const;
		bool CanMoveLeft() const;
		bool CanMoveRight() const;

		// Data
		bool m_IsEnemy{ false };

		float m_Speed{};
		bool m_Immobilized{false};
		glm::vec2 m_Dir{};
		glm::vec2 m_MovementDir{};
		float m_AlignmentMargin{ 2.f };
		kob::Animator* m_pAnimator;
		const LevelComponent* m_pCurrentLevel;
	};
}
