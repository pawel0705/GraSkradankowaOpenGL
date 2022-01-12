#include "pch.h"
#include "maze.h"
#include "../Rendering system/Model/objReader.h"
#include "tileType.h"
#include <set>

Maze::Maze()
{
	this->initMaze();
}

void Maze::initMaze()
{
	std::string mazeDataFileName = "Maps/map_1.txt";

	std::ifstream mazefile;
	mazefile.open(mazeDataFileName);

	if (!mazefile.is_open())
	{
		std::cout << "Error while reading maze data file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string singleline;

	std::getline(mazefile, singleline);
	this->mazeDimensionX = atoi(singleline.c_str());
	std::getline(mazefile, singleline);
	this->mazeDimensionY = atoi(singleline.c_str());
	this->mazeIndexData = new int* [this->mazeDimensionX];

	for (int i = 0; i < this->mazeDimensionX; i++)
	{
		this->mazeIndexData[i] = new int[this->mazeDimensionY];
	}


	for (int j = 0; !mazefile.eof(); j++)
	{
		std::getline(mazefile, singleline);

		std::stringstream maze_line(singleline);
		for (int i = 0; i < this->mazeDimensionY; i++)
		{
			maze_line >> this->mazeIndexData[i][j];
		}
	}

	mazefile.close();

	this->initMazeMaterials();
	this->initMazeTextures();
	this->initObjModels();
	this->initMazeShaders();
	this->initMatrixMVP();

	//pointLights.push_back(Light::Point { {30.0f, 0.0f, 52.0f}, {1.0f, 1.0f, 1.0f} });
}

void Maze::initMatrixMVP()
{
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(this->camera->getCameraZoom()),
		(float)Config::g_defaultWidth / (float)Config::g_defaultHeight,
		0.1f,
		100.0f
	);

	this->shaderProgram->useShader();
	this->shaderProgram->setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(this->shaderProgram);

	this->shaderGrassProgram->useShader();
	this->shaderGrassProgram->setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(this->shaderGrassProgram);

	this->shaderPickupProgram->useShader();
	this->shaderPickupProgram->setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(this->shaderPickupProgram);

	this->shaderParticles.useShader();
	this->shaderParticles.setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(&this->shaderParticles);

	this->shaderGeometryPass.useShader();
	this->shaderGeometryPass.setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(&this->shaderGeometryPass);

	this->shaderOITpickup.useShader();
	this->shaderOITpickup.setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(&this->shaderOITpickup);

	this->shaderOITparticle.useShader();
	this->shaderOITparticle.setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(&this->shaderOITparticle);
}

