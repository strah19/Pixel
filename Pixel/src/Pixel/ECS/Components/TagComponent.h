#ifndef TAG_COMPONENT_H
#define TAG_COMPONENT_H

#include "ECS/EntityComponentSystem.h"

namespace Pixel {
	class TagComponent : public Component {
	public:
		TagComponent(const std::string& name) : name(name), Component("Tag") { }

		inline std::string GetTagName() const { return name; }
		inline void Rename(const std::string& name) { this->name = name; }
	private:
		std::string name;
	};
}

#endif //!TAG_COMPONENT_H