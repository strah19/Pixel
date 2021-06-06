#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

#include "Core/Layer.h"
#include "Renderer/FrameBuffer.h"
#include "Camera/PerspectiveCameraController.h"

namespace Pixel {
	class EditorLayer : public Layer {
	public:
		EditorLayer(std::shared_ptr<Pixel::FrameBuffer>* framebuffer);

		void OnAttach();
		void OnDetach();
		void UpdateGui();

		void OnUpdate(float delta);
		void UserDefEvent(Event& event);

		inline PerspectiveCameraController GetEditorCamera() { return editor_camera; }
	private:
		bool activate_editor = false;
		std::shared_ptr<Pixel::FrameBuffer>* framebuffer;
		PerspectiveCameraController editor_camera;
	};
}

#endif EDITOR_LAYER_H