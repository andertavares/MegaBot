#pragma once

// enums, why you no type safe and pollute my namespaces?
template<typename def, typename inner = typename def::type>
class SafeEnum : public def
{
public:
	SafeEnum() : mValue() {}
	SafeEnum(inner v) : mValue(v) {}

	inner underlying() const { return mValue; }

	bool operator == (const SafeEnum &other) const { return mValue == other.mValue; }
	bool operator != (const SafeEnum &other) const { return mValue != other.mValue; }
	bool operator <  (const SafeEnum &other) const { return mValue <  other.mValue; }
	bool operator <= (const SafeEnum &other) const { return mValue <= other.mValue; }
	bool operator >  (const SafeEnum &other) const { return mValue >  other.mValue; }
	bool operator >= (const SafeEnum &other) const { return mValue >= other.mValue; }

private:
	inner mValue;
};