#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include <imgui.h>
#include "Renderer/FrameBuffer.h"
#include <memory>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include "Core/Layer.h"

namespace Pixel {
	class ImGuiLayer : public Layer {
	public:
		ImGuiLayer();
		
		virtual void OnAttach();
		void OnDetach();

		void OnUpdate(float delta);
		void UserDefEvent(Event& event);
		void UpdateGui() { }

		void Begin();
		void End();
	private:

		static void UpdateDocking(std::shared_ptr<FrameBuffer>* framebuffer);
	};
}

#endif // !IMGUI_LAYER_H