void Maze::initMazeShaders()
{

	this->fragmentShader = Shader::createShaderFromFile("Shaders/map.frag", Shader::Type::eFragment);
	this->vertexShader = Shader::createShaderFromFile("Shaders/map.vert", Shader::Type::eVertex);

	this->shaderProgram = new ShaderProgram();
	this->shaderProgram->attachShader(this->fragmentShader);
	this->shaderProgram->attachShader(this->vertexShader);
	this->shaderProgram->linkShaderProgram();

	this->fragmentShaderGrass = Shader::createShaderFromFile("Shaders/mapGrass.frag", Shader::Type::eFragment);
	this->vertexShaderGrass = Shader::createShaderFromFile("Shaders/map.vert", Shader::Type::eVertex);

	this->shaderGrassProgram = new ShaderProgram();
	this->shaderGrassProgram->attachShader(this->fragmentShaderGrass);
	this->shaderGrassProgram->attachShader(this->vertexShaderGrass);
	this->shaderGrassProgram->linkShaderProgram();

	this->fragmentShaderPickup = Shader::createShaderFromFile("Shaders/mapPickup.frag", Shader::Type::eFragment);
	this->vertexShaderPickup = Shader::createShaderFromFile("Shaders/map.vert", Shader::Type::eVertex);

	this->shaderPickupProgram = new ShaderProgram();
	this->shaderPickupProgram->attachShader(this->fragmentShaderPickup);
	this->shaderPickupProgram->attachShader(this->vertexShaderPickup);
	this->shaderPickupProgram->linkShaderProgram();

	Shader particlesVert = Shader::createShaderFromFile("Shaders/particle.vert", Shader::Type::eVertex);
	Shader particlesGeom = Shader::createShaderFromFile("Shaders/particle.geom", Shader::Type::eGeometry);
	Shader particlesFrag = Shader::createShaderFromFile("Shaders/particle.frag", Shader::Type::eFragment);

	shaderParticles.attachShader(particlesVert);
	shaderParticles.attachShader(particlesGeom);
	shaderParticles.attachShader(particlesFrag);
	shaderParticles.linkShaderProgram();



	Shader geometryPassVert = Shader::createShaderFromFile("Shaders/map_gBuffer.vert", Shader::Type::eVertex);
	Shader geometryPassFrag = Shader::createShaderFromFile("Shaders/map_gBuffer.frag", Shader::Type::eFragment);

	shaderGeometryPass.attachShader(geometryPassVert);
	shaderGeometryPass.attachShader(geometryPassFrag);
	shaderGeometryPass.linkShaderProgram();

	Shader lightingPassVert = Shader::createShaderFromFile("Shaders/map_deferred.vert", Shader::Type::eVertex);
	Shader lightingPassFrag = Shader::createShaderFromFile("Shaders/map_deferred.frag", Shader::Type::eFragment);

	shaderLightingPass.attachShader(lightingPassVert);
	shaderLightingPass.attachShader(lightingPassFrag);
	shaderLightingPass.linkShaderProgram();



	Shader OITpickupVert = Shader::createShaderFromFile("Shaders/OIT/map.vert", Shader::Type::eVertex);
	Shader OITpickupFrag = Shader::createShaderFromFile("Shaders/OIT/mapPickup.frag", Shader::Type::eFragment);

	shaderOITpickup.attachShader(OITpickupVert);
	shaderOITpickup.attachShader(OITpickupFrag);
	shaderOITpickup.linkShaderProgram();

	Shader OITparticleVert = Shader::createShaderFromFile("Shaders/OIT/particle.vert", Shader::Type::eVertex);
	Shader OITparticleGeom = Shader::createShaderFromFile("Shaders/OIT/particle.geom", Shader::Type::eGeometry);
	Shader OITparticleFrag = Shader::createShaderFromFile("Shaders/OIT/particle.frag", Shader::Type::eFragment);

	shaderOITparticle.attachShader(OITparticleVert);
	shaderOITparticle.attachShader(OITparticleGeom);
	shaderOITparticle.attachShader(OITparticleFrag);
	shaderOITparticle.linkShaderProgram();

	Shader compositePassVert = Shader::createShaderFromFile("Shaders/OIT/OIT_composite.vert", Shader::Type::eVertex);
	Shader compositePassFrag = Shader::createShaderFromFile("Shaders/OIT/OIT_composite.frag", Shader::Type::eFragment);

	shaderOITcomposite.attachShader(compositePassVert);
	shaderOITcomposite.attachShader(compositePassFrag);
	shaderOITcomposite.linkShaderProgram();

	Shader screenVert = Shader::createShaderFromFile("Shaders/OIT/OIT_screen.vert", Shader::Type::eVertex);
	Shader screenFrag = Shader::createShaderFromFile("Shaders/OIT/OIT_screen.frag", Shader::Type::eFragment);

	shaderOITscreen.attachShader(screenVert);
	shaderOITscreen.attachShader(screenFrag);
	shaderOITscreen.linkShaderProgram();

	shaderProgram->useShader();
	setLightUniforms(*shaderProgram);

	shaderGrassProgram->useShader();
	setLightUniforms(*shaderGrassProgram);

	shaderPickupProgram->useShader();
	setLightUniforms(*shaderPickupProgram);

	shaderOITpickup.useShader();
	setLightUniforms(shaderOITpickup);

}

void Maze::initMazeMaterials()
{
	this->material = new Material(glm::vec3(0.25));
}

void Maze::initMazeTextures()
{
	this->wallTexture = new Texture(Texture::createTextureFromFile("res/Textures/wall.bmp", Texture::Type::BMP));
	this->floorTexture = new Texture(Texture::createTextureFromFile("res/Textures/floor.bmp", Texture::Type::BMP));
	this->ceilingTexture = new Texture(Texture::createTextureFromFile("res/Textures/ceiling.bmp", Texture::Type::BMP));
	this->torchTexture = new Texture(Texture::createTextureFromFile("res/Textures/wood.png", Texture::Type::PNG));
	this->grass_1Texture = new Texture(Texture::createTextureFromFile("res/Textures/grass1.png", Texture::Type::PNG));
	this->grass_2Texture = new Texture(Texture::createTextureFromFile("res/Textures/grass2.png", Texture::Type::PNG));
	this->grass_3Texture = new Texture(Texture::createTextureFromFile("res/Textures/grass3.png", Texture::Type::PNG));
	this->spawnActiveTexture = new Texture(Texture::createTextureFromFile("res/Textures/spawnActive.png", Texture::Type::PNG));
	this->spawnInactiveTexture = new Texture(Texture::createTextureFromFile("res/Textures/spawnInactive.png", Texture::Type::PNG));
	this->exitDoorTexture = new Texture(Texture::createTextureFromFile("res/Textures/doors.png", Texture::Type::BMP));

	this->normalMapCeiling = new Texture(Texture::createTextureFromFile("res/Textures/ceiling_nrm.png", Texture::Type::NORMAL_MAP));
	this->normalMapWall = new Texture(Texture::createTextureFromFile("res/Textures/wall_nrm.png", Texture::Type::NORMAL_MAP));
	this->normalMapFloor = new Texture(Texture::createTextureFromFile("res/Textures/floor_nrm.png", Texture::Type::NORMAL_MAP));
	this->normalMapDoors = new Texture(Texture::createTextureFromFile("res/Textures/doors_nrm.png", Texture::Type::NORMAL_MAP));

	this->specularMapWall = new Texture(Texture::createTextureFromFile("res/Textures/wall_specular.png", Texture::Type::SPECULAR));
	this->specularMapCeiling = new Texture(Texture::createTextureFromFile("res/Textures/ceiling_specular.png", Texture::Type::SPECULAR));
	this->specularMapFloor = new Texture(Texture::createTextureFromFile("res/Textures/floor_specular.png", Texture::Type::SPECULAR));
	this->specularMapWood = new Texture(Texture::createTextureFromFile("res/Textures/wood_specular.png", Texture::Type::SPECULAR));
	this->specularDoors = new Texture(Texture::createTextureFromFile("res/Textures/doors_specular.png", Texture::Type::SPECULAR));

	this->enemyTexture = new Texture(Texture::createTextureFromFile("res/Textures/purple.png", Texture::Type::BMP));

	ResourceManager::getInstance().loadTexture("fire", "res/Textures/fire.png", Texture::Type::PNG);
	ResourceManager::getInstance().loadTexture("smoke", "res/Textures/smoke.png", Texture::Type::PNG);
}

