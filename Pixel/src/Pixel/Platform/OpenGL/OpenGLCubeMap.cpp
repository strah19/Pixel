#include "pixelpch.h"
#include "OpenGLCubeMap.h"
#include "Core/Logger.h"

#include <glad/glad.h>
#include "OpenGLTexture.h"

namespace Pixel {
     OpenGLCubeMap::OpenGLCubeMap(const std::vector<std::string>& faces) {
         glGenTextures(1, &texture_id);
         glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

        int width, height, channels;
        for (unsigned int i = 0; i < faces.size(); i++) {
            unsigned char* data = LoadTexture(faces[i].c_str(), &width, &height, &channels, 0);
            if (data) 
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else 
                PIXEL_LOG_ERROR("CUBEMAP::FAILED_TO_LOAD::%s", faces[i].c_str());
            FreeTexture(data);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	uint32_t OpenGLCubeMap::GetTexture() {
		return texture_id;
	}

    void OpenGLCubeMap::Bind(uint32_t slot) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    }

    void OpenGLCubeMap::UnBind() {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGLCubeMap::Begin() {
        glDepthMask(GL_FALSE);
    }

    void OpenGLCubeMap::End() {
        glDepthMask(GL_TRUE);
    }
}