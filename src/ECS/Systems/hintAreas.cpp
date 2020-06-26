#include "pch.hpp"
#include "hintAreas.hpp"
#include "GUI/gui.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/simRegionComponents.hpp"
#include "ECS/Systems/gunAttacks.hpp"
#include "ECS/Systems/meleeAttacks.hpp"
#include "ECS/Systems/playerMovementInput.hpp"
#include "ECS/entityUtil.hpp"

namespace ph::system {

using namespace component;

void HintAreas::update(float dt)
{
	PH_PROFILE_FUNCTION();

	if(sPause || !isPlayerOnScene()) return;

	auto playerPos = getPlayerCenterPos();

	mRegistry.view<InsideSimRegion, Hint, BodyRect>().each([&]
	(auto, auto& hint, auto hintAreaBody)
	{
		if(hintAreaBody.contains(playerPos))
		{
			PH_ASSERT_UNEXPECTED_SITUATION(GUI::hasInterface("hints"),
				"Player walked into hint area but gui does not have hints Interface!");

			hint.isShown = true;
			auto* hints = GUI::getInterface("hints");
			hints->show();
			auto* hintBackground = hints->getWidget("hintBackground");
			auto* hintContent = Cast<TextWidget*>(hintBackground->getWidget("hintContent"));
			if(sf::Joystick::isConnected(0))
				hintContent->setText(hint.joystickContent);
			else	
				hintContent->setText(hint.keyboardContent);

			// NOTE: all controls hints enable and disable everything
			//       to avoid weird stuff when you're for example teleporting

			if(hint.hintName == "controlHint" && !mWasPlayerInControlHint) 
			{
				mWasPlayerInControlHint = true;
				GunAttacks::shootInputDisabled = true;
				GunAttacks::changeWeaponInputDisabled = true;
				MeleeAttacks::inputDisabled = true;
				PlayerMovementInput::dashInputDisabled = true;
			}
			else if(hint.hintName == "meleeFightingHint") 
			{
				mWasPlayerInControlHint = true;
				GunAttacks::shootInputDisabled = true;
				GunAttacks::changeWeaponInputDisabled = true;
				MeleeAttacks::inputDisabled = false;
				PlayerMovementInput::dashInputDisabled = true;
			}
			else if(hint.hintName == "shootingHint") 
			{
				mWasPlayerInControlHint = true;
				GunAttacks::shootInputDisabled = false;
				GunAttacks::changeWeaponInputDisabled = true;
				MeleeAttacks::inputDisabled = false;
				PlayerMovementInput::dashInputDisabled = false;
			}
			else if(hint.hintName == "weaponChangingHint") 
			{
				mWasPlayerInControlHint = true;
				GunAttacks::shootInputDisabled = false;
				GunAttacks::changeWeaponInputDisabled = false;
				MeleeAttacks::inputDisabled = false;
				PlayerMovementInput::dashInputDisabled = false;
			}
			else if(hint.hintName == "dashingHint") 
			{
				mWasPlayerInControlHint = true;
				GunAttacks::shootInputDisabled = true;
				GunAttacks::changeWeaponInputDisabled = true;
				MeleeAttacks::inputDisabled = false;
				PlayerMovementInput::dashInputDisabled = false;
			}
		}
		else if(hint.isShown)
		{
			GUI::hideInterface("hints");
			hint.isShown = false;
		}
	});
}

}

