#include <iostream>
#include <glut.h>

#define DEBUG false

using namespace std;

const int w = 700, h = 700;
int n;
bool doors[100] = {false};
bool printedFinalOutput = false;
int pass = 0, door;

inline void GUI_Text(string s, int x, int y, float r, float g, float b){
	glColor3f( r, g, b );
	glDisable(GL_TEXTURE_2D); 
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, w, 0.0, h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);
	void * font = GLUT_BITMAP_9_BY_15;
	for (string::iterator i = s.begin(); i != s.end(); ++i)
	{
	  char c = *i;
	  glColor3f(1.0, 1.0, 1.0);
	  glutBitmapCharacter(font, c);
	}
	glMatrixMode(GL_PROJECTION); 
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW); 
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

//same rendering as above, but forces black
inline void GUI_Text(string s, int x, int y){ 
	GUI_Text(s, x, y, 0, 0, 0);
}

inline void debug(){
	int num = 1;
	for(int i = 0; i < 100; i++){
		cout << num++ << ": " << (doors[i]?"open":"closed") << endl;
	}
}

inline void render(){
	gluLookAt(-0.5f, 6, 0.001f,
			  -0.5f, -0.5f , 0,
			  0, 1 , 0);
	int x = -5, z = -5;
	for(int i = 0; i < 100; i++){
		//set the color
		if(doors[i]){
			glColor3f(0.25f, 0.25f, 0.25f);
		}else{
			glColor3f(0.75f, 0.75f, 0.75f);
		}
		glPushMatrix();{
			glTranslatef(x, 0, z);
			glScalef(1, 0.0005f, 1);
			glutSolidCube(0.9f);
			x++;
			if(x >= 5){
				z++;
				x = -5;
			}
		}glPopMatrix();
	}
}

inline void update(){
	for(door = pass; door < 100; door += pass + 1){
		doors[door] = !doors[door];
		if(DEBUG){
			debug();
			printf("%d %d\n", pass, door);
		}
	}
}

void cb_display(){
	if(pass >= 100){
		if(!printedFinalOutput){
			GUI_Text("Done", 10, 10);
			debug();
			if(DEBUG)
				cout << "Done" << endl;
			printedFinalOutput = true;
		}
		return;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	update();
	render();
	glFlush();
	glutSwapBuffers(); // for smoother animation
	pass++;
	_sleep(500);//wait before calculating the next frame
}

void cb_idle(){
	glutPostRedisplay();
}

void cb_reshape(int w, int h) {
	int aspect = w/h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, aspect, 0.0005, 100); // necessary to preserve aspect ratio
	glMatrixMode(GL_MODELVIEW);
}

inline void setup(int & argc, char* argv[]){
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutCreateWindow("100 Doors - By Cody");
	glutIgnoreKeyRepeat(false);

	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable (GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glCullFace(GL_FRONT_AND_BACK);
	//lighting
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
	GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

	glClearColor(1, 1, 1, 1);
	glutDisplayFunc(cb_display);
	glutIdleFunc(cb_idle);
	glutReshapeFunc(cb_reshape);
}

int main(int argc, char* argv[]){
	//100 doors
	setup(argc, argv);
	glutMainLoop();
}

