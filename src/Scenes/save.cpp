#include "pch.hpp"
#include "save.hpp"
#include "ECS/entityUtil.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"

namespace ph {

struct GameSaveBlob
{
	Vec2 savePointPos;
	Vec2 playerPos;
	int playerHealth;
	u8 playerZ;
};

using namespace component;

void saveGame(entt::registry* registry, Vec2 savePointPos)
{
	// TODO: Handle file opening fail

	GameSaveBlob blob;
	blob.savePointPos = savePointPos;
	registry->view<Player, BodyRect, RenderQuad, Health>().each([&]
	(auto, auto body, const auto& renderQuad, auto health)
	{
		blob.playerPos = body.pos;
		blob.playerHealth = health.healthPoints;
		blob.playerZ = renderQuad.z;
	});

	FILE* file = fopen("save", "wb");
	fwrite(&blob, sizeof(GameSaveBlob), 1, file);
	fclose(file);
}

void loadGameSave(entt::registry* registry)
{
	// TODO: Handle file opening fail

	GameSaveBlob blob;
	FILE* file = fopen("save", "rb");
	fread(&blob, sizeof(GameSaveBlob), 1, file);
	fclose(file);

	registry->view<Player, BodyRect, RenderQuad, Health>().each([&]
	(auto, auto& body, auto& renderQuad, auto& health)
	{
		body.pos = blob.playerPos;
		health.healthPoints = blob.playerHealth;
		renderQuad.z = blob.playerZ;
	});

	registry->view<SavePoint, BodyRect>().each([&]
	(auto& savePoint, auto body)
	{
		if(body.pos == blob.savePointPos)
			savePoint.isIntersectingPlayer = true;
	});
}

}
