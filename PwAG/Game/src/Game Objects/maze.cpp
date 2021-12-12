#include "pch.h"
#include "maze.h"
#include "../Rendering system/Model/objReader.h"
#include "tileType.h"


Maze::Maze()
{
	this->initMaze();
}

void Maze::initMaze()
{
	std::string mazeDataFileName = "Maps/map_1.txt";

	std::ifstream mazefile;
	mazefile.open(mazeDataFileName);

	if(!mazefile.is_open())
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

	for(int i = 0; i < this->mazeDimensionX; i++)
	{
		this->mazeIndexData[i] = new int[this->mazeDimensionY];
	}


	for(int j = 0; !mazefile.eof(); j++)
	{
		std::getline(mazefile, singleline);

		std::stringstream maze_line(singleline);
		for(int i = 0; i < this->mazeDimensionY; i++)
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

	shaderProgram->useShader();
	setLightUniforms(*shaderProgram);

	shaderGrassProgram->useShader();
	setLightUniforms(*shaderGrassProgram);

	shaderPickupProgram->useShader();
	setLightUniforms(*shaderPickupProgram);

}

void Maze::initMazeMaterials()
{
	this->material = new Material(glm::vec3(0.25));
}

void Maze::initMazeTextures()
{
	this->wallTexture = new Texture("res/Textures/wall.bmp", TextureType::BMP);
	this->floorTexture = new Texture("res/Textures/floor.bmp", TextureType::BMP);
	this->ceilingTexture = new Texture("res/Textures/ceiling.bmp", TextureType::BMP);
	this->torchTexture = new Texture("res/Textures/wood.png", TextureType::PNG);
	this->grass_1Texture = new Texture("res/Textures/grass1.png", TextureType::PNG);
	this->grass_2Texture = new Texture("res/Textures/grass2.png", TextureType::PNG);
	this->grass_3Texture = new Texture("res/Textures/grass3.png", TextureType::PNG);
	this->spawnActiveTexture = new Texture("res/Textures/spawnActive.png", TextureType::PNG);
	this->spawnInactiveTexture = new Texture("res/Textures/spawnInactive.png", TextureType::PNG);

	this->normalMapCeiling = new Texture("res/Textures/ceiling_nrm.png", TextureType::NORMAL_MAP);
	this->normalMapWall = new Texture("res/Textures/wall_nrm.png", TextureType::NORMAL_MAP);
	this->normalMapFloor = new Texture("res/Textures/floor_nrm.png", TextureType::NORMAL_MAP);

	this->enemyTexture = new Texture("res/Textures/purple.png", TextureType::BMP);

	ResourceManager::getInstance().loadTexture("fire", "res/Textures/fire.png", TextureType::PNG);
	ResourceManager::getInstance().loadTexture("smoke", "res/Textures/smoke.png", TextureType::PNG);
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


	int wallInstances = 0;
	int ceilingInstances = 0;
	int floorInstances = 0;
	int torchInstances = 0;
	int grass1Instances = 0;
	int grass2Instances = 0;
	int grass3Instances = 0;

	TransformationOBJ transformation = TransformationOBJ();

	for(int i = 0; i < this->mazeDimensionX; i++)
	{
		for(int j = 0; j < this->mazeDimensionY; j++)
		{
			if(this->mazeIndexData[i][j] == (int)TileType::WALL)
			{
				offsetsWalls.emplace_back(i * 2.f);
				offsetsWalls.emplace_back(0.0f);
				offsetsWalls.emplace_back(j * 2.f);

				wallInstances++;
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
			else if(this->mazeIndexData[i][j] == (int)TileType::EMPTY_SPACE)
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
			else if(this->mazeIndexData[i][j] == (int)TileType::PLAYER_RESPAWN)
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

	this->walls = new GameObject(material, this->wallTexture, cubeObjects, transformation, offsetsWalls, wallInstances);
	this->walls->setNormalMapTexture(this->normalMapWall);

	this->ceilings = new GameObject(material, this->ceilingTexture, planeUpObjects, transformation, offsetsCeiling, ceilingInstances);
	this->ceilings->setNormalMapTexture(this->normalMapCeiling);
	


	// randomize torhes
	for(int i = 0; i < floorInstances; i++)
	{
		int randomValue = rand() % 10;

		// some chance to spawn torch
		if(randomValue == 0)
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
			this->torchesParticleEmitters.emplace_back(torchPos);
		}
	}

	this->torches = new GameObject(material, this->torchTexture, torchObjects, transformation, offsetsTorches, torchInstances);

	// randomize grass
	for(int i = 0; i < floorInstances; i++)
	{

		int j = 3 * i;
		float x = offsetsFloors[j];
		float y = offsetsFloors[j + 2];

		int from = -8;
		int to = 8;

		float offsetX = (from + rand() % (to - from + 1)) / 10.0f;
		float offsetY = (from + rand() % (to - from + 1)) / 10.0f;

		int randomValue = rand() % 3;

		if(randomValue == 0)
		{
			offsetsGrass1.emplace_back(x + offsetX);
			offsetsGrass1.emplace_back(-1.7f);
			offsetsGrass1.emplace_back(y + offsetY);

			grass1Instances++;
		}
		else if(randomValue == 1)
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
}

void Maze::drawMaze(float deltaTime)
{
	this->shaderProgram->useShader();
	this->camera->updateEulerAngels();
	this->camera->setCameraUniforms(this->shaderProgram);

	this->walls->draw(this->shaderProgram);
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

	for(auto p : this->respawnPickup)
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
		if(emitter.isActive())
		{
			emitter.render(shaderParticles, this->camera->getCameraPosition());
		}
	}

	for(auto& smokeBomb : smokeBombs)
	{
		smokeBomb.render(deltaTime, shaderParticles, this->camera->getCameraPosition());
	}
}

void Maze::updateMaze(float deltaTime)
{
	for (auto& emitter : torchesParticleEmitters)
	{
		if(glm::distance(emitter.getPosition(), camera->getCameraPosition()) < 15.0f)
		{
			emitter.setActive(true);
			emitter.update(deltaTime);
		}
		else
		{
			emitter.setActive(false);
		}
	}

	for(auto iter = smokeBombs.begin(); iter != smokeBombs.end(); ++iter)
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

	if(smokeBombCooldownLeft > 0.0f)
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

void Maze::updateEnemy(float deltaTime) {
	// przeciwnik
	glm::vec3 playerPosition = this->camera->getCameraPosition();
	bool collisionWithPlayer = false;
	for (auto p : this->opponents)
	{
		p->updateEnemy(deltaTime);
		glm::vec3 enemyPosition = p->getEnemyPosition();

		// kolizja
		if (enemyPosition.x - 1.0f < playerPosition.x &&
			enemyPosition.x + 1.0f > playerPosition.x &&
			enemyPosition.z - 1.0f < playerPosition.z &&
			enemyPosition.z + 1.0f > playerPosition.z) {

			this->camera->setCameraPosition(this->startPosition);
			collisionWithPlayer = true;
		}

		// id� za graczem gdy znajdzie si� w zasi�gu
		bool followPlayer = false;
		if (enemyPosition.x - 25.0f < playerPosition.x &&
			enemyPosition.x + 25.0f > playerPosition.x &&
			enemyPosition.z - 25.0f < playerPosition.z &&
			enemyPosition.z + 25.0f > playerPosition.z) {

			followPlayer = true;
		}

		if (followPlayer == true)
		{
			float enemyDirectionX = 0.0f;
			float enemyDirectionZ = 0.0f;

			if (playerPosition.x > enemyPosition.x + 0.35f) {
				enemyDirectionX = deltaTime * 0.8f;
			}
			else if (playerPosition.x < enemyPosition.x - 0.35f)
			{
				enemyDirectionX = -deltaTime * 0.8f;
			}

			if (playerPosition.z > enemyPosition.z + 0.35f) {
				enemyDirectionZ = deltaTime * 0.8f;
			}
			else if (playerPosition.z < enemyPosition.z - 0.35f) {
				enemyDirectionZ = -deltaTime * 0.8f;
			}

			glm::vec3 enemyTransforationPos = p->getEnemyPositionWithoutOffset();

			p->setEnemyPosition(glm::vec3(enemyTransforationPos.x + enemyDirectionX, enemyTransforationPos.y, enemyTransforationPos.z + enemyDirectionZ));
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

	for(int i = 0; i < this->offsetsWalls.size(); i += 3)
	{
		float x = this->offsetsWalls[i];
		float z = this->offsetsWalls[i + 2];

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
	if(smokeBombCooldownLeft > 0.0f)
	{
		return;
	}

	smokeBombCooldownLeft = smokeBombCooldown;

	const auto cameraPos = camera->getCameraPosition();
	glm::vec3 pos = { cameraPos.x, 0.0f, cameraPos.z };

	smokeBombs.emplace_back(pos);
}

Maze::~Maze()
{
	if(this->mazeIndexData != nullptr)
	{
		for(int i = 0; i < this->mazeDimensionX; i++)
		{
			delete[] this->mazeIndexData[i];
		}
		delete[] this->mazeIndexData;
	}

	for(auto p : this->respawnPickup)
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

void Maze::setLightUniforms(ShaderProgram& shader)
{
	shader.setInt("pointLightsCount", static_cast<int>(pointLights.size()));
	char lightIndex[20];
	for (int i = 0; i < pointLights.size(); ++i)
	{
		sprintf_s(lightIndex, 20, "pointLights[%d].", i);
		std::string index{ lightIndex };
		shader->setVec3f(index + "position", pointLights[i].getPosition());

		shader.setFloat(index + "constant", pointLights[i].getAttenuation().getConstant());
		shader.setFloat(index + "linear", pointLights[i].getAttenuation().getLinear());
		shader.setFloat(index + "quadratic", pointLights[i].getAttenuation().getQuadratic());
	}
}

void Maze::updateSmokeBombs(float deltaTime)
{
	if(smokeBombCooldownLeft > 0.0f)
	{
		smokeBombCooldownLeft -= deltaTime;
	}

	for(auto iter = smokeBombs.begin(); iter != smokeBombs.end(); ++iter)
	{
		iter->update(deltaTime);
		if(iter->getDurationTime() >= iter->getMaxDurationTime())
		{
			iter = smokeBombs.erase(iter);
		}
	}
}
