#include "PlayingCommands.h"
#include "GameObject.h"
#include "SDLSoundSystem.h"
#include "ServiceLocator.h"

// Move Command
bt::MoveCommand::MoveCommand(MovementComponent& component, const glm::vec3& dir)
	: m_pComponent(&component)
	, m_Direction(normalize(dir))
{}
void bt::MoveCommand::Execute()
{
	m_pComponent->Move(m_Direction);
}

// Pepper Command
bt::ThrowPepperCommand::ThrowPepperCommand(MovementComponent& moveComponent, ThrowPepperComponent& pepperComponent, float throwDst)
	: m_pMoveComponent(&moveComponent)
	, m_pThrowPepperComponent(&pepperComponent)
	, m_Dst(throwDst)
{ }
void bt::ThrowPepperCommand::Execute()
{
	const auto lookDir = m_pMoveComponent->GetDirection();
	m_pThrowPepperComponent->ThrowPepper(lookDir, m_Dst);
}

// Sound
void bt::ToggleMuteSoundCommand::Execute()
{
	m_Muted = !m_Muted;
	if (m_Muted)
	{
		m_GlobalVolumeScale = kob::ServiceLocator::GetSoundService().GetGlobalVolumeScale();
		kob::ServiceLocator::GetSoundService().StopAll();
		kob::ServiceLocator::RegisterSoundService(std::make_unique<kob::NullSoundSystem>());
	}
	else
	{
		kob::ServiceLocator::RegisterSoundService(std::make_unique<kob::SDLSoundSystem>());
		kob::ServiceLocator::GetSoundService().SetGlobalVolumeScale(m_GlobalVolumeScale);
	}
}