void Maze::initObjModels()
{
	std::vector<DataOBJ> cubeObjects = readObj("res/Models/wall.obj");
	std::vector<DataOBJ> planeObjects = readObj("res/Models/plate.obj");
	std::vector<DataOBJ> planeUpObjects = readObj("res/Models/plateUp.obj");
	std::vector<DataOBJ> torchObjects = readObj("res/Models/torch.obj");
	std::vector<DataOBJ> grass_Objects = readObj("res/Models/square.obj");
	std::vector<DataOBJ> spawn_Objects = readObj("res/Models/respawnPickup.obj");
	std::vector<DataOBJ> enemyObjects = readObj("res/Models/ghost.obj");
	std::vector<DataOBJ> exitDoorsObjects = readObj("res/Models/exit.obj");


	int wallInstances = 0;
	int ceilingInstances = 0;
	int floorInstances = 0;
	int torchInstances = 0;
	int grass1Instances = 0;
	int grass2Instances = 0;
	int grass3Instances = 0;
	int exitInstances = 0;

	TransformationOBJ transformation = TransformationOBJ();

	for (int i = 0; i < this->mazeDimensionX; i++)
	{
		for (int j = 0; j < this->mazeDimensionY; j++)
		{
			if (this->mazeIndexData[i][j] == (int)TileType::WALL)
			{
				offsetsWalls.emplace_back(i * 2.f);
				offsetsWalls.emplace_back(0.0f);
				offsetsWalls.emplace_back(j * 2.f);

				wallInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_END_POS) {
				offsetsExitDoors.emplace_back(i * 2.f);
				offsetsExitDoors.emplace_back(0.0f);
				offsetsExitDoors.emplace_back(j * 2.f);

				exitInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_START_POS) {
				this->camera = new Camera(glm::vec3(i * 2.f, -1.0f, j * 2.f));

				this->startPosition = glm::vec3(i * 2.f, -1.0f, j * 2.f);

				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;

				std::vector<GLfloat> offsetRespawn;

				offsetRespawn.emplace_back(i * 2.f);
				offsetRespawn.emplace_back(0.0f);
				offsetRespawn.emplace_back(j * 2.f);

				GameObject* respawn = new GameObject(material, this->spawnActiveTexture, spawn_Objects, transformation, offsetRespawn, 1);

				this->respawnPickup.push_back(new RespawnPoint(respawn, true));

			}
			else if (this->mazeIndexData[i][j] == (int)TileType::EMPTY_SPACE)
			{
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_RESPAWN)
			{
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;

				std::vector<GLfloat> offsetRespawn;

				offsetRespawn.emplace_back(i * 2.f);
				offsetRespawn.emplace_back(0.0f);
				offsetRespawn.emplace_back(j * 2.f);

				GameObject* respawn = new GameObject(material, this->spawnInactiveTexture, spawn_Objects, transformation, offsetRespawn, 1);

				this->respawnPickup.push_back(new RespawnPoint(respawn, false));
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::ENEMY_SPAWN) {
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;

				std::vector<GLfloat> offsetEnemy;

				offsetEnemy.emplace_back(i * 2.f);
				offsetEnemy.emplace_back(-0.5f);
				offsetEnemy.emplace_back(j * 2.f);


				GameObject* enemyGameObject = new GameObject(material, this->enemyTexture, enemyObjects, transformation, offsetEnemy, 1);

				this->opponents.push_back(new Enemy(enemyGameObject));
			}
		}
	}

	this->floors = new GameObject(material, this->floorTexture, planeObjects, transformation, offsetsFloors, floorInstances);
	this->floors->setNormalMapTexture(this->normalMapFloor);
	this->floors->setSpecular(this->specularMapFloor);

	this->walls = new GameObject(material, this->wallTexture, cubeObjects, transformation, offsetsWalls, wallInstances);
	this->walls->setNormalMapTexture(this->normalMapWall);
	this->walls->setSpecular(this->specularMapWall);

	this->exitDoors = new GameObject(material, this->exitDoorTexture, exitDoorsObjects, transformation, offsetsExitDoors, exitInstances);
	this->exitDoors->setNormalMapTexture(this->normalMapDoors);
	this->exitDoors->setSpecular(this->specularDoors);

	this->ceilings = new GameObject(material, this->ceilingTexture, planeUpObjects, transformation, offsetsCeiling, ceilingInstances);
	this->ceilings->setNormalMapTexture(this->normalMapCeiling);
	this->ceilings->setSpecular(this->specularMapCeiling);

	// randomize torhes
	for (int i = 0; i < floorInstances; i++)
	{
		int randomValue = rand() % 10;

		// some chance to spawn torch
		if (randomValue == 0)
		{
			int j = 3 * i;
			float x = offsetsFloors[j];
			float y = offsetsFloors[j + 2];

			int from = -8;
			int to = 8;

			float offsetX = (from + rand() % (to - from + 1)) / 10.0f;
			offsetsTorches.emplace_back(x + offsetX);
			offsetsTorches.emplace_back(-2.0f);

			float offsetY = (from + rand() % (to - from + 1)) / 10.0f;
			offsetsTorches.emplace_back(y + offsetY);

			torchInstances++;

			glm::vec3 torchPos = transformation.objectPosition + glm::vec3{ x + offsetX, -1.05f, y + offsetY };
			this->pointLights.push_back(Light::Point(torchPos, { 0.6f, 0.5f, 0.5f }));
			this->torchesParticleEmitters.emplace_back(torchPos, glm::vec3(0.0f, 0.02f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.0f, 0.1f), ResourceManager::getInstance().getTexture("fire"), glm::vec3(0.2f, 0.4f, 1.0f));
		}
	}

	this->torches = new GameObject(material, this->torchTexture, torchObjects, transformation, offsetsTorches, torchInstances);
	this->torches->setSpecular(this->specularMapWood);

	// randomize grass
	for (int i = 0; i < floorInstances; i++)
	{

		int j = 3 * i;
		float x = offsetsFloors[j];
		float y = offsetsFloors[j + 2];

		int from = -8;
		int to = 8;

		float offsetX = (from + rand() % (to - from + 1)) / 10.0f;
		float offsetY = (from + rand() % (to - from + 1)) / 10.0f;

		int randomValue = rand() % 3;

		if (randomValue == 0)
		{
			offsetsGrass1.emplace_back(x + offsetX);
			offsetsGrass1.emplace_back(-1.7f);
			offsetsGrass1.emplace_back(y + offsetY);

			grass1Instances++;
		}
		else if (randomValue == 1)
		{
			offsetsGrass2.emplace_back(x + offsetX);
			offsetsGrass2.emplace_back(-1.7f);
			offsetsGrass2.emplace_back(y + offsetY);

			grass2Instances++;
		}
		else
		{
			offsetsGrass3.emplace_back(x + offsetX);
			offsetsGrass3.emplace_back(-1.7f);
			offsetsGrass3.emplace_back(y + offsetY);

			grass3Instances++;
		}
	}

	this->grass1 = new GameObject(material, this->grass_1Texture, grass_Objects, transformation, offsetsGrass1, grass1Instances);
	this->grass2 = new GameObject(material, this->grass_2Texture, grass_Objects, transformation, offsetsGrass2, grass2Instances);
	this->grass3 = new GameObject(material, this->grass_3Texture, grass_Objects, transformation, offsetsGrass3, grass3Instances);

	float quadVertices[] = {
		// positions				// uv
		-1.0f, -1.0f, 0.0f,			0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,			1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,			1.0f, 1.0f,

		 1.0f,  1.0f, 0.0f,			1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,			0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,			0.0f, 0.0f
	};

	quadVAO.bind();
	quadVBO.bind();
	quadVBO.bufferData(quadVertices, sizeof(quadVertices));
	quadVBO.setAttributesPointers(0, 3, GL_FLOAT, 5 * sizeof(float), nullptr);
	quadVBO.setAttributesPointers(1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	quadVAO.unbind();
}

void Maze::drawMaze(float deltaTime, bool wireframe)
{
	//this->defaultRender(deltaTime);
	this->OIT_render(deltaTime, wireframe);

}

void Maze::OIT_render(float deltaTime, bool wireframe)
{
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	this->OIT_solidPass(deltaTime);
	this->OIT_transparentPass(deltaTime);
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	this->OIT_compositePass(deltaTime);
	this->OIT_finalPass(deltaTime);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Maze::OIT_solidPass(float deltaTime)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	oit.opaque.bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	this->shaderProgram->useShader();
	this->camera->updateCameraEulerAng();
	this->camera->setCameraUniforms(this->shaderProgram);

	this->walls->draw(this->shaderProgram);
	this->exitDoors->draw(this->shaderProgram);
	this->floors->draw(this->shaderProgram);
	this->ceilings->draw(this->shaderProgram);
	this->torches->draw(this->shaderProgram);

	this->shaderGrassProgram->useShader();
	this->camera->setCameraUniforms(this->shaderGrassProgram);

	this->grass1->draw(this->shaderGrassProgram);
	this->grass2->draw(this->shaderGrassProgram);
	this->grass3->draw(this->shaderGrassProgram);
}

void Maze::OIT_transparentPass(float deltaTime)
{
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunci(0, GL_ONE, GL_ONE);
	glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
	glBlendEquation(GL_FUNC_ADD);

	oit.transparent.bind();

	const GLfloat zeroFillerVec[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
	const GLfloat oneFillerVec[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);

	shaderOITparticle.useShader();
	this->camera->setCameraUniforms(&shaderOITparticle);
	for (auto& emitter : torchesParticleEmitters)
	{
		if (emitter.isActive())
		{
			emitter.render(shaderOITparticle, this->camera->getCameraPosition());
		}
	}

	for (auto& smokeBomb : smokeBombs)
	{
		smokeBomb.render(deltaTime, shaderOITparticle, this->camera->getCameraPosition());
	}

	this->shaderOITpickup.useShader();
	this->camera->setCameraUniforms(&this->shaderOITpickup);

	for (auto p : this->respawnPickup)
	{
		p->drawRespawnPoint(&this->shaderOITpickup);
	}

	for (auto p : this->opponents)
	{
		p->drawEnemy(&this->shaderOITpickup);
	}
}

void Maze::OIT_compositePass(float deltaTime)
{
	glDepthFunc(GL_ALWAYS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	oit.opaque.bind();

	shaderOITcomposite.useShader();

	oit.accumTex.bindTexture(0);
	oit.revealTex.bindTexture(1);
	quadVAO.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Maze::OIT_finalPass(float deltaTime)
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	shaderOITscreen.useShader();
	oit.opaqueTex.bindTexture(0);
	quadVAO.bind();
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Maze::setLightUniforms(ShaderProgram& shader)
{
	shader.setInt("pointLightsCount", static_cast<int>(pointLights.size()));
	char lightIndex[20];
	for (int i = 0; i < pointLights.size(); ++i)
	{
		sprintf_s(lightIndex, 20, "pointLights[%d].", i);
		std::string index{ lightIndex };
		shader.setVec3f(index + "position", pointLights[i].getPosition());
		shader.setVec3f(index + "diffuse", pointLights[i].getDiffuse());
		shader.setVec3f(index + "specular", pointLights[i].getSpecular());

		shader.setFloat(index + "constant", pointLights[i].getAttenuation().getConstant());
		shader.setFloat(index + "linear", pointLights[i].getAttenuation().getLinear());
		shader.setFloat(index + "quadratic", pointLights[i].getAttenuation().getQuadratic());
	}
}

void Maze::defaultRender(float deltaTime)
{
	this->shaderProgram->useShader();
	this->camera->updateCameraEulerAng();
	this->camera->setCameraUniforms(this->shaderProgram);

	this->walls->draw(this->shaderProgram);
	this->exitDoors->draw(this->shaderProgram);
	this->floors->draw(this->shaderProgram);
	this->ceilings->draw(this->shaderProgram);
	this->torches->draw(this->shaderProgram);


	this->shaderGrassProgram->useShader();
	this->camera->setCameraUniforms(this->shaderGrassProgram);

	this->grass1->draw(this->shaderGrassProgram);
	this->grass2->draw(this->shaderGrassProgram);
	this->grass3->draw(this->shaderGrassProgram);

	this->shaderPickupProgram->useShader();
	this->camera->setCameraUniforms(this->shaderPickupProgram);

	for (auto p : this->respawnPickup)
	{
		p->drawRespawnPoint(this->shaderPickupProgram);
	}

	for (auto p : this->opponents)
	{
		p->drawEnemy(this->shaderPickupProgram);
	}
	shaderParticles.useShader();
	this->camera->setCameraUniforms(&shaderParticles);
	for (auto& emitter : torchesParticleEmitters)
	{
		if (emitter.isActive())
		{
			emitter.render(shaderParticles, this->camera->getCameraPosition());
		}
	}

	for (auto& smokeBomb : smokeBombs)
	{
		smokeBomb.render(deltaTime, shaderParticles, this->camera->getCameraPosition());
	}
}

void Maze::deferred_geometryPass(float deltaTime)
{
	deferred.gBuffer.bind();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderGeometryPass.useShader();

	this->camera->updateCameraEulerAng();
	this->camera->setCameraUniforms(&shaderGeometryPass);

	this->walls->draw(&shaderGeometryPass);
	this->exitDoors->draw(&shaderGeometryPass);
	this->floors->draw(&shaderGeometryPass);
	this->ceilings->draw(&shaderGeometryPass);
	this->torches->draw(&shaderGeometryPass);

	deferred.gBuffer.unbind();
}

void Maze::deferred_lightingPass(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shaderLightingPass.useShader();

	for (uint32_t i = 0; i < deferred.attachmentsCount; ++i)
	{
		deferred.attachments[i].bindTexture(i);
	}
	setLightUniforms(shaderLightingPass);
	this->camera->setCameraUniforms(&shaderLightingPass);

	deferred.gBuffer.bindRead();
	deferred.gBuffer.unbindDraw();
	deferred.gBuffer.blit(0, 0, Config::g_defaultWidth, Config::g_defaultHeight, 0, 0, Config::g_defaultWidth, Config::g_defaultHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	deferred.gBuffer.unbind();
}

void Maze::updateMaze(float deltaTime)
{
	for (auto& emitter : torchesParticleEmitters)
	{
		if (glm::distance(emitter.getPosition(), camera->getCameraPosition()) < 30.0f)
		{
			emitter.setActive(true);
			emitter.update(deltaTime);
		}
		else
		{
			emitter.setActive(false);
		}
	}

	for (auto iter = smokeBombs.begin(); iter != smokeBombs.end(); ++iter)
	{
		iter->update(deltaTime);
	}

	smokeBombs.erase(
		std::remove_if(
			smokeBombs.begin(),
			smokeBombs.end(),
			[&](const SmokeBomb& bomb) -> bool
			{
				return bomb.getDurationTime() > bomb.getMaxDurationTime();
			}),
		smokeBombs.end());

	if (smokeBombCooldownLeft > 0.0f)
	{
		smokeBombCooldownLeft -= deltaTime;
	}

	this->updateEnemy(deltaTime);

	this->updateRespawnPoint(deltaTime);
}

void Maze::updateRespawnPoint(float deltaTime) {
	glm::vec3 playerPosition = this->camera->getCameraPosition();
	int respawnIterator = -1;
	int respawnActive = -1;
	for (auto p : this->respawnPickup)
	{
		respawnIterator++;
		p->updateRespawnPoint(deltaTime);

		glm::vec3 respawnPointPos = p->getRespawnPointPosition();

		// kolizja
		if (respawnPointPos.x - 1.5f < playerPosition.x &&
			respawnPointPos.x + 1.5f > playerPosition.x &&
			respawnPointPos.z - 1.5f < playerPosition.z &&
			respawnPointPos.z + 1.5f > playerPosition.z) {

			respawnActive = respawnIterator;
		}
	}

	if (respawnActive != -1) {
		for (auto p : this->respawnPickup)
		{
			p->setRespawnActivation(false);
			p->setRespawnPointTexture(this->spawnInactiveTexture);
		}

		this->respawnPickup[respawnActive]->setRespawnActivation(true);
		this->respawnPickup[respawnActive]->setRespawnPointTexture(this->spawnActiveTexture);
		glm::vec3 newRespawnPos = this->respawnPickup[respawnActive]->getRespawnPointPosition();
		this->startPosition = glm::vec3(newRespawnPos.x, this->startPosition.y, newRespawnPos.z);
	}
}

bool Maze::isValidPath(int row, int col)
{
	bool val = (row >= 0) && (row < this->mazeDimensionX) && (col >= 0) && (col < this->mazeDimensionY);

	return val;
}

bool Maze::isUnBlockedPath(int row, int col)
{
	if (this->mazeIndexData[row][col] == (int)TileType::WALL) {
		return false;
	}
	else {
		return true;
	}
}

bool Maze::isDestinationPath(int row, int col, Pair dest)
{
	if (row == dest.first && col == dest.second)
	{
		return true;
	}
	else {
		return false;
	}
}

double Maze::calculateHValuePath(int row, int col, Pair dest)
{
	return ((double)sqrt(
		(row - dest.first) * (row - dest.first)
		+ (col - dest.second) * (col - dest.second)));
}

PairFloat Maze::tracePath(cell cellDetails[][30], Pair dest)
{
	int row = dest.first;
	int col = dest.second;

	int maxIterations = 50;
	int i = 0;

	std::vector<Pair> Path;

	while (!(cellDetails[row][col].parent_i == row
		&& cellDetails[row][col].parent_j == col)) {
		Path.push_back(std::make_pair(row, col));
		int temp_row = cellDetails[row][col].parent_i;
		int temp_col = cellDetails[row][col].parent_j;
		row = temp_row;
		col = temp_col;

		i++;

		if (i > maxIterations) {
			break;
		}
	}

	if (Path.size() < 1) {
		return PairFloat(0.0f, 0.0f);
	}

	Path.push_back(std::make_pair(row, col));

	std::pair<int, int> origin = Path[Path.size() - 1];

	std::pair<int, int> nextPath = Path[Path.size() - 2];

	float enemyDirectionX = 0.0f;
	float enemyDirectionZ = 0.0f;

	if (nextPath.first > origin.first) {
		enemyDirectionX = 1.2f;
	}
	else if (nextPath.first < origin.first) {
		enemyDirectionX = -1.2f;
	}
	else if (nextPath.second > origin.second) {
		enemyDirectionZ = 1.2f;
	}
	else if (nextPath.second < origin.second) {
		enemyDirectionZ = -1.2f;
	}

	return PairFloat(enemyDirectionX, enemyDirectionZ);
}

PairFloat Maze::aStarSearchPath(Pair src, Pair dest)
{
	if (this->isValidPath(src.first, src.second) == false) {
		return PairFloat(0, 0);
	}

	if (isValidPath(dest.first, dest.second) == false) {
		return PairFloat(0, 0);
	}

	if (isUnBlockedPath(src.first, src.second) == false
		|| this->isUnBlockedPath(dest.first, dest.second)
		== false) {
		return PairFloat(0, 0);
	}

	bool closedList[30][30];
	memset(closedList, false, sizeof(closedList));

	cell cellDetails[30][30];

	int i, j;

	for (i = 0; i < this->mazeDimensionX; i++) {
		for (j = 0; j < this->mazeDimensionY; j++) {
			cellDetails[i][j].f = FLT_MAX;
			cellDetails[i][j].g = FLT_MAX;
			cellDetails[i][j].h = FLT_MAX;
			cellDetails[i][j].parent_i = -1;
			cellDetails[i][j].parent_j = -1;
		}
	}

	i = src.first, j = src.second;

	cellDetails[i][j].parent_i = i;
	cellDetails[i][j].parent_j = j;
	cellDetails[i][j].f = 0.0;
	cellDetails[i][j].g = 0.0;
	cellDetails[i][j].h = 0.0;

	std::set<pPair> openList;

	openList.insert(std::make_pair(0.0, std::make_pair(i, j)));

	while (!openList.empty()) {
		pPair p = *openList.begin();

		openList.erase(openList.begin());

		i = p.second.first;
		j = p.second.second;
		closedList[i][j] = true;

		double gNew;
		double hNew;
		double fNew;

		if (this->isValidPath(i - 1, j)) {
			if (this->isDestinationPath(i - 1, j, dest)) {
				cellDetails[i - 1][j].parent_i = i;
				cellDetails[i - 1][j].parent_j = j;
				return tracePath(cellDetails, dest);
			}
			else if (closedList[i - 1][j] == false
				&& this->isUnBlockedPath(i - 1, j)
				== true) {
				gNew = cellDetails[i][j].g + 1.0;
				hNew = this->calculateHValuePath(i - 1, j, dest);
				fNew = gNew + hNew;
				if (cellDetails[i - 1][j].f == FLT_MAX || cellDetails[i - 1][j].f > fNew) {
					openList.insert(std::make_pair(fNew, std::make_pair(i - 1, j)));

					cellDetails[i - 1][j].parent_i = i;
					cellDetails[i - 1][j].parent_j = j;

					cellDetails[i - 1][j].f = fNew;
					cellDetails[i - 1][j].g = gNew;
					cellDetails[i - 1][j].h = hNew;
				}
			}
		}

		if (this->isValidPath(i + 1, j)) {
			if (this->isDestinationPath(i + 1, j, dest) == true) {
				cellDetails[i + 1][j].parent_i = i;
				cellDetails[i + 1][j].parent_j = j;
				return tracePath(cellDetails, dest);
			}
			else if (closedList[i + 1][j] == false && this->isUnBlockedPath(i + 1, j) == true) {
				gNew = cellDetails[i][j].g + 1.0; hNew = this->calculateHValuePath(i + 1, j, dest); fNew = gNew + hNew;
				if (cellDetails[i + 1][j].f == FLT_MAX
					|| cellDetails[i + 1][j].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i + 1, j)));

					cellDetails[i + 1][j].parent_i = i;
					cellDetails[i + 1][j].parent_j = j;

					cellDetails[i + 1][j].f = fNew;
					cellDetails[i + 1][j].g = gNew;
					cellDetails[i + 1][j].h = hNew;
				}
			}
		}

		if (this->isValidPath(i, j + 1) == true) {
			if (this->isDestinationPath(i, j + 1, dest)) {
				cellDetails[i][j + 1].parent_i = i;
				cellDetails[i][j + 1].parent_j = j;
				return tracePath(cellDetails, dest);
			}
			else if (closedList[i][j + 1] == false == true) {
				gNew = cellDetails[i][j].g + 1.0; hNew = this->calculateHValuePath(i, j + 1, dest); fNew = gNew + hNew;
				if (cellDetails[i][j + 1].f == FLT_MAX
					|| cellDetails[i][j + 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j + 1)));


					cellDetails[i][j + 1].parent_i = i;
					cellDetails[i][j + 1].parent_j = j;

					cellDetails[i][j + 1].f = fNew;
					cellDetails[i][j + 1].g = gNew;
					cellDetails[i][j + 1].h = hNew;
				}
			}
		}
		if (this->isValidPath(i, j - 1) == true) {
			if (this->isDestinationPath(i, j - 1, dest) == true) {

				cellDetails[i][j - 1].parent_i = i;
				cellDetails[i][j - 1].parent_j = j;
				return tracePath(cellDetails, dest);
			}
			else if (closedList[i][j - 1] == false
				&& this->isUnBlockedPath(i, j - 1) == true) {
				gNew = cellDetails[i][j].g + 1.0; hNew = this->calculateHValuePath(i, j - 1, dest); fNew = gNew + hNew;
				if (cellDetails[i][j - 1].f == FLT_MAX
					|| cellDetails[i][j - 1].f > fNew) {
					openList.insert(std::make_pair(
						fNew, std::make_pair(i, j - 1)));

					cellDetails[i][j - 1].parent_i = i;
					cellDetails[i][j - 1].parent_j = j;

					cellDetails[i][j - 1].f = fNew;
					cellDetails[i][j - 1].g = gNew;
					cellDetails[i][j - 1].h = hNew;
				}
			}
		}
	}

	return PairFloat(0, 0);
}

