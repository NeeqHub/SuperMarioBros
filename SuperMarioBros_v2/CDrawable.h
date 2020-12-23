#pragma once
#include "Component.h"

class Object;

class CDrawable
{
public:
	CDrawable();
	virtual ~CDrawable();

	virtual void Draw(Window& window);
	void SetSortOrder(int order);
	int GetSortOrder() const;

private:
	int sortOrder;
};

class SDrawable
{
public:
	void Add(std::vector<std::shared_ptr<Object>>& object);
	void ProcessRemovals();
	void Draw(Window& window);


private:
	void Add(std::shared_ptr<Object> object);
	void Sort();
	std::vector<std::shared_ptr<Object>> drawables;
};