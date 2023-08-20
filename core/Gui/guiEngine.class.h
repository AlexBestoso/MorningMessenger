#include "./objects/objectLinker.h"
#include "./GuiContext.class.h"
class GuiEngine{
	private:
		static void mouseClick(int button, int state, int x, int y){
                	context.mouseClickContextSwitch(button, state, x, y);
        	}
	
        	static void resize(int w, int h){
        	        context.displayW = w;
        	        context.displayH = h;
        	        glViewport(0, 0, w, h);
        	}

        	static void mouseMovement(int x, int y){
        	        context.mouseMovementContextSwitch();
        	}

        	static void display(void){
        	        context.displayContextSwitch();
        	        glutSwapBuffers();
        	}

        	static void mousePassive(int x, int y){
        	        context.mousePassiveContextSwitch(getX(x), getY(y));
        	}

        	static void idle(void){
        	        context.idleContextSwitch();
        	        glutPostRedisplay();
        	}

		static void keyboard_down(unsigned char key, int mouseX, int mouseY){
        	        context.keydownContextSwitch(key, getX(mouseX), getY(mouseY));
        	        glutPostRedisplay();
        	}

        	static void keyboard_up(unsigned char key, int mouseX, int mouseY){
        	        context.keyupContextSwitch(key, getX(mouseX), getY(mouseY));
        	        glutPostRedisplay();
        	}

		static void keyboardSpecial_down(int key, int mouseX, int mouseY){
			context.specialKeydownContextSwitch(key, getX(mouseX), getY(mouseY));
			glutPostRedisplay();
		}

		static void keyboardSpecial_up(int key, int mouseX, int mouseY){
                        context.specialKeyupContextSwitch(key, getX(mouseX), getY(mouseY));
                        glutPostRedisplay();
                }
		
	public:
		int displayX = 0;
        	int displayY = 0;

        	void getDisplayResolution(void){
        	        Display* d = XOpenDisplay(NULL);
        	        Screen*  s = DefaultScreenOfDisplay(d);
        	        displayX = s->width;
        	        displayY = s->height;
        	        XCloseDisplay(d);
        	}

        	void startEngine(int argc, char *argv[]){
        	        getDisplayResolution();
	
	                glutInit(&argc, argv);
	                glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	                glutInitWindowSize(displayX, displayY);
	                glutInitWindowPosition(0, 0);
	                glutCreateWindow("Morning Messenger");
	                glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

	                glutDisplayFunc(display);
	                glutMouseFunc(mouseClick);
	                glutMotionFunc(mouseMovement);
	                glutPassiveMotionFunc(mousePassive);
	                glutKeyboardUpFunc(keyboard_up);
			glutSpecialUpFunc(keyboardSpecial_up);
	                glutIdleFunc(idle);
	                glutKeyboardFunc(keyboard_down);
			glutSpecialFunc(keyboardSpecial_down);
	                glutReshapeFunc(resize);

	                glutMainLoop();
        	}

		static float getX(float x){
         	       if(x >=0 && x < context.displayW/2){
         	               x = -1 + (x*((9999999/(context.displayW/2))*0.0000001));
         	       }else if(x == context.displayW/2){
         	               x = 0;
         	       }else if(x > context.displayW/2 && x < context.displayW){
         	               x = -1 + (x*((9999999/(context.displayW/2))*0.0000001));
         	       }else if(x == context.displayW){
         	               x = 1;
         	       }
        	        return x;
        	}

        	static float getY(float y){
        	        if(y >=0 && y < context.displayH/2){
        	                y = -1 + (y*((9999999/(context.displayH/2))*0.0000001));
        	        }else if(y == context.displayH/2){
        	                y = 0;
        	        }else if(y > context.displayH/2 && y < context.displayH){
        	                y = -1 + (y*((9999999/(context.displayH/2))*0.0000001));
        	        }else if(y == context.displayH){
        	                y = 1;
        	        }
        	        return y;
        	}
};