Pair Maze::positionToXYPath(PairFloat position)
{
	return Pair(int((position.first + 1.0f) / 2.0f), int((position.second + 1.0f) / 2.0f));
}

void Maze::updateEnemy(float deltaTime) {
	// przeciwnik
	glm::vec3 playerPosition = this->camera->getCameraPosition();
	bool collisionWithPlayer = false;
	for (auto p : this->opponents)
	{
		p->updateEnemy(deltaTime);
		glm::vec3 enemyPosition = p->getEnemyPosition();

		// kolizja
		if (enemyPosition.x - 1.25f < playerPosition.x &&
			enemyPosition.x + 1.25f > playerPosition.x &&
			enemyPosition.z - 1.25f < playerPosition.z &&
			enemyPosition.z + 1.25f > playerPosition.z) {

			this->camera->setCameraPosition(this->startPosition);
			collisionWithPlayer = true;
		}

		bool isInSmoke = false;
		for (auto& smokeBomb : smokeBombs)
		{
			if (glm::distance(enemyPosition, smokeBomb.getPosition()) < 3.8f)
			{
				isInSmoke = true;
				break;
			}
		}

		if (isInSmoke)
		{
			continue;
		}

		// id� za graczem gdy znajdzie si� w zasi�gu
		bool followPlayer = false;
		if (enemyPosition.x - 10.0f < playerPosition.x &&
			enemyPosition.x + 10.0f > playerPosition.x &&
			enemyPosition.z - 10.0f < playerPosition.z &&
			enemyPosition.z + 10.0f > playerPosition.z) {

			followPlayer = true;
		}

		if (followPlayer == true)
		{
			Pair enemyPosXY = this->positionToXYPath(PairFloat(enemyPosition.x, enemyPosition.z));
			Pair playerPosXY = this->positionToXYPath(PairFloat(playerPosition.x, playerPosition.z));

			//	std::cout << "PLAYER: " << playerPosXY.first << " " << playerPosXY.second << " ENEMY: " << enemyPosXY.first << " " << enemyPosXY.second << std::endl;

			glm::vec3 enemyTransforationPos = p->getEnemyPositionWithoutOffset();

			PairFloat enemyDirection = this->aStarSearchPath(enemyPosXY, playerPosXY);
			p->setLastEnemyDirectionX(enemyDirection.first);
			p->setLastEnemyDirectionY(enemyDirection.second);

			float addToX = deltaTime * p->getLastEnemyDirectionX();
			float addToZ = deltaTime * p->getLastEnemyDirectionY();

			p->setEnemyPosition(glm::vec3(enemyTransforationPos.x + addToX, enemyTransforationPos.y, enemyTransforationPos.z + addToZ));
		}
	}

	if (collisionWithPlayer == true) {
		for (auto p : this->opponents)
		{
			p->resetEnemyPosition();
		}
	}
}

