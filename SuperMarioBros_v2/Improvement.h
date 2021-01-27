#pragma once
#include "Object.h"

class Improvement : public Object
{
public:
	Improvement(SharedContext& cotext);
	void Create();

private:
	SharedContext& context;

};