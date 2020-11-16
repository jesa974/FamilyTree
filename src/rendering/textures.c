#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <stdbool.h>
#include <math.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define rmask 0xFF000000
#define gmask 0x00FF0000
#define bmask 0x0000FF00
#define amask 0x000000FF
#else
#define rmask 0x000000FF
#define gmask 0x0000FF00
#define bmask 0x00FF0000
#define amask 0xFF000000
#endif

#include "../../include/texture.h"

/*Code converti du langage C++ vers le langage C par Thibaud Simon */
/* Ce qui a permis de rajouter une condition booleenne quant à l'utilisation des mip maps */

/* Cette partie de code est utile à l'application de textures sur des objets Glut, en utilisant SDL & SDL_image */

/* Auteur du code (source) : kayl - OpenClassRooms.com [https://openclassrooms.com/uploads/fr/ftp/kayl/sdz_sdlglutils.zip] */

SDL_Surface * flipSurface(SDL_Surface * surface)
{
    int current_line,pitch;
    SDL_Surface * fliped_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   surface->w,surface->h,
                                   surface->format->BitsPerPixel,
                                   surface->format->Rmask,
                                   surface->format->Gmask,
                                   surface->format->Bmask,
                                   surface->format->Amask);



    SDL_LockSurface(surface);
    SDL_LockSurface(fliped_surface);

    pitch = surface->pitch;
    for (current_line = 0; current_line < surface->h; current_line ++)
    {
        memcpy(&((unsigned char* )fliped_surface->pixels)[current_line*pitch],
               &((unsigned char* )surface->pixels)[(surface->h - 1  -
                                                    current_line)*pitch],
               pitch);
    }

    SDL_UnlockSurface(fliped_surface);
    SDL_UnlockSurface(surface);
    return fliped_surface;
}

/* Chargement des textures */
/* Appel : texture = loadTexture("monfichier.jpg/png", true/false (utilisation de mipmaps) ); */
GLuint loadTexture(const char * filename, bool useMipMap)
{
    GLuint glID;
    SDL_Surface * picture_surface = NULL;
    SDL_Surface *gl_surface = NULL;
    SDL_Surface * gl_fliped_surface = NULL;

    picture_surface = IMG_Load(filename);
    if (picture_surface == NULL)
        return 0;

    SDL_PixelFormat format = *(picture_surface->format);
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = rmask;
    format.Gmask = gmask;
    format.Bmask = bmask;
    format.Amask = amask;

    gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);

    gl_fliped_surface = flipSurface(gl_surface);

    glGenTextures(1, &glID);

    glBindTexture(GL_TEXTURE_2D, glID);


    if (useMipMap)
    {

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
                          gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
                          gl_fliped_surface->pixels);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
                     gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     gl_fliped_surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    SDL_FreeSurface(gl_fliped_surface);
    SDL_FreeSurface(gl_surface);
    SDL_FreeSurface(picture_surface);

    return glID;
}

int savePNGPicture(char *destination, int width, int height){
	SDL_Surface * image = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, rmask, gmask, bmask, 0);

	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	image = flipSurface(image);

	SDL_SaveBMP(image, destination);
	SDL_FreeSurface(image);
	
	return EXIT_SUCCESS;
}
