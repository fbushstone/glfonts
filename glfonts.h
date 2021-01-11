struct glfonts_font {
	FT_Face *faces;
	unsigned int num_faces;
};

int glfonts_init_lib(FT_Library *lib);

int glfonts_init_font(FT_Library lib, struct glfonts_font *font, const char **filename, unsigned int num_faces, FT_F26Dot6 size_x, FT_F26Dot6 size_y, FT_UInt dpi_x, FT_UInt dpi_y);

void glfonts_release_font(struct glfonts_font *font);

void glfonts_release_lib(FT_Library lib);

int glfonts_display_string(FT_Library lib, struct glfonts_font *font, const wchar_t *str, GLint x, GLint y, GLuint rgba);
