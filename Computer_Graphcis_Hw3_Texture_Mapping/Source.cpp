#include<Windows.h>
#include<gl\GL.h>
#include<gl\GLU.h>
#include<gl\glut.h>
#include<math.h>
#include <iostream>

using namespace std;
#define PI 3.14159265
#define WIDTH_HEIGHT 512

typedef GLfloat point3[3]; /* array of 3 floats */
typedef GLfloat position[3]; /* array of 3 floats */
position p[36][18];
position polygon_nomal_vector[36][18];
position point_nomal_vector[36][18];
int circle_number = 36;
int point_number = 18;
int temp_number = 1;//��ü ���к���
GLuint texIDs[3];

void draw_polygons_normal_vector(int _circle_number, int _point_number);
void draw_points_normal_vector(int _circle_number, int _point_number);
float*  cross_product(position p0, position p1, position p2, position cross_P);
GLfloat my_position[4] = { 1,1,1,1 };

GLuint texID;
void myNormalize(position &p)
{
	double d = p[0] * p[0] + p[1] * p[1] + p[2] * p[2];//���̸� 1�� ����
	if (d > 0)
	{
		float len = (float)(1.0 / sqrt(d));
		p[0] *= len;
		p[1] *= len;
		p[2] *= len;
	}
}
void myCrossProduct(position a, position b, position c, position &r)
{
	r[0] = (b[1] - a[1])*(c[2] - a[2]) - (b[2] - a[2])*(c[1] - a[1]);
	r[1] = (b[2] - a[2])*(c[0] - a[0]) - (b[0] - a[0])*(c[2] - a[2]);
	r[2] = (b[0] - a[0])*(c[1] - a[1]) - (b[1] - a[1])*(c[0] - a[0]);
	myNormalize(r);
}
GLuint LoadTexture(int num)
{
	GLuint texture;
	unsigned char* image = new unsigned char[WIDTH_HEIGHT * WIDTH_HEIGHT * 3];

	FILE* file;
	string str = "Material/";
	string filename[3] = { "check","marble", "wood" };
	str.append(filename[num]);
	str.append(".raw");

	file = fopen(str.c_str(), "rb");
	
	fread(image, WIDTH_HEIGHT * WIDTH_HEIGHT * 3, 1, file);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH_HEIGHT, WIDTH_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	fclose(file);
	free(image);

	return texture;
}
void Draw_point()
{
	p[36][0][0] = 0.0;
	double x = 0.2; //�߽�
	double y = 0.2; //�߽�
	double r = 0.04; //36, 18�� �׸���
	for (int i = 0; i < 18; i++)
	{
		float theta = (float)i * 20;
		float temp[3] = {
			cos(theta * PI / 180) * r - sin(theta * PI / 180) * r + x,
			sin(theta * PI / 180) * r + cos(theta * PI / 180) * r + y, 0
		};
		memcpy(p[0][i], temp, sizeof p[0][i]); //memcpy�� �̿��ؾ��ϴ±���
	}
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			float theta = (float)i * 10;
			float temp[3] = {
				cos(theta * PI / 180) * p[0][j][0] - sin(theta * PI / 180) * p[0][j][2], p[0][j][1],
				-sin(theta * PI / 180) * p[0][j][0] + cos(theta * PI / 180) * p[0][j][2]
			};

			memcpy(p[i][j], temp, sizeof p[i][j]); //memcpy�� �̿��ؾ��ϴ±���
		}
	}
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			position a;

			
			myCrossProduct(p[i][j],  p[(i + 1) % 36][j], p[i][(j + 1) % 18], a);

			memcpy(polygon_nomal_vector[i][j], a, sizeof(a));
		}
	}
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 18; j++)
		{
			position a;
			for (int k = 0; k < 3; k++)
			{
				point_nomal_vector[i][j][k] = ((polygon_nomal_vector[i][j][k] +
					polygon_nomal_vector[(i + 35) % 36][j][k] +
					polygon_nomal_vector[i][(j + 17) % 18][k] +
					polygon_nomal_vector[(i + 35) % 36][(j + 17) % 18][k]) /4 );

			}
			myNormalize(point_nomal_vector[i][j]);//�ϳ� �� ���ϳ� ���ڳ�

			//cross_product(p[i][j], p[i][(j + 1) % 18], p[(i + 1) % 36][j], a);
			//memcpy(polygon_nomal_vector[i][j], a, sizeof(a));
		}
	}
	for (int i = 0; i < 3; i++)texIDs[i] = LoadTexture(i);
	//texID = 1;
}
void draw_clean() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// camera view configuration 
	gluLookAt(1.f, 1.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	// draw
	glColor3f(1.f, 0.f, 0.f); // ������ ����
	glBegin(GL_LINE_LOOP); // x�� �׸���
	glVertex3f(10.0, 0.0, 0.0); // ���� ��
	glVertex3f(0, 0.0, 0.0); // �� ��
	glEnd();
	glColor3f(0.f, 1.f, 0.f); // �ʷϻ� ����
	glBegin(GL_LINE_LOOP); // y�� �׸���
	glVertex3f(0.0, 10.0, 0.0);
	glVertex3f(0.0, 0, 0.0);
	glEnd();

	glColor3f(0.f, 0.f, 1.f); // �Ķ��� ����
	glBegin(GL_LINE_LOOP); // z�� �׸���
	glVertex3f(0.0, 0.0, 10.0);
	glVertex3f(0.0, 0.0, 0);
	glEnd();
}

