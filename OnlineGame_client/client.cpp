#include "head.h"
#include "WindowToNDC.h"
#include "Player.h"
#include "socket.h"

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

//Init
Player player{ Vec2{0.0f, 0.0f}, 0.5f, 0.5f, RGB{1.0f, 0.0f, 0.0f} };

int main(int argc, char** argv) 
{
	if (!InitSocketClient()) 
		return 1;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("FreeGLUT Example");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		CleanupSocket();
		return 1;
	}
	else {
		std::cout << "GLEW initialized successfully" << std::endl;
	}

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();

	CleanupSocket();
	return 0;
}

GLvoid drawScene(GLvoid)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(player.color.r, player.color.g, player.color.b);
	glRectf(player.pos.x - player.width / 2.0f,
		player.pos.y - player.height / 2.0f,
		player.pos.x + player.width / 2.0f,
		player.pos.y + player.height / 2.0f);
	
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	string msg;

	switch (key) {
	case 'w':
		msg = "MOVE|UP";
		break;
	case 'a':
		msg = "MOVE|LEFT";
		break;
	case 's':
		msg = "MOVE|DOWN";
		break;
	case 'd':
		msg = "MOVE|RIGHT";
		break;
	default:
		msg = "MOVESTOP";
		break;
	}

	sendto(clientSocket, msg.c_str(), (int)msg.length(), 0,
		(SOCKADDR*)&serverAddress, sizeof(serverAddress));

	glutPostRedisplay();
}
