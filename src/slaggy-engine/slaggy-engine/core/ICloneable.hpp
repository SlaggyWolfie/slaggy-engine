#pragma once
#ifndef SE_I_CLONEABLE_HPP
#define SE_I_CLONEABLE_HPP

class ICloneable
{
public:
	virtual ~ICloneable() = default;
	virtual ICloneable* clone() = 0;
};

template <typename T>
class ICloneableTemplate// : public ICloneable
{
public:
	virtual ~ICloneableTemplate() = default;
	virtual T* clone() = 0;
};
#endif