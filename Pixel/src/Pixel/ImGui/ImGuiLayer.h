#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace Pixel {
	class ImGuiLayer {
	public:
		static void Init();
		static void Destroy();
		
		static void Begin();
		static void End();
	};
}

#endif // !IMGUI_LAYER_H
