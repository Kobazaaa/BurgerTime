#pragma once
namespace bt
{
	class GameManagerComponent;
	class IGameState
	{
	public:
		//--------------------------------------------------
		//    Constructor & Destructor
		//--------------------------------------------------
		IGameState(GameManagerComponent& gameManagerComp)
			: m_pGameManagerComp{ &gameManagerComp }
		{};
		virtual ~IGameState() = default;

		//--------------------------------------------------
		//    Interface
		//--------------------------------------------------
		virtual IGameState* Update() = 0;
		virtual void OnEnter() = 0;
		virtual void OnExit() = 0;

	protected:
		//--------------------------------------------------
		//    Accessors & Mutators
		//--------------------------------------------------
		GameManagerComponent* GetGameManager() const { return m_pGameManagerComp; }

	private:
		// Data
		GameManagerComponent* m_pGameManagerComp{ };
	};
}