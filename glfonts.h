struct glfonts_font {
	FT_Face *faces;
	uint32_t num_faces;
};

int32_t glfonts_init_lib(FT_Library *lib);

int32_t glfonts_init_font(FT_Library lib, struct glfonts_font *font, const char **filename, uint32_t num_faces, FT_F26Dot6 size_x, FT_F26Dot6 size_y, FT_UInt dpi_x, FT_UInt dpi_y);

void glfonts_release_font(struct glfonts_font *font);

void glfonts_release_lib(FT_Library lib);

int32_t glfonts_display_string(FT_Library lib, struct glfonts_font *font, const wchar_t *str, GLint x, GLint y, GLuint rgba);
