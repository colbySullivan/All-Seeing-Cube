#include <GL/glut.h>
#include <GL/glut.h>
#include <cstdlib>

static bool spinning = true;
static bool rolling = true;
static int rand_message = 1;
static const int FPS = 60; // This is the number of frames per second to render.
static GLfloat currentAngleOfRotation = 0.0;
const float window_height = 500;
const float window_width = 500;

// Performs application-specific initialization. Sets colors and sets up a
// simple orthographic projection.
void init() {

  // Set a deep purple background and draw in a greenish yellow.
  glClearColor(0.5, 0.0, 0.2, 1.0);
  glColor3f(0.5, 0.0, 0.0);

  // Set up the viewing volume: 500 x 500 x 1 window with origin lower left.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 500.0, 0.0, 500.0, 0.0, 1.0);
}

void timer(int v) {
  if (spinning) {
    currentAngleOfRotation += 5.0;
    if (currentAngleOfRotation > 360.0) {
      currentAngleOfRotation -= 360.0;
    }
    glutPostRedisplay();
  }
  glutTimerFunc(1000/FPS, timer, v);
}

// Handles mouse events
void mouse(int button, int state, int x, int y) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    spinning = true;
  } else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
    spinning = false;
  }
}

void drawBitmapText(char* text, int x, int y) {
    char* c;

    glLoadIdentity();
    glColor3f(255, 255, 255);
    glRasterPos2f(x, y);

    for(c = text; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *c);
    }
}

void drawControls(){
    char* move_message = (char*) "Press SPACE to stop";
    char* exit_message = (char*) "Press ESC or Q to exit";

    drawBitmapText(move_message, -70, 40);
    drawBitmapText(exit_message, 10, -40);
}

char* pick_message(){
	char* message = (char*) "Ehh try again";
	switch(rand_message){
		case 0:{
			message = (char*) "Yes";
			break;
		}
		case 1:{
			message = (char*) "No";
			break;
		}
		case 2:{
			message = (char*) "Maybe";
			break;
		}
		case 3:{
			message = (char*) "You will be hungry in one hour";
			break;
		}
	}
	if(rand_message > 3){
		char* message = (char*) "Since you failed press SPACE to spin again";
		drawBitmapText(message, -70, 40);
	}

	return message;
}

void drawMessage(){
	char* buffer = pick_message();
	char* move_message = buffer;
	glLoadIdentity();
	glColor3f(100, 100, 100);
	drawBitmapText(move_message, -20, 0);
}

void emptyDice(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glColor3f(100, 0, 0);
	glLoadIdentity();
	glRotatef(currentAngleOfRotation, 0.0, 5.0, 1.0);
	glRectf(-25.0, -25.0, 25.0, 25.0); //Dimensions of object
}

void rolledDice(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3f(0, 0, 0);
	glRotatef(currentAngleOfRotation, 0.0, 0.0, 1.0);
	glRectf(-25.0, -25.0, 25.0, 25.0); //Dimensions of object
	glFlush();
	glutSwapBuffers();
}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2f(250, 250);
	if(!rolling){
		rolledDice();
		drawMessage();
	}
	else{
		//Rand in here so the message doesn't change while diplaying rolledDice
		rand_message = rand() % 7;
		emptyDice();
		drawControls();
	}
	glFlush();
	glutSwapBuffers();
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  GLfloat aspect = (GLfloat)w / (GLfloat)h;
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if (w <= h) {
    // width is smaller, go from -50 .. 50 in width
    glOrtho(-80.0, 80.0, -50.0/aspect, 50.0/aspect, -1.0, 1.0);
  } else {
    // height is smaller, go from -50 .. 50 in height
    glOrtho(-80.0*aspect, 50.0*aspect, -50.0, 50.0, -1.0, 1.0);
  }
}

void keyboard(unsigned char key, int x, int y){//Need unused x and y for glutKeyboardFunc
	switch(key){
    	case ' ':{
        	rolling = !rolling;
            break;
        }
        case 27:// Escape key
        case 'q':{
            exit(0);
        }
    }
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(window_height, window_width);
  glutInitWindowPosition(40, 40);
  glutCreateWindow("Decision Wheel");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(100, timer, 0);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
}
