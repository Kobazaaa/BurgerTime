#pragma once
#include "Command.h"
#include "GameObject.h"

namespace kob {	class Animator; }
namespace bt
{
	class MoveCommand final : public kob::Command
	{
	public:
		MoveCommand(kob::GameObject& gameObject, const glm::vec3& dir, float speed);
		void Execute() override;
	private:
		kob::GameObject* m_pGameObject;
		kob::Animator* m_pAnimator;
		glm::vec3 m_Direction;
		float m_Speed;
	};
}
