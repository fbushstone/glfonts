#include <errno.h>
#include <stddef.h>
#include <wchar.h>

#include <GL/gl.h>
#include <EGL/egl.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H

#include "glfonts.h"

int glfonts_init_lib(FT_Library *lib) {
	int err;

	if((err = FT_Init_FreeType(lib)))
		return err;

	return 0;
}

int glfonts_init_font(FT_Library lib, struct glfonts_font *font, const char **filenames, unsigned int num_faces, FT_F26Dot6 size_x, FT_F26Dot6 size_y, FT_UInt dpi_x, FT_UInt dpi_y) {
	int err;
	unsigned int i;

	font->num_faces = num_faces;

	if(!(font->faces = malloc(font->num_faces * sizeof(FT_Face))))
		return -ENOMEM;

	for(i = 0; i < font->num_faces; ++i) {
		if((err = FT_New_Face(lib, filenames[i], 0, &font->faces[i])))
        	        return err;
	
        	if(FT_IS_SCALABLE(font->faces[i]))
                	if((err = FT_Set_Char_Size(font->faces[i], size_x * 64, size_y * 64, dpi_x, dpi_y)))
        	                return err;
	}
        return 0;
}

void glfonts_release_font(struct glfonts_font *font) {
	unsigned int i;
	for(i = 0; i < font->num_faces; ++i) {
		FT_Done_Face(font->faces[i]);
	}
	free(font->faces);
}

void glfonts_release_lib(FT_Library lib) {
	FT_Done_FreeType(lib);
}

int glfonts_display_string(FT_Library lib, struct glfonts_font *font, const wchar_t *str, GLint x, GLint y, GLuint rgba) {
        int err;
	unsigned int i, j;
        FT_UInt glyph_index;
	FT_Bitmap flipped_ft_bitmap;
	GLfloat width, height, descent, advance;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glColor4ub((GLubyte) (rgba >> 24), (GLubyte) ((rgba >> 16) & 0xFF), (GLubyte) ((rgba >> 8) & 0xFF), (GLubyte) (rgba & 0xFF));
        glRasterPos2i(x, y);

        for(i = 0; i < wcslen(str); ++i) {
		for(j = 0; j < font->num_faces; ++j)
			if((glyph_index = FT_Get_Char_Index(font->faces[j], str[i])))
				break;

		if(!glyph_index)
				return -1;
	
                if((err = FT_Load_Glyph(font->faces[j], glyph_index, FT_LOAD_RENDER | FT_LOAD_TARGET_MONO)))
			return err;
		
		if((descent = (GLfloat) ((font->faces[j]->glyph->metrics.horiBearingY - font->faces[j]->glyph->metrics.height) / -64) - 1.0) == -1.0)
        	        descent = 0.0;

		width = (GLfloat) (font->faces[j]->glyph->bitmap.pitch * 8);
		height = (GLfloat) (font->faces[j]->glyph->bitmap.rows);
		advance = (GLfloat) (font->faces[j]->glyph->metrics.horiAdvance / 64);

        	font->faces[j]->glyph->bitmap.pitch = -font->faces[j]->glyph->bitmap.pitch;
        	FT_Bitmap_Init(&flipped_ft_bitmap);
		FT_Bitmap_Copy(lib, &font->faces[j]->glyph->bitmap, &flipped_ft_bitmap);
		
        	glBitmap(width, height, (GLfloat) 0, descent, advance, (GLfloat) 0, flipped_ft_bitmap.buffer);

        	FT_Bitmap_Done(lib, &flipped_ft_bitmap);
        }

        glFlush();

	return 0;
}
