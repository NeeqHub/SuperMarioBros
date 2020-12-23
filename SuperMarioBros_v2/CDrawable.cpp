#include "CDrawable.h"
#include "Object.h"

CDrawable::CDrawable() : sortOrder(0) {}

CDrawable::~CDrawable() {}

void CDrawable::Draw(Window& window)
{
}

void CDrawable::SetSortOrder(int order)
{
	sortOrder = order;
}

int CDrawable::GetSortOrder() const
{
	return sortOrder;
}

void SDrawable::Add(std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto o : objects)
	{
		Add(o);
	}

	Sort();
}

void SDrawable::Add(std::shared_ptr<Object> object)
{
	std::shared_ptr<CDrawable> draw = object->GetDrawable();

	if (draw)
	{
		drawables.emplace_back(object);
	}
}

void SDrawable::Sort()
{
	std::sort(drawables.begin(), drawables.end(), [](std::shared_ptr<Object> a, std::shared_ptr<Object> b) -> bool
	{
		return a->GetDrawable()->GetSortOrder() < b->GetDrawable()->GetSortOrder();
	}
	);
}

void SDrawable::ProcessRemovals()
{
	auto objIterator = drawables.begin();
	while (objIterator != drawables.end())
	{
		auto obj = *objIterator;

		if (obj->IsQueuedForRemoval())
		{
			objIterator = drawables.erase(objIterator);
		}
		else
		{
			++objIterator;
		}
	}
}

void SDrawable::Draw(Window& window)
{
	for (auto& d : drawables)
	{
		d->Draw(window);
	}
}