bool Maze::willBeCollisionWithWall(float deltaTime)
{
	glm::vec3 playerPosition = this->camera->getFutureCameraPosition();
	bool isCollision = false;

	for (int i = 0; i < this->offsetsWalls.size(); i += 3)
	{
		float x = this->offsetsWalls[i];
		float z = this->offsetsWalls[i + 2];

		if (x - 1.65f < playerPosition.x &&
			x + 1.65f > playerPosition.x &&
			z - 1.65f < playerPosition.z &&
			z + 1.65f > playerPosition.z) {

			isCollision = true;
		}
	}

	return isCollision;
}

bool Maze::willBeCollisionWithExit() {
	glm::vec3 playerPosition = this->camera->getFutureCameraPosition();
	bool isCollision = false;

	for (int i = 0; i < this->offsetsExitDoors.size(); i += 3)
	{
		float x = this->offsetsExitDoors[i];
		float z = this->offsetsExitDoors[i + 2];

		if (x - 1.5f < playerPosition.x &&
			x + 1.5f > playerPosition.x &&
			z - 1.5f < playerPosition.z &&
			z + 1.5f > playerPosition.z) {

			isCollision = true;
		}
	}

	return isCollision;
}

void Maze::useSmokeBomb()
{
	if (smokeBombCooldownLeft > 0.0f)
	{
		return;
	}

	smokeBombCooldownLeft = smokeBombCooldown;

	const auto cameraPos = camera->getCameraPosition();
	glm::vec3 pos = { cameraPos.x, -1.0f, cameraPos.z };

	smokeBombs.emplace_back(pos);
}

