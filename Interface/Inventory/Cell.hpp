#pragma once
#include <SFML/Graphics.hpp>
#include "AssetManager.hpp"
#include "Interface/Components/Frame.hpp"
#include <memory>
#include "World/Item.hpp"

class Cell : public Frame {
protected:
	std::shared_ptr<Item> item;
	bool empty;

	void SelfDraw(sf::RenderTarget&)override;
	void SelfInit()override;
public:
	Cell();
	Cell(const std::shared_ptr<Item>& _item);

	std::shared_ptr<Item> getItem() { return item; };
	bool isEmpty() { return empty; };
	//string Info() { return temporary; }	//'Return Item info to subwindow'
};