#pragma once

template<typename T>
class Prototype
{
public:
	virtual ~Prototype() = default;
	virtual T* Clone() = 0;
};