void Maze::updateSmokeBombs(float deltaTime)
{
	if (smokeBombCooldownLeft > 0.0f)
	{
		smokeBombCooldownLeft -= deltaTime;
	}

	for (auto iter = smokeBombs.begin(); iter != smokeBombs.end(); ++iter)
	{
		iter->update(deltaTime);
		if (iter->getDurationTime() >= iter->getMaxDurationTime())
		{
			iter = smokeBombs.erase(iter);
		}
	}
}

Maze::~Maze()
{
	if (this->mazeIndexData != nullptr)
	{
		for (int i = 0; i < this->mazeDimensionX; i++)
		{
			delete[] this->mazeIndexData[i];
		}
		delete[] this->mazeIndexData;
	}

	for (auto p : this->respawnPickup)
	{
		delete p;
	}
	this->respawnPickup.clear();

	for (auto p : this->opponents)
	{
		delete p;
	}
	this->opponents.clear();

	delete this->walls;

	delete this->exitDoors;

	delete this->ceilings;

	delete this->ceilings;

	delete this->torches;

	delete this->grass1;

	delete this->grass2;

	delete this->grass3;

	delete this->camera;

	delete this->shaderProgram;

	delete this->shaderGrassProgram;

	delete this->shaderPickupProgram;

	delete this->material;

	delete this->wallTexture;

	delete this->torchTexture;

	delete this->spawnInactiveTexture;

	delete this->spawnActiveTexture;

	delete this->enemyTexture;
}