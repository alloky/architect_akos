#include "Compiler.h"
#include "cpuemu.h"

#include <GLFW/glfw3.h>
#include <iostream>


// GLFWwindow* window;

// 	/* Initialize the library */
// if (!glfwInit())
// return -1;

// /* Create a windowed mode window and its OpenGL context */
// window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
// if (!window)
// {
// 	glfwTerminate();
// 	return -1;
// }

// /* Make the window's context current */
// glfwMakeContextCurrent(window);

// /* Loop until the user closes the window */
// while (!glfwWindowShouldClose(window))
// {
// 	/* Render here */
// 	glClear(GL_COLOR_BUFFER_BIT);

// 	/* Swap front and back buffers */
// 	glfwSwapBuffers(window);

// 	/* Poll for and process events */
// 	glfwPollEvents();
// }

// glfwTerminate();

/*
void DrawImage(void) {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();


	glLoadIdentity();
	glDisable(GL_LIGHTING);


	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mark_textures[0].id);


	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, 100, 0);
	glTexCoord2f(1, 1); glVertex3f(100, 100, 0);
	glTexCoord2f(1, 0); glVertex3f(100, 0, 0);
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

}
*/

int main() {
	Compiler c;
	c.compile("test_prog", ".", "out_bins");
	CpuEmu emulator(20000);
	emulator.execute("out_bins/test_prog_0.0.1.asmbin");
}