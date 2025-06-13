#pragma once
#include "Command.h"
#include "MovementComponent.h"
#include "ThrowPepperComponent.h"
#include "vec3.hpp"

namespace kob {	class Animator; }
namespace bt
{
	class MoveCommand final : public kob::Command
	{
	public:
		MoveCommand(MovementComponent& component, const glm::vec3& dir);
		void Execute() override;
	private:
		MovementComponent* m_pComponent;
		glm::vec3 m_Direction;
	};
	class ThrowPepperCommand final : public kob::Command
	{
	public:
		ThrowPepperCommand(MovementComponent& moveComponent, ThrowPepperComponent& pepperComponent, float throwDst);
		void Execute() override;
	private:
		MovementComponent* m_pMoveComponent;
		ThrowPepperComponent* m_pThrowPepperComponent;
		float m_Dst{};
	};
}
