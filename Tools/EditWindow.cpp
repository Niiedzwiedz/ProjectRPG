#include <algorithm>
#include "EditWindow.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

void EditWindow::start() {
	width = 1280;
	height = 720;
	sf::VideoMode mode;
	editorWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, height), "RPGEditor");
	editorWindow->setFramerateLimit(60);
	ImGui::SFML::Init(*editorWindow);
	textureManager.autoload();

	sf::Clock deltaClock;
	while(editorWindow->isOpen()) {
		eventPoll();
		ImGui::SFML::Update(*editorWindow, deltaClock.restart());
		editorWindow->clear();

		frameLoop();

		if(MouseMovement.leftClick) leftClickHandler();

		ImGui::SFML::Render(*editorWindow);
		editorWindow->display();

	}

	ImGui::SFML::Shutdown();
}

void EditWindow::frameLoop() {
	this->drawMenuBar();

	if(!EditingMap.isLoaded) {
		static char buf[128];
		static int width = 100, height = 100, type = 0;

		ImGui::OpenPopup("Enter map details");
		if (ImGui::BeginPopupModal("Enter map details", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
			ImGui::InputText("Filename", buf, 128);
			ImGui::InputInt("Width", &width);
			ImGui::InputInt("Height", &height);
			ImGui::InputInt("Default Tile", &type);

			ImGui::Dummy(ImVec2(ImGui::GetWindowWidth()-30, 0));
			if(ImGui::Button("Create") && width > 0 && height > 0) {
				EditingMap.width = width;
				EditingMap.height = height;
				EditingMap.fname = std::string(buf);
				EditingMap.mapData = Map::make_empty(Vec2u(width, height),type);
				EditingMap.mapData.initializeVertexArrays();
				EditingMap.isLoaded = true;
				picker.init();

				Tools.brush = std::make_shared<Brush>(EditingMap.mapData);
				Tools.cursor = std::make_shared<CursorTool>(EditingMap.mapData);
				Tools.creator = std::make_shared<NPCCreator>(EditingMap.mapData);
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		return;
	}

	Player fakePlayer;

	Vec2i pos = mapPosition;
	if(MouseMovement.isMoving)
		pos = mapPosition + sf::Mouse::getPosition(*editorWindow) - MouseMovement.mouseStart;

	sf::View view(Vec2f(pos) + (Vec2f(width, height) / 2.0f), Vec2f(width, height));
	editorWindow->setView(view);
	EditingMap.mapData.draw(*editorWindow, fakePlayer);

	MouseMovement.hoverCoordinates = (pos + sf::Mouse::getPosition(*editorWindow)) / (int)Tile::dimensions();
	MouseMovement.hoverCoordinates.x = std::clamp(MouseMovement.hoverCoordinates.x, 0, EditingMap.width-1);
	MouseMovement.hoverCoordinates.y = std::clamp(MouseMovement.hoverCoordinates.y, 0, EditingMap.height-1);

	picker.drawWindow();

	if(currentTool) currentTool->drawToolWindow(Vec2u(MouseMovement.hoverCoordinates),*editorWindow);
}

void EditWindow::eventPoll() {
	sf::Event event {};
	while (editorWindow->pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		if(event.type == sf::Event::Resized) {
			this->width = event.size.width;
			this->height = event.size.height;
		}

		if (event.type == sf::Event::Closed) {
			editorWindow->close();
		}

		if(event.type == sf::Event::MouseButtonPressed) {
			if(ImGui::GetIO().WantCaptureMouse) continue;

			if(event.mouseButton.button == sf::Mouse::Button::Left)
				MouseMovement.leftClick = true;
			if(event.mouseButton.button == sf::Mouse::Button::Middle) {
				MouseMovement.mouseStart = sf::Mouse::getPosition(*editorWindow);
				MouseMovement.isMoving = true;
			}
		}

		if(event.type == sf::Event::MouseButtonReleased) {
			if(ImGui::GetIO().WantCaptureMouse) continue;

			if(event.mouseButton.button == sf::Mouse::Button::Left)
				MouseMovement.leftClick = false;
			if(event.mouseButton.button == sf::Mouse::Button::Middle) {
				mapPosition += sf::Mouse::getPosition(*editorWindow) - MouseMovement.mouseStart;
				MouseMovement.isMoving = false;
			}
		}
	}
}

void EditWindow::leftClickHandler() {
	if(EditingMap.isLoaded) {
		if(currentTool) currentTool->onToolUse(Vec2u(MouseMovement.hoverCoordinates), picker.getSelection());
	}
}

bool EditWindow::drawCommonWindows() {
	return false;
}

void EditWindow::drawMenuBar() {
	if(ImGui::BeginMainMenuBar()) {
		bool thisFrame = false;
		if(ImGui::BeginMenu("File")) {
			if(ImGui::MenuItem("New Map...")) {
				EditingMap.isLoaded = false;
			}
			ImGui::EndMenu();
		}

#define TOOL_BUTTON(ptrName, buttonText) \
		if(currentTool && currentTool == ptrName) ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.56, 0.57f, 1.0f)); \
		if(ImGui::Button(buttonText) && currentTool != ptrName) { \
			thisFrame = true; \
			currentTool = ptrName; \
		} \
		if(currentTool && currentTool == ptrName && !thisFrame) ImGui::PopStyleColor(1);

		TOOL_BUTTON(Tools.cursor,"Cursor")
		TOOL_BUTTON(Tools.brush, "Brush")
		TOOL_BUTTON(Tools.creator, "NPC Tool")

		ImGui::TextColored(ImVec4(255, 255, 0,255),"Coords: %ix%i", MouseMovement.hoverCoordinates.x, MouseMovement.hoverCoordinates.y);

		ImGui::EndMainMenuBar();
	}
}