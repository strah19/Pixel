#include "pixelpch.h"
#include "RendererContext.h"
#include "Renderer/RendererAPI.h"

namespace Pixel {
	std::unique_ptr<RendererContext> RendererContext::CreateRendererContext(Window* window) {
		switch (RendererAPI::GetAPI()) {
			
		}
		return nullptr;
	}
}