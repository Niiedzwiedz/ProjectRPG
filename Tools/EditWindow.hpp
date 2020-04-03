#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "World/Map.hpp"
#include "Graphics/TextureManager.hpp"
#include "Tool.hpp"
#include "Brush.hpp"
#include "TilePicker.hpp"
#include "CursorTool.hpp"
#include "NPCCreator.hpp"

class EditWindow {
	unsigned width, height;

	std::shared_ptr<sf::RenderWindow> editorWindow;

	struct {
		bool isLoaded = false;
		int width, height;
		std::string fname;
		Map mapData;
	} EditingMap;

	struct {
		bool isMoving = false;
		Vec2i mouseStart;
		Vec2i hoverCoordinates;
		bool leftClick = false;
	} MouseMovement;

	struct {
		std::shared_ptr<Brush> brush;
		std::shared_ptr<CursorTool> cursor;
		std::shared_ptr<NPCCreator> creator;
	} Tools;

	TextureManager textureManager;

	std::shared_ptr<Tool> currentTool;

	TilePicker picker;

	Vec2i mapPosition;

	bool drawCommonWindows();
	void drawMenuBar();
	void leftClickHandler();
	void frameLoop();
	void eventPoll();
public:
	void start();
};