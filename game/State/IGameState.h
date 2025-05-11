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
		explicit IGameState(GameManagerComponent& gameManagerComp)
			: m_pGameManagerComp{ &gameManagerComp }
		{}
		virtual ~IGameState() = default;
		IGameState(const IGameState& other) = default;
		IGameState(IGameState&& other) noexcept = default;
		IGameState& operator=(const IGameState& other) = default;
		IGameState& operator=(IGameState&& other) noexcept = default;

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