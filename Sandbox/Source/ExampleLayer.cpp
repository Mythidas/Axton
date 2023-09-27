//#include "ExampleLayer.h"
//#include "FlyCamera.h"
//
//static Entity entity(-1);
//static Transform* transform;
//
//void ExampleLayer::OnAttach()
//{
//	World::SetActiveLevel(CreateRef<Level>());
//	Renderer::SetLogging(true);
//
//	Entity cameraEntity = World::CreateEntity();
//	Camera* camera = World::AddComponent<Camera>(cameraEntity);
//	camera->Data.Format = CameraData::ProjectionFormat::Perspective;
//	World::AddComponent<FlyCamera>(cameraEntity);
//	World::GetComponent<Transform>(cameraEntity)->Position = { 0.0f, 0.0f, -1.0f };
//
//	Ref<Texture2D> texture = Texture2D::Create(Texture2DSpecs(), "C:\\Programming\\Axton\\Sandbox\\Assets\\Textures\\Idle-Sheet.png");
//	Ref<SpriteSheet> knightSprites = CreateRef<SpriteSheet>(30.0f);
//	knightSprites->AddSprite("knight_01", Sprite::Create(texture, { 0, 0 }, { 32, 32 }));
//	knightSprites->AddSprite("knight_02", Sprite::Create(texture, { 2, 0 }, { 32, 32 }));
//	knightSprites->AddSprite("knight_03", Sprite::Create(texture, { 1, 0 }, { 32, 32 }));
//	knightSprites->AddSprite("knight_04", Sprite::Create(texture, { 2, 0 }, { 32, 32 }));
//	knightSprites->AddSprite("knight_05", Sprite::Create(texture, { 3, 0 }, { 32, 32 }));
//
//	Ref<Texture2D> dungeonTexture = Texture2D::Create(Texture2DSpecs(), "C:\\Programming\\Axton\\Sandbox\\Assets\\Textures\\Environment\\Dungeon Prison\\Assets\\Tiles.png");
//	Ref<SpriteSheet> dungeonSprites = CreateRef<SpriteSheet>();
//	dungeonSprites->AddSprite("dungeon_bl_01", Sprite::Create(dungeonTexture, { 4, 22 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_bm_01", Sprite::Create(dungeonTexture, { 5, 22 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_br_01", Sprite::Create(dungeonTexture, { 6, 22 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_ml_01", Sprite::Create(dungeonTexture, { 4, 23 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_mm_01", Sprite::Create(dungeonTexture, { 5, 23 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_mr_01", Sprite::Create(dungeonTexture, { 6, 23 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_tl_01", Sprite::Create(dungeonTexture, { 4, 24 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_tm_01", Sprite::Create(dungeonTexture, { 5, 24 }, { 16, 16 }));
//	dungeonSprites->AddSprite("dungeon_tr_01", Sprite::Create(dungeonTexture, { 6, 24 }, { 16, 16 }));
//
//	Entity entityTile1(-1);
//	Transform* transform1 = nullptr;
//	SpriteRenderer* spriteRenderer1 = nullptr;
//	SpriteMap* spriteMap1 = nullptr;
//
//	entityTile1 = World::CreateEntity();
//	spriteMap1 = World::AddComponent<SpriteMap>(entityTile1);
//	spriteMap1->SpriteSheet = dungeonSprites;
//	spriteMap1->Map = {
//		{ 6, 7, 7, 7, 7, 8 },
//		{ 3, 4, 4, 4, 4, 5 },
//		{ 3, 4, 4, 4, 4, 5 },
//		{ 3, 4, 4, 4, 4, 5 },
//		{ 3, 4, 4, 4, 4, 5 },
//		{ 0, 1, 1, 1, 1, 2 }
//	};
//	transform1 = World::GetComponent<Transform>(entityTile1);
//	transform1->Position = { 0.0f, 0.0f, 0.0f };
//}
//
//void ExampleLayer::OnUpdate()
//{
//	World::GetActiveLevel()->OnUpdate();
//}