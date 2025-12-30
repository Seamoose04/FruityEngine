#pragma once
#include <type_traits>

template<typename Enum>
class Flags {
	static_assert(std::is_enum_v<Enum>, "Flags requires an enum type.");
public:
	Flags() = default;
	void AddFlag(Enum flag) {
		_flags |= static_cast<unsigned long>(flag);
	}
	void ClearFlag(Enum flag) {
		_flags &= ~static_cast<unsigned long>(flag);
	}
	void ClearAll() {
		_flags = 0;
	}
	bool CheckFlag(Enum flag) {
		return _flags & static_cast<unsigned long>(flag);
	}
	bool ProcessFlag(Enum flag) {
		bool flagSet = CheckFlag(flag);
		if (flagSet) {
			ClearFlag(flag);
		}
		return flagSet;
	}

private:
	unsigned long _flags = 0;
};
