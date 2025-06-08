#pragma once
#include "Component.h"
#include "Event.h"

namespace bt
{
	class IngredientTileComponent final : public kob::Component
	{
	public:
		//--------------------------------------------------
		//    Constructor
		//--------------------------------------------------
		explicit IngredientTileComponent(kob::GameObject& parent);

		//--------------------------------------------------
		//    Loop
		//--------------------------------------------------
		void Update() override;
		void OnCollisionEnter(kob::GameObject& other) override;

		//--------------------------------------------------
		//    Events
		//--------------------------------------------------
		void ResetCollision();
		bool HasBeenSteppedOn() const;
		kob::Event<const IngredientTileComponent&> WasSteppedOnEvent = kob::Event<const IngredientTileComponent&>();

	private:
		bool m_HasBeenSteppedOn = false;
	};
}
