//
// Created by arunc on 01/07/2025.
//

#ifndef TEXTURE_HPP
#define TEXTURE_HPP


#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdexcept>
#include <iostream>

class Texture {
public:
	static GLuint LoadTexture(const std::string& path, bool gammaCorrection = false) {
		SDL_Surface* surface = IMG_Load(path.c_str());
		if (!surface) {

		}

		SDL_Surface* formatted = SDL_ConvertSurface(surface, SDL_PIXELFORMAT_RGBA32);
		SDL_DestroySurface(surface);
		if (!formatted) {
			throw std::runtime_error("Failed to convert surface format: " + std::string(SDL_GetError()));
		}
		FlipSurfaceVertically(formatted);
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		GLenum internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, formatted->w, formatted->h,
					 0, GL_RGBA, GL_UNSIGNED_BYTE, formatted->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		SDL_DestroySurface(formatted);

		return texture;
	}


	static void FlipSurfaceVertically(SDL_Surface* surface) {
		int pitch = surface->pitch;
		char* temp = new char[pitch];
		char* pixels = static_cast<char*>(surface->pixels);

		for (int i = 0; i < surface->h / 2; ++i) {
			char* row1 = pixels + i * pitch;
			char* row2 = pixels + (surface->h - i - 1) * pitch;

			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}

		delete[] temp;
	}

	static GLuint LoadHDRTexture(const std::string& path) {
		// SDL3_image nu suportă HDR direct — păstrează `stb_image` doar pentru HDR sau folosește alt loader.
		throw std::runtime_error("HDR texture loading is not supported by SDL3_image. Use stb_image instead.");
	}
};

#endif // TEXTURE_HPP

