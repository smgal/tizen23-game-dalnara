
/*
 * Win32
 * config 1: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 2: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 3: rgba=5:6:5:0(16), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 4: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 5: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 * config 6: rgba=8:8:8:8(32), ds=24:0, caveat=3038, native=0, level=0, surface=0005, visual=3038, id=0
 *
 * WIZ
 * config 1: rgba=5:6:5:0(16), ds=16:0, caveat=3050, native=0, level=0, surface=0005, visual=3038, id=0
 * config 2: rgba=5:6:5:0(16), ds=16:0, caveat=3050, native=0, level=0, surface=0005, visual=3038, id=0
*/

#if defined(_WIN32)
#	pragma warning(disable: 4786)
#	pragma comment(lib, "libgles_cm.lib")
#endif

////////////////////////////////////////////////////////////////////////////////
// uses

#include "gfx3d_config.h"

////////////////////////////////////////////////////////////////////////////////
// native OpenGL definition

namespace avej_lite { namespace gfx3d
{
	bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth);
	void DoneOpenGL(void);
	void SwapBuffers(void);
	void ProcessMessage(unsigned long delay_time);
}}

////////////////////////////////////////////////////////////////////////////////
// native OpenGL ES implementation

#if !defined(NULL)
#define NULL  0
#endif

namespace avej_lite { namespace gfx3d
{
	bool InitOpenGL(unsigned int width, unsigned int height, unsigned int depth)
	{
		glClearColorx(0, 0, 0, 0);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		return true;
	}

	void DoneOpenGL(void)
	{
	}

	void s_DrawTexture2D(int x, int y, int w, int h, GLuint texture, int tex_w, int tex_h, bool can_display)
	{
		const GLfixed Z_DEPTH = 50;

		struct
		{
			GLint x, y, width, height;
		} view_port;

		// ���� view port ���� ���
		glGetIntegerv(GL_VIEWPORT, (GLint*)&view_port);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();

		glOrthox(view_port.x, view_port.width, view_port.y , view_port.height, -Z_DEPTH, Z_DEPTH);

		const GLfixed x1 = x;
		const GLfixed y1 = view_port.height - h - y;
		const GLfixed x2 = x1 + w;
		const GLfixed y2 = y1 + h;

		const GLfixed vertices[] =
		{
			x1, y1,
			x2, y1,
			x1, y2,
			x2, y2
		};

		const GLfixed tex_x1 = 0;
		const GLfixed tex_y1 = 0;
		const GLfixed tex_x2 = 65536 * view_port.width  / tex_w;
		const GLfixed tex_y2 = 65536 * view_port.height / tex_h;

		const GLfixed texture_coord[] =
		{
			tex_x1, tex_y1,
			tex_x2, tex_y1,
			tex_x1, tex_y2,
			tex_x2, tex_y2
		};

		static const unsigned short index_buffer[] =
		{
			0, 1, 2, 3
		};

		// vertex buffer �Է�
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2, GL_FIXED, 0, vertices);

		// texture coordinate �Է�
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FIXED, 0, texture_coord);

		// texture�� ��� ��
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatex(0, 0, -Z_DEPTH+1);

		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, &index_buffer[0]);

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDisable(GL_TEXTURE_2D);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	}

	void SwapBuffers(void)
	{
		//eglSwapBuffers(s_egl_attrib.egl_display, s_egl_attrib.egl_surface_window);
	}
	
	void ProcessMessage(unsigned long delay_time)
	{
		//osal::ProcessMessage(delay_time);
	}
}}