float*  cross_product(position p0, position p1, position p2, position cross_P)
{
	temp_number = 4;

	position vect_A;
	position vect_B;
	for (int i = 0; i < 3; i++)
	{
		vect_A[i] = p1[i] - p0[i];
		vect_B[i] = p2[i] - p0[i];
	}

	cross_P[0] = vect_A[1] * vect_B[2] - vect_A[2] * vect_B[1];
	cross_P[1] = vect_A[2] * vect_B[0] - vect_A[0] * vect_B[2];
	cross_P[2] = vect_A[0] * vect_B[1] - vect_A[1] * vect_B[0];

	return cross_P;
}
void draw_polygons_normal_vector(int _circle_number = 36, int _point_number = 18)

{
	temp_number = 6;

	draw_clean();
	////�׵θ� �׸���
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.f, 0.f, 0.f); // ������ ���� //black
	glBegin(GL_QUADS); // mode ����
	for (int i = 0; i < _circle_number; i++)
	{
		for (int j = 0; j < _point_number; j++)
		{
			glVertex3fv(p[i][j]);
			glVertex3fv(p[i][((j + 1) % 18)]);
			glVertex3fv(p[(i + 1) % 36][((j + 1) % 18)]);
			glVertex3fv(p[(i + 1) % 36][(j)]);

		}
	}
	glEnd();

	glColor3f(0.f, 0.f, 0.f); // ������ ����

	for (int i = 0; i < _circle_number; i++)
	{
		for (int j = 0; j < _point_number; j++)
		{
			/*glPointSize(3);
			glLineWidth(3);*/
			glBegin(GL_LINES); // mode ����

			/*glVertex3fv(a);*/
			glVertex3f((p[i][j][0] + p[i][(j + 1) % 18][0] + p[(i + 1) % 36][(j + 1) % 18][0] + p[(i + 1) % 36][j][0]) / 4//���� ��� �� (����� �̷�� ������ ���)
				, (p[i][j][1] + p[i][(j + 1) % 18][1] + p[(i + 1) % 36][(j + 1) % 18][1] + p[(i + 1) % 36][j][1]) / 4,
				(p[i][j][2] + p[i][(j + 1) % 18][2] + p[(i + 1) % 36][(j + 1) % 18][2] + p[(i + 1) % 36][j][2]) / 4);
			glVertex3f((p[i][j][0] + p[i][(j + 1) % 18][0] + p[(i + 1) % 36][(j + 1) % 18][0] + p[(i + 1) % 36][j][0]) / 4 + polygon_nomal_vector[i][j][0] * 1.0//���� ��� ���� ������ ���� �����ش�.
				, (p[i][j][1] + p[i][(j + 1) % 18][1] + p[(i + 1) % 36][(j + 1) % 18][1] + p[(i + 1) % 36][j][1]) / 4 + polygon_nomal_vector[i][j][1] * 1.0,
				(p[i][j][2] + p[i][(j + 1) % 18][2] + p[(i + 1) % 36][(j + 1) % 18][2] + p[(i + 1) % 36][j][2]) / 4 + polygon_nomal_vector[i][j][2] * 1.0);




			glEnd();
		}
	}
	glFlush();
	glutSwapBuffers();

}

void draw_total(int _circle_number = 36, int _point_number = 18) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int i = 0; i < _circle_number; i++)
	{
		for (int j = 0; j < _point_number; j++)
		{

				glColor3f(0.f, 0.f, 1.f); // �Ķ��� ����
				
			glBegin(GL_QUAD_STRIP); // mode ����
			glNormal3fv(point_nomal_vector[i][j]);
			glTexCoord2d(i / 36.0, j / 18.0);
			glVertex3fv(p[i][j]);

	

			glNormal3fv(point_nomal_vector[(i + 1) % 36][((j + 1) % 18)]);
			glTexCoord2d((i + 1) / 36.0, (j + 1) / 18.0);

			glVertex3fv(p[(i + 1) % 36][((j + 1) % 18)]);

			glNormal3fv(point_nomal_vector[i][((j + 1) % 18)]);
			glTexCoord2d(i / 36.0, (j + 1) / 18.0);

			glVertex3fv(p[i][((j + 1) % 18)]);

			glNormal3fv(point_nomal_vector[(i + 1) % 36][(j)]);
			glTexCoord2d((i + 1) / 36.0, j / 18.0);

			glVertex3fv(p[(i + 1) % 36][(j)]);

		}
	}
	glEnd();
	////�׵θ� �׸���
	
	//glFlush();
	//glutSwapBuffers();
}

void renderScene()
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	draw_clean();
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, my_position);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texID);

	draw_total(circle_number, point_number);

	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q' | 'Q':
		exit(0);
		break;
	case VK_ESCAPE:
		exit(0);
		break;
	case '1':
		texID = texIDs[0];

		break;
	case '2':
		texID = texIDs[1];

		break;
	case '3':
		texID = texIDs[2];

		break;
	
	default:
		break;
	}
}

void SetupRC(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void ChangeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, (GLfloat)w / h, 0.1f, 1000.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
void timer(int value)
{

	glutPostRedisplay();
	glutTimerFunc(1000 / 30, timer, 1);//4�ʿ� 120��
	//glutTimerFunc(1000 / 10, cut_off_timer, 1);//0.1�ʿ� 1�� -> 4�ʿ� 40��
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // ���÷��̸�� ���� // �� �̱۷� �ؾߵ���
	glutInitWindowSize(500, 500);
	glutCreateWindow("Drawing the torus");
	glutInitWindowPosition(0, 0);
	glClearColor(255.0, 255.0, 255.0, 0.0); //alpha?
	Draw_point();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keyboard);
	SetupRC();
	//glMatrixMode(GL_PROJECTION); //�̰� ���� ������ �ǹ��ϴ°ɱ�
	//glLoadIdentity();
	//glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

	//glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(1000 / 30, timer, 1);//�⺻ Ÿ�̸� ����

	glutMainLoop();
}