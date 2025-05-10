#pragma once
#include "Command.h"
#include "MovementComponent.h"
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
}
