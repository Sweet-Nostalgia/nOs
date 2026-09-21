#include "nospch.h"
#include "RenderCommand.h"

#include "Platform/DirectX/DirectXRendererAPI.h"

namespace nOs {
	RendererAPI* RenderCommand::s_RendererAPI = new DirectXRendererAPI;
}