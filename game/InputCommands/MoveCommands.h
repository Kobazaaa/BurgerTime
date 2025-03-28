#pragma once

#include "Command.h"
#include "GameObject.h"

namespace kob
{
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(GameObject& gameobject, const glm::vec3& dir, float speed);
		void Execute() override;
	private:
		GameObject* m_pGameObject;
		glm::vec3 m_Direction;
		float m_Speed;
	};
}