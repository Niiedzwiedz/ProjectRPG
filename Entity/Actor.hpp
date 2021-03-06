#pragma once
#include <queue>
#include <map>
#include "Types.hpp"
#include "World/Tile.hpp"

/*
 *      WorldEntity - klasa reprezentująca obiekt aktywny w świecie gry
 *  Przez obiekt aktywny rozumie się te elementy świata, których pozycja ulega częstym zmianom
 *  Dostępne funkcje wirtualne, dzięki którym (ewentualnie) będzie można implementować
 *  zaawansowane interakcje ze światem gry.
 *  entityType wskazuje na typ obiektu
 */

class Actor {
protected:
	unsigned entityType;
	Vec2u worldPosition;
	Vec2f spritePosition;
	unsigned movementSpeed;
	Direction facing;
	bool isMoving;
	unsigned frameCounter;

	std::queue<Direction> movementQueue;
	std::map<std::string, int> statistics;
	void enqueueMove(Direction dir);
public:
	Actor(unsigned type, unsigned moveSpeed)
	: entityType(type), movementSpeed(moveSpeed), facing(Direction::Down), isMoving(false), frameCounter(0) { }

	Actor(unsigned type, unsigned moveSpeed, Vec2u worldPos)
	: Actor(type, moveSpeed)
	{
		worldPosition = worldPos;
		spritePosition = Vec2f(worldPos * Tile::dimensions());
	}

	static Direction flipDirection(Direction);

	Vec2u getWorldPosition()  const { return worldPosition; }
	Vec2f getSpritePosition() const { return spritePosition; }
	Direction getDirection()  const { return facing; }
	unsigned getMoveSpeed()   const { return movementSpeed; }
	std::map<std::string, int>& getStatistics() { return statistics; }

	void setFacing(Direction dir) { facing = dir; }

	void move(Direction dir);
	void update();

	bool wantsToMove() const;
	Direction popMovement();

	virtual void onMove(Direction dir) = 0;
	virtual void onUpdate() = 0;

	virtual void onInteract(Direction dir) = 0;
	virtual void onStep() = 0;
};