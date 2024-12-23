/*Chương trình chiếu sáng Blinn-Phong (Phong sua doi) cho hình lập phương đơn vị, điều khiển quay bằng phím x, y, z, X, Y, Z.*/

#include "Angel.h"  /* Angel.h là file tự phát triển (tác giả Prof. Angel), có chứa cả khai báo includes glew và freeglut*/
#define PI 3.1415927

// remember to prototype
void generateGeometry(void);
void initGPUBuffers(void);
void shaderSetup(void);
void display(void);
void keyboard(unsigned char key, int x, int y);

typedef vec4 point4;
typedef vec4 color4;
using namespace std;


// Số các đỉnh của các tam giác
const int NumPoints = 36;

point4 points[NumPoints]; /* Danh sách các đỉnh của các tam giác cần vẽ*/
color4 colors[NumPoints]; /* Danh sách các màu tương ứng cho các đỉnh trên*/
vec3 normals[NumPoints]; /*Danh sách các vector pháp tuyến ứng với mỗi đỉnh*/

point4 vertices[8]; /* Danh sách 8 đỉnh của hình lập phương*/
color4 vertex_colors[8]; /*Danh sách các màu tương ứng cho 8 đỉnh hình lập phương*/

GLuint program;

GLfloat ttxebus = 0, aa=1.6, xx=0, yy=0;
GLfloat theta[] = { 0,0,0 };
GLfloat alpha[] = { 0,0,0 };
GLfloat alpha3 = 0, alpha4 = 0, alpha5 = 0, alpha6 = 0, alpha1 = 0, alpha2 = 0, alpha7 = 0;
GLfloat zKeoTu = 0.0, ycuakinh = 0;

GLfloat busPositionX = 0.0;   // Vị trí X của xe buýt
GLfloat busSpeed = 0.01;      // Tốc độ di chuyển của xe buýt
int busDirection = 1;         // 1: Tiến lên, -1: Lùi lại

GLfloat carPositionX = 0.0;   // Vị trí X của xe ô tô con
GLfloat carSpeed = 0.02;      // Tốc độ di chuyển của xe ô tô con
int carDirection = 1;         // 1: Tiến lên, -1: Lùi lại

GLfloat truckPositionZ = 0.0;   // Vị trí X của xe tải
GLfloat truckSpeed = 0.02;      // Tốc độ di chuyển của xe tải
int truckDirection = 1;         // 1: Tiến lên, -1: Lùi lại

GLfloat craneRotationAngle = 0.0;  // Góc quay của trục cần cẩu
GLfloat craneRotationAngle1 = 0.0;  // Góc quay của trục cần cẩu


GLuint model_loc;
mat4 projection;
GLuint projection_loc;
mat4 view;
GLuint view_loc;

GLfloat l = -1.0, r = 1.0, t = 1.0, b = -1.0, zNear = 1, zFar = 100; //tham số góc nhìn của camera
GLfloat thetal = 0.0; //tham số góc quay camera 
GLfloat xEye = 0.0, yEye = 0.0, zEye = 0.0; // tham sô tịnh tiến camera

mat4 model, modeltaycua;
mat4 quayBase;


void initCube()
{
	// Gán giá trị tọa độ vị trí cho các đỉnh của hình lập phương
	vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
	vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
	vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
	vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
	vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
	vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
	vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
	vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

	// Gán giá trị màu sắc cho các đỉnh của hình lập phương	
	vertex_colors[0] = color4(0.0, 0.0, 0.0, 1.0); // black
	vertex_colors[1] = color4(1.0, 0.0, 0.0, 1.0); // red
	vertex_colors[2] = color4(1.0, 1.0, 0.0, 1.0); // yellow
	vertex_colors[3] = color4(0.0, 1.0, 0.0, 1.0); // green
	vertex_colors[4] = color4(0.0, 0.0, 1.0, 1.0); // blue
	vertex_colors[5] = color4(1.0, 0.0, 1.0, 1.0); // magenta
	vertex_colors[6] = color4(1.0, 0.5, 0.0, 1.0); // orange
	vertex_colors[7] = color4(0.0, 1.0, 1.0, 1.0); // cyan
}
int Index = 0;
void quad(int a, int b, int c, int d)  /*Tạo một mặt hình lập phương = 2 tam giác, gán màu cho mỗi đỉnh tương ứng trong mảng colors*/
{
	vec4 u = vertices[b] - vertices[a];
	vec4 v = vertices[c] - vertices[b];
	vec3 normal = normalize(cross(u, v));

	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
	normals[Index] = normal; colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}
void makeColorCube(void)  /* Sinh ra 12 tam giác: 36 đỉnh, 36 màu*/

{
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(3, 0, 4, 7);
	quad(6, 5, 1, 2);
	quad(4, 5, 6, 7);
	quad(5, 4, 0, 1);
}
void generateGeometry(void)
{
	initCube();
	makeColorCube();
}


void initGPUBuffers(void)
{
	// Tạo một VAO - vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Tạo và khởi tạo một buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(normals), normals);


}

/* Khởi tạo các tham số chiếu sáng - tô bóng*/
point4 light_position(-2.0, 2.0, 0.0, 0.0); // vị trí điểm sáng 
color4 light_ambient(0.2, 0.2, 0.2, 1.0);
color4 light_diffuse(1.0, 1.0, 1.0, 1.0);
color4 light_specular(1.0, 1.0, 1.0, 1.0); //sáng chói

color4 material_ambient(1.0, 0.0, 1.0, 1.0);
color4 material_diffuse(1.0, 0.8, 0.0, 1.0);
color4 material_specular(1.0, 0.8, 0.0, 1.0);
float material_shininess = 100.0;

color4 ambient_product = light_ambient * material_ambient * vec4(1.1, 1.1, 1.1, 1.0);
color4 diffuse_product = light_diffuse * material_diffuse * vec4(1.2, 1.2, 1.2, 1.0);
color4 specular_product = light_specular * material_specular * vec4(1.5, 1.5, 1.5, 1.0);


void shaderSetup(void)
{
	// Nạp các shader và sử dụng chương trình shader
	program = InitShader("vshader1.glsl", "fshader1.glsl");   // hàm InitShader khai báo trong Angel.h
	glUseProgram(program);

	// Khởi tạo thuộc tính vị trí đỉnh từ vertex shader
	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(points) + sizeof(colors)));

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, specular_product);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, light_position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), material_shininess);

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);        /* Thiết lập màu trắng là màu xóa màn hình*/
}
vec4 mauvang = vec4(1.0, 1.0, 0.0, 1.0);
vec4 mautuong = vec4(0.0, 1, 1, 0.5);
vec4 maubanghe = vec4(1.0, 0.0, 0.0, 1.0);
vec4 maubanghe2 = vec4(1.0, 0.6, 0.6, 1.0);
vec4 maukhungcuaso = vec4(1, 0.59, 0.12, 1.0);
vec4 maucanhcuaso = vec4(1, 0, 0.5, 0.6);
vec4 maungankeo = vec4(0.75, 1, 0.08, 0.6);
vec4 mauden;
vec4 mautu = vec4(0.8, 0.5, 0.5, 1.0);
vec4 maukim = vec4(0, 0, 0, 1);


void matPhang(GLfloat x, GLfloat y, GLfloat z, mat4 mt, vec4 color_of_obj) {

	material_diffuse = color_of_obj;
	diffuse_product = light_diffuse * material_diffuse;
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, diffuse_product);

	//camera
	point4 eye(0 + xEye, 1 + yEye, 4 + zEye, 1.0); // Camera position
	point4 at(-5 * sin(thetal) + xEye, 1 + yEye, -5 * cos(thetal) + zEye, 1.0); // Look-at point
	vec4 up(0, 1, 0, 1.0); // Up direction


	mat4 v = LookAt(eye, at, up); // camera
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, v);

	mat4 ins = Scale(x, y, z);   // scale
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, quayBase * mt * ins);

	mat4 p = Frustum(l, r, b, t, zNear, zFar);  // view volumn (vision)
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, p);
	glDrawArrays(GL_TRIANGLES, 0, NumPoints);
}

// Ngoi Nha 
void Wall_left() {
	model =  Translate(-3,1,0);
	matPhang(0.02, 2, 6, model, mautuong);
}
void Wall_Right() {
	model = Translate(3, 1, 0);
	matPhang(0.02, 2, 6, model, mautuong);
}
void Wall_Behind() {
	model = Translate(-2.2, 1, -3);
	matPhang(1.6, 2, 0.02, model, mautuong);

	model = Translate(2.2, 1, -3);
	matPhang(1.6, 2, 0.02, model, mautuong);

	model = Translate(0,1.75,-3 );
	matPhang(2.8, 0.5, 0.02, model, mautuong);

	model = Translate(0, 0.25, -3);
	matPhang(2.8, 0.5, 0.02, model, mautuong);

	model = Translate(0.5,1,-3);
	matPhang(0.2, 1, 0.04, model, mautuong);

	model = Translate(-0.5, 1, -3);
	matPhang(0.2, 1, 0.04, model, mautuong);
}
void Wall_inFontof(){
	model = Translate(-1.7 ,0.7 ,3) ;
	matPhang(2.6,1.4,0.02, model, mautuong);

	model = Translate(1.7, 0.7, 3);
	matPhang(2.6, 1.4, 0.02, model, mautuong);

	model = Translate(0, 1.7, 3);
	matPhang(6,0.6, 0.02, model, mautuong);
}

void DatNen()
{
	model = Translate(0, -0.01, 0);
	matPhang(25, 0.02, 25, model, vec4(1, 1, 0.79, 0.3));
}
void San_Nha() {

	model = Translate(0, 0, 0) ;
	matPhang(6, 0.04, 6, model, mautuong); 
}
void Tran_Nha() {
	model = Translate(0, 2, 0);
	matPhang(6, 0.04, 6, model, mautuong);

	model = Translate(0, 3.97, 0);
	matPhang(6, 0.04, 6, model, mautuong);
}
void cot() {
	model = Translate(2.9, 3, -2.9);
	matPhang(0.2, 2, 0.2, model, mautuong);

	model = Translate(2.9, 3, 2.9);
	matPhang(0.2, 2, 0.2, model, mautuong);

	model = Translate(-2.9, 3, -2.9);
	matPhang(0.2, 2, 0.2, model, mautuong);

	model = Translate(-2.9, 3, 2.9);
	matPhang(0.2, 2, 0.2, model, mautuong);
}

void taycamcuachinh()
{
	matPhang(0.05, 0.02, 0.05, model, mautuong);
}

void cuachinh() {

	model = Translate(-0.4, 0.7, 3) * RotateY(alpha5) * Translate(0.4, 0, 0);
	matPhang(0.8, 1.4, 0.02, model, vec4(0.53, 0.46, 0.82, 0.5));

	model = model* Translate(0.3, 0, 0.03) * RotateZ(alpha6);
	taycamcuachinh();
}

void donggach() {
	// phần đế
	model = Translate(-3, 0.2, 4);
	matPhang(1.15, 0.1, 1.15, model, vec4(0.57, 0.56, 0.05, 0.3));

	// phần chân
	model = Translate(-3.4, 0.15, 4.4);
	matPhang(0.2, 0.2, 0.2, model, vec4(0.57, 0.56, 0.05, 0.3));
	model = Translate(-3.4, 0.15, 3.6);
	matPhang(0.2, 0.2, 0.2, model, vec4(0.57, 0.56, 0.05, 0.3));
	model = Translate(-2.6, 0.15, 3.6);
	matPhang(0.2, 0.2, 0.2, model, vec4(0.57, 0.56, 0.05, 0.3));
	model = Translate(-2.6, 0.15, 4.4);
	matPhang(0.2, 0.2, 0.2, model, vec4(0.57, 0.56, 0.05, 0.3));

	// gạch
	model = Translate(-3.25, 0.35, 4.25);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.54, 0.05, 0.3));
	model = Translate(-3.25, 0.35, 3.75);
	matPhang(0.5, 0.2, 0.5, model, vec4(01, 0.83, 0.5, 0.3));
	model = Translate(-2.75, 0.35, 4.25);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.54, 0.05, 0.3));
	model = Translate(-2.75, 0.35, 3.75);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.83, 0.5, 0.3));
	//
	model = Translate(-3.25, 0.55, 4.25);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.83, 0.5, 0.3));
	model = Translate(-3.25, 0.55, 3.75);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.54, 0.05, 0.3));
	model = Translate(-2.75, 0.55, 4.25);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.83, 0.5, 0.3));
	model = Translate(-2.75, 0.55, 3.75);
	matPhang(0.5, 0.2, 0.5, model, vec4(1, 0.54, 0.05, 0.3));
}

//cửa sổ 
void canh_cua1() {
	model = Translate(0, 0.5, 0) * RotateX(alpha[0]) * Translate(0, -0.5, 0);
	matPhang(0.8, 1, 0.04, model, maucanhcuaso);
}
void canh_cua2() {
	model = Translate(-1.4, 0, 0) * RotateY(alpha[0]) * Translate(0.4, 0, 0);
	matPhang(0.8, 1, 0.04, model, maucanhcuaso);
}
void canh_cua3() {
	model = Translate(1.4, 0, 0) * RotateY(alpha[1]) * Translate(-0.4, 0, 0);
	matPhang(0.8, 1, 0.04, model, maucanhcuaso);
}
void windown() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Scale(0.8, 1.1, 0.8) * Translate(0, 1, -6);
	canh_cua1();
	canh_cua2();
	canh_cua3();
}

void Nha() { 
	quayBase = RotateY(theta[0]) * RotateX(theta[1])*Scale(0.8,1.1,0.8)*Translate(0,0,-3);
	DatNen();
	San_Nha();
	Tran_Nha();
	Wall_left();
	Wall_Right();
	Wall_Behind();
	Wall_inFontof();
	cot();
	donggach();
	
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Scale(0.8, 1.1, 0.8) * Translate(0, 0, -3);
	cuachinh();
	windown();
}


//Đường, vỉa hè, đèn đường
void landuong1() {
	// mat duong 
	model = Translate(0, 0, 5);
	matPhang(8, 0.04, 2.2, model, maukim);
	
	// vach ke duong 
	model = Translate(3.5, 0.035, 5);
	matPhang(1, 0.01, 0.1, model, mauvang);

	model = Translate(2, 0.035, 5);
	matPhang(1, 0.01, 0.1, model, mauvang);

	model = Translate(0.5, 0.035, 5);
	matPhang(1, 0.01, 0.1, model, mauvang);

	model = Translate(-1, 0.035, 5);
	matPhang(1, 0.01, 0.1, model, mauvang);

	model = Translate(-2.5, 0.035, 5);
	matPhang(1, 0.01, 0.1, model, mauvang);

	model = Translate(-3.8, 0.035, 5);
	matPhang(0.4, 0.01, 0.1, model, mauvang);
}
void Viahe()
{
	model = Translate(0, 0, 6.6);
	matPhang(8, 0.2, 1, model, vec4(1, 0.83, 0.5, 0.3));
}
void denduong() {

	for (int i = 0; i < 40; i++) {
		// phần đế đèn
		model = Translate(0, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));
		model = Translate(1, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));
		model = Translate(2, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));
		model = Translate(-1, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));
		model = Translate(-2, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));
		model = Translate(-3, 0.5, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.4, 0.06, model, vec4(1, 0.83, 0.5, 0.3));

		model = Translate(0, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);
		model = Translate(1, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);
		model = Translate(2, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);
		model = Translate(-1, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);
		model = Translate(-2, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);
		model = Translate(-3, 2.52, 6.65) * RotateY(3 * i);
		matPhang(0.08, 0.02, 0.08, model, maukim);

		// phần đèn
		model = Translate(0, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);
		model = Translate(1, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);
		model = Translate(2, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);
		model = Translate(-1, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);
		model = Translate(-2, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);
		model = Translate(-3, 2.62, 6.65) * RotateY(3 * i);
		matPhang(0.06, 0.2, 0.06, model, mauden);

	}

	model = Translate(0, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
	model = Translate(1, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
	model = Translate(2, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
	model = Translate(-1, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
	model = Translate(-2, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
	model = Translate(-3, 1.6, 6.65);
	matPhang(0.03, 1.8, 0.03, model, maukim);
}

void landuong() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(0, 0, -5.5) * Scale(4, 1.0, 1.7);
	landuong1();
	Viahe();
	denduong();
}


//Mô phỏng cần cẩu
void thancancau() {

	for (int j = 0;j < 40;j++) {
		model = Translate(5, 2.75, 2) * RotateY(3+ j);
		matPhang(0.4, 5.5, 0.4, model, vec4(1.0, 1.0, 0, 0.6));
	}
}

void cabin() {
	// phan de
	model = Translate(5, 5.6, 2);
	matPhang(1.2, 0.2, 1.4, model, vec4(0.42, 0.45, 0.44, 0.5));

	// phan ghe lai
	model = Translate(4.9, 6.2, 2.4) ;
	matPhang(1.2, 1, 0.6, model, vec4(0.42, 0.44, 0.55, 0.5));

	// bo may 
	model = Translate(5, 6.2, 1.5);
	matPhang(1.1, 0.8, 0.8, model, vec4(0.85, 0.71, 0.69, 0.6));
	
	// phan duoi cau
	model = Translate(6.9, 5.9, 1.8);
	matPhang(0.5, 1.2, 0.5, model, vec4(0.71, 0.71, 0.69, 0.6));
	
	// phan canh tay can cau
	model = Translate(3.15, 5.9, 1.8);
	matPhang(7.2, 0.2, 0.4, model, vec4(0.84, 0.75, 0, 0.3));

}

void palang() {
 
	for (int i = 0; i < 100; i++)
	{
		model = Translate(1.2+xx, 5.0+yy , 2.0)* RotateY(2 + i);
		matPhang(0.06, aa, 0.06, model, vec4(0.26, 0.24, 0.27, 0.4));

		model = Translate(1.2+xx, 5.0+yy , 1.6) * RotateY(2 + i);
		matPhang(0.06, aa, 0.06, model, vec4(0.26, 0.24, 0.27, 0.4));

		model = Translate(1.0 + xx, 5.0+yy , 1.6) * RotateY(2 + i);
		matPhang(0.06, aa, 0.06, model, vec4(0.26, 0.24, 0.27, 0.4));

		model = Translate(1.0 + xx, 5.0+yy , 2.0) * RotateY(2 + i);
		matPhang(0.06, aa, 0.06, model, vec4(0.26, 0.24, 0.27, 0.4));

	}

	//Vật liệu
	model = Translate(1 + xx, 3.4 + 2 * yy, 1.8) * RotateY(30);
	matPhang(0.2, 0.2, 1.4, model, vec4(0.81, 0.57, 0, 0.4));

	model = Translate(1.2 + xx, 3.4 + 2 * yy, 1.8) * RotateY(30);
	matPhang(0.2, 0.2, 1.4, model, vec4(0.81, 0.57, 0, 0.4));

	model = Translate(1.1 + xx, 3.6 + 2 * yy, 1.8) * RotateY(30);
	matPhang(0.2, 0.2, 1.4, model, vec4(0.81, 0.57, 0, 0.4));
}
void xeconcanhtay() {
	//Móc cẩu
	for (int i = 0; i < 100; i++)
	{
		model = Translate(1.1 + xx, 4.2 + 2* yy, 1.8) * RotateZ(3 + i);
		matPhang(0.2, 0.2, 0.4, model, vec4(0.91, 0.84, 0.07, 0.5));

	}
	model = Translate(1.2 + xx, 3.7 + 2 * yy, 1.6) * RotateZ(20);
	matPhang(0.06, 0.8, 0.06, model, maukim);

	model = Translate(1.2 + xx, 3.7 + 2 * yy, 2.0) * RotateZ(20);
	matPhang(0.06, 0.8, 0.06, model, maukim);

	model = Translate(0.9 + xx, 3.7 + 2 * yy, 1.6) * RotateZ(-20);
	matPhang(0.06, 0.8, 0.06, model, maukim);

	model = Translate(0.9 + xx, 3.7 + 2 * yy, 2.0) * RotateZ(-20);
	matPhang(0.06, 0.8, 0.06, model, maukim);

}

void cancau() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(2, 0, -3);
	thancancau();

	quayBase = quayBase * Translate(5, 5.6, 2) * RotateY(craneRotationAngle) * Translate(-5, -5.6, -2);
	cabin();
	palang();
	xeconcanhtay();
}


//mô phỏng xe buýt 
void dauxe() {
	model = Translate(0, 1.2, 5);
	matPhang(1.2, 0.1, 1.8, model, maubanghe2);

	model = Translate(0, 3.0, 5);
	matPhang(1.2, 0.1, 1.8, model, maubanghe2);

	model = Translate(-0.6, 1.35, 5);
	matPhang(0.08, 0.4, 1.8, model, maubanghe2);

	model = Translate(-0.6, 2.3, 5);
	matPhang(0.08, 1.6, 1.8, model, maubanghe);

	model = Translate(0.6, 2.1, 5);
	matPhang(0.08, 1.91, 1.8, model, maubanghe);
}
void gam_xe1() {
	model = Translate(1.5, -1.4, -1.9);
	matPhang(1.8, 0.5, 3.0, model, vec4(1.0, 0.75, 1.0, 0));
 }

void banhxe(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	model = Translate(a, b, c) * RotateX(d); 
	matPhang(0.2, 0.6, 0.6, model, vec4(0.58, 0.51, 0.5, 0.3));

}
void den_xe(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	model = Translate(a, b, c) * RotateZ(d); 
	matPhang(0.15, 0.15, 0.1, model, vec4(1, 0.6, 0, 0));
}

void thanh_doc() {
	model = Translate(2.3, -0.15, -0.48) ; 
	matPhang(0.15, 2.0, 0.15, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(0.7, -0.15, -0.48);
	matPhang(0.15, 2.0, 0.15, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(0.65, -0.7, -2.0);
	matPhang(0.1, 1.0, 2.8, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(2.35, -0.7, -2.4);
	matPhang(0.1, 1.0, 2.0, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(1.45, -0.15, -3.35);
	matPhang(1.7, 2.0, 0.1, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(1.45, -0.75, -0.55);
	matPhang(1.7, 0.8, 0.3, model, vec4(1.0, 0.75, 1.0, 0));

	model = Translate(1.45, -0.34, -0.56) * RotateX(30);
	matPhang(1.65, 0.17, 0.27, model, vec4(1.0, 0.75, 1.0, 0));
}

void thanh_cua(GLfloat a, GLfloat b, GLfloat c)
{
	model = Translate(a, b, c);
	matPhang(0.1, 1.0, 0.1, model, vec4(0, 0, 0, 0));
}

void maixe() {
	model = Translate(1.5, 0.85, -1.9);
	matPhang(1.8, 0.05, 3.0, model, vec4(1.5, 0.75, 0, 0));

}

void cuaxe() {
	model = Translate(2.3, -0.15, -0.98);
	matPhang(0.15, 2.0, 0.88, model, vec4(1.0, 0.75, 1.0, 0));
}

void xebuyt() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(busPositionX, 0, 0) * RotateY(-90) * Translate(3, 1.3, 4) * Scale(0.5, 0.6, 1.7);
	gam_xe1();
	thanh_doc();
	for (int j = 0; j < 20; j++) {
		banhxe(2.5, -1.6, -1.2, (6 * j));
		banhxe(2.5, -1.6, -2.8, (6 * j));
		banhxe(0.5, -1.6, -1.2, (6 * j));
		banhxe(0.5, -1.6, -2.8, (6 * j));
		den_xe(0.9, -1.4, -0.4, (6 * j));
		den_xe(2.2, -1.4, -0.4, (6 * j));

		thanh_cua(2.35, 0.3, -1.5);
		thanh_cua(2.35, 0.3, -2.0);
		thanh_cua(2.35, 0.3, -2.5);
		thanh_cua(2.35, 0.3, -3.0);

		thanh_cua(0.7, 0.3, -1.5);
		thanh_cua(0.7, 0.3, -2.0);
		thanh_cua(0.7, 0.3, -2.5);
		thanh_cua(0.7, 0.3, -3.0);
	}
	maixe();
	quayBase = quayBase * Translate(2.3, -0.15, -0.54 + ttxebus) * RotateY(alpha7) * Translate(-2.3, 0.15, 0.54 - ttxebus);
	cuaxe();
}


//mô phỏng xe con
void gam_xe2() {
	model = Translate(3.8, -1.4, -1.9);
	matPhang(1, 0.5, 3.0, model, vec4(1.0, 0.75, 1.0, 0));
}

void banhxe1(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	model = Translate(a, b, c) * RotateZ(d);
	matPhang(0.5, 0.5, 0.2, model, vec4(0.58, 0.51, 0.5, 0.3));
}

void thanxe() {
	model = Translate(1.3, -1.2, -1.5);
	matPhang(1.5, 1.3, 1, model, vec4(1, 0.2, 0, 0));

	model = Translate(-0.2, -1.5, -1.5);
	matPhang(1.5, 0.7, 1, model, vec4(1, 0.2, 0, 0));

	model = Translate(0.6, -1.18, -1.5);
	matPhang(1.5, 0.9, 0.99, model, vec4(1, 0.2, 0, 0));
}

void cuaso() {
	model = Translate(0.7, -0.9, -0.99);
	matPhang(0.4, 0.4, 0.01, model, vec4(1, 1, 1, 0));

	model = Translate(0.7, -0.9, -2.01);
	matPhang(0.4, 0.4, 0.01, model, vec4(1, 1, 1, 0));
}

void xecon() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(carPositionX, 0, 0) * RotateY(-180) * Scale(0.9, 0.9, 0.9) * Translate(7, 2.0, -1.0);
	thanxe();
	cuaso();
	for (int i = 0; i < 20; i++) {
		banhxe1(0.1, -1.65, -0.95, (6 * i));
		banhxe1(0.1, -1.65, -2.05, (6 * i));
		banhxe1(1.7, -1.65, -0.95, (6 * i));
		banhxe1(1.7, -1.65, -2.05, (6 * i));
	}
}


//Tủ đồ
void than_tu() {
	// backgr
	model = Translate(0, 0, -0.2);
	matPhang(1.2, 1.4, 0.02, model, maukhungcuaso);

	//thanh tren
	model = Translate(0, 0.7, 0);
	matPhang(1.2, 0.02, 0.4, model, maukhungcuaso);

	//thanh duoi
	model = Translate(0, -0.7, 0);
	matPhang(1.2, 0.02, 0.4, model, maukhungcuaso);

	//thanh trai
	model = Translate(-0.6, 0, 0);
	matPhang(0.02, 1.4, 0.4, model, maukhungcuaso);

	//thanh phai
	model = Translate(0.6, 0, 0);
	matPhang(0.02, 1.4, 0.4, model, maukhungcuaso);

	//trục dọc  
	model = Translate(0, 0, 0);
	matPhang(0.02, 1.4, 0.4, model, maukhungcuaso);

	//trục ngang  
	model = Translate(0, 0, 0);
	matPhang(1.2, 0.02, 0.4, model, maukhungcuaso);

	//trục ngang  
	model = Translate(0, -0.35, 0);
	matPhang(1.2, 0.02, 0.4, model, maukhungcuaso);

	//trục ngang  
	model = Translate(0, 0.35, 0);
	matPhang(1.2, 0.02, 0.4, model, maukhungcuaso);
}
void tu_do() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(2.18, 0.72, -4) * Scale(0.8, 1.1, 0.8) * RotateY(-90);
	than_tu();
}


//bàn làm việc 
void chanban() {
	//chan 1
	model = Translate(-0.58, 0, -0.28);
	matPhang(0.04, 0.6, 0.04, model, maubanghe);
	//chân 2
	model = Translate(0.58, -0, -0.28);
	matPhang(0.04, 0.6, 0.04, model, maubanghe);
	//chân 3
	model = Translate(-0.58, -0, 0.28);
	matPhang(0.04, 0.6, 0.04, model, maubanghe);
	//chân 4
	model = Translate(0.58, -0, 0.28);
	matPhang(0.04, 0.6, 0.04, model, maubanghe);

}
void matban() {
	model = Translate(0, 0.3, 0);
	matPhang(1.2, 0.02, 0.6, model, maubanghe);
}
void ngankeo() {
	model = Translate(0, 0, zKeoTu) * Translate(-0.36, 0.2, 0);
	matPhang(0.4, 0.2, 0.6, model, maungankeo);

	// tay nắm ngăn kéo 
	model = Translate(0, 0, zKeoTu) * Translate(-0.36, 0.24, 0.32);
	matPhang(0.04, 0.04, 0.04, model, mautuong);

}

void the_table() {
	quayBase = RotateY(theta[0]) * RotateX(theta[1]) * Translate(1.98, 0.3, -2.5) * Scale(0.8, 1.1, 0.8) * RotateY(-90);
	chanban();
	matban();
	ngankeo();
}


// mô phỏng xe tải
void gamxe() {
	// thanh gam tren
	model = Translate(0,1.2, 0);
	matPhang(4.4, 0.2, 0.8, model, vec4(0.55, 0.55, 0.55, 0.3));

	// truc banh xe
	model = Translate(1.8, 0.75, 0);
	matPhang(0.15, 0.15, 0.9, model, vec4(0.55, 0.55, 0.55, 0.3));
	model = Translate(1.0, 0.75, 0);
	matPhang(0.15, 0.15, 0.9, model, vec4(0.55, 0.55, 0.55, 0.3));
	model = Translate(-1.8, 0.75, 0);
	matPhang(0.15, 0.15, 0.9, model, vec4(0.55, 0.55, 0.55, 0.3));
	
	// lo xo xe
	model = Translate(-1.8, 1.0, 0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.16, 0.28, 0.27, 0.4));
	model = Translate(-1.8, 1.0, -0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.16, 0.28, 0.27, 0.4));
	model = Translate(1.8, 1.0, 0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.16, 0.28, 0.27, 0.4));
	model = Translate(1.8, 1.0, -0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.16, 0.28, 0.27, 0.4));

	model = Translate(1.0, 1.0, 0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.55, 0.55, 0.55, 0.3));
	model = Translate(1.0, 1.0, -0.35);
	matPhang(0.15, 0.5, 0.15, model, vec4(0.55, 0.55, 0.55, 0.3));
}

void banhxetai(GLfloat a, GLfloat b, GLfloat c, GLfloat d)
{
	model = Translate(a, b, c) * RotateZ(d);
	matPhang(0.5, 0.5, 0.2, model, vec4(0.16, 0, 0.13, 0.3));
}

void cabinxetai() {

	// mat truoc -> mat duoi->  tren -> sau 
	model = Translate(-2.1, 1.6, 0);
	matPhang(0.1, 0.5, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
	model = Translate(-2.1, 2.2, 0);
	matPhang(0.1, 0.8, 1.2, model, vec4(1, 1, 0.88, 0.3));
	model = Translate(-1.55, 1.3, 0);
	matPhang(1.2, 0.1, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
	model = Translate(-1.55, 2.6, 0);
	matPhang(1.2, 0.1, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
	model = Translate(-1.0, 1.95, 0);
	matPhang(0.1, 1.4, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
}

void cuaxetai() {

	model = Translate(-1.5, 1.6, 0.55)*Translate(-0.55,0,0)*RotateY(alpha1)*Translate(0.55,0,0);
	matPhang(1.1, 0.8, 0.1, model, vec4(0.58, 0.51, 0.5, 0.3));
	
	// phan kinh xe 
	model = Translate(-1.5, 2.25 + ycuakinh, 0.55)*Translate(-0.55, 0, 0) * RotateY(alpha1) * Translate(0.55, 0, 0);
	matPhang(1.1, 0.6, 0.1, model, vec4(1, 0.76, 0.93, 0.3));
	model = Translate(-1.5, 1.6, -0.55) * Translate(-0.55, 0, 0) * RotateY(alpha4) * Translate(0.55, 0, 0);
	matPhang(1.1, 0.8, 0.1, model, vec4(0.58, 0.51, 0.5, 0.3));
	model = Translate(-1.5, 2.25 + ycuakinh, -0.55) * Translate(-0.55, 0, 0) * RotateY(alpha4) * Translate(0.55, 0, 0);
	matPhang(1.1, 0.6, 0.1, model, vec4(1, 0.76, 0.93, 0.3));
}

void thungxe() {
	// truoc sau
	model = Translate(-0.6, 1.95, 0);
	matPhang(0.1, 1.4, 1.2, model, vec4(0.63, 0.76, 0.93, 0.3));
	model = Translate(2.4, 1.65, 0) * Translate(0, 0.4, 0) * RotateZ(alpha2) * Translate(0,-0.4, 0);
	matPhang(0.1, 0.8, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
	// tren
	model = Translate(-0.7, 2.6, 0);
	matPhang(0.2, 0.1, 1.2, model, vec4(0.87, 0.7, 0.45, 0.3));
	model = Translate(0.95, 1.3, 0);
	matPhang(3.2, 0.1, 1.2, model, vec4(0.63, 0.76, 0.93, 0.3));
	// canh
	model = Translate(0.95, 1.65, 0.55);
	matPhang(3.0, 0.8, 0.1, model, vec4(0.63, 0.76, 0.93, 0.3));
	model = Translate(0.95, 1.65, -0.55);
	matPhang(3.0, 0.8, 0.1, model, vec4(0.63, 0.76, 0.93, 0.3));
}

void xetai() {
	quayBase=RotateY(theta[0]) * Translate(0, 0, truckPositionZ) * RotateX(theta[1])*Translate(-5,-0.45,-1)*RotateY(90);
	gamxe();
	for (int i = 0;i < 60;i++) {

		banhxetai(-1.8, 0.75, 0.55, 6* i);
		banhxetai(-1.8, 0.75, -0.55, 6 * i);
		banhxetai(1.8, 0.75, 0.55, 6* i);
		banhxetai(1.8, 0.75, -0.55, 6 * i);
		banhxetai(1.0, 0.75, 0.55, 6 * i);
		banhxetai(1.0, 0.75, -0.55, 6 * i);
	}
	cabinxetai();
	cuaxetai();
	quayBase = quayBase * Translate(2.5, 1.3, 0) * RotateZ(alpha3) * Translate(-2.5, -1.3, 0);
	thungxe();
	
}


//--------------------------------------------------------
//tổng  hợp các thiết bị trong phòng 
void congtruong() {

	Nha();
	xebuyt();
	xetai();
	xecon();
	landuong();
	cancau();
	tu_do();
	the_table();
}
//--------------------------------------------------------

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	congtruong();
	
	glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
	// keyboard handler

	switch (key) {
	case 033:			// 033 is Escape key octal value
		exit(1);		// quit program
		break;

		//điều khiển camera
	case 'f': // xoay sang phải
		thetal -= 0.1;
		glutPostRedisplay();
		break;
	case 'g': // xoay sang trái
		thetal += 0.1;
		glutPostRedisplay();
		break;

	case 'c': // Di chuyển camera xuống dưới
		yEye -= 0.1;
		break;
	case 'v': // Di chuyển camera lên trên
		yEye += 0.1;
		break;

	case 'd': // Di chuyển camera sang phải
		xEye -= 0.1;
		break;
	case 'a': // Di chuyển camera sang trái
		xEye += 0.1;
		break;

	case 'w': //Di chuyển camera tiến lên
		zEye -= 0.1;
		break;
	case 's': //Di chuyển camera lùi xuống
		zEye += 0.1;
		break;

	case 'z': //xoay camera hướng lên
		theta[1] += 5;
		glutPostRedisplay();
		break;
	case 'x': //xoay camera hướng xuống
		theta[1] -= 5;
		glutPostRedisplay();
		break;

	//tăng giảm view volumn
	case 'e':
		zNear *= 1.05; zFar *= 1.05; //Giảm view volumn
		break;
	case 'r':
		zNear *= 0.95; zFar *= 0.95; //Tăng view volumn
		break;
	
	/*case 't': thetal += dr; break;
	case 'T': thetal -= dr; break;*/


	//Xe tải
	case '-': //mở cửa kính xe tải
		ycuakinh -= 0.1;
		if (ycuakinh < -0.4) ycuakinh = -0.4;
		glutPostRedisplay();
		break;
	case '=': //đóng cửa kính xe tải
		ycuakinh+=0.1;
		if (ycuakinh>0) ycuakinh = 0;
		glutPostRedisplay();
		break;

	case '7': //nâng thùng xe tải lên
		alpha3 -= 5;
		alpha2 += 5;
		if (alpha2 > 90) alpha2 = 90;
		if (alpha3 < -75) alpha3 = -75;
		glutPostRedisplay();
		break;
	case '8': //hạ thùng xe tải xuống
		alpha2 -= 5;
		alpha3 += 5;
		if (alpha2 <0) alpha2 = 0;
		if (alpha3 > 0 ) alpha3 = 0;
		glutPostRedisplay();
		break;

	case '9': //mở cửa xe tải
		alpha1 -= 5;
		if (alpha1 < -75) alpha1 = -75;
		glutPostRedisplay();
		break;
	case '0': //đóng cửa xe tải
		alpha1 += 5;
		if (alpha1 > 0) alpha1 = 0;
		glutPostRedisplay();
		break;

	//Nhà chính
	case 'o': // mở cửa sổ 
		alpha[0] += 5;
		alpha[1] -= 5;
		if (alpha[0] > 75) alpha[0] = 75;
		if (alpha[1] <-75) alpha[1] = -75;
		glutPostRedisplay();
		break;
	case 'p': // đóng cửa sổ 
		alpha[0] -= 5;
		alpha[1] += 5;
		if (alpha[0] < 0) alpha[0] = 0;
		if (alpha[1] >0) alpha[1] = 0;
		glutPostRedisplay();
		break;

	case 'u': // mở cửa chính 
		if (alpha6 > 90)
		{
			alpha5 += 5;

			if (alpha5 > 160) alpha5 = 160;
		}
		glutPostRedisplay();
		break;
	case 'i': // đóng cửa chính
		alpha5 -= 5;
		if (alpha5 < 0) alpha5 = 0;
		glutPostRedisplay();
		break;

	case 't': // tay mở cửa chính 
		alpha6 += 5;
		if (alpha6 > 180) alpha6 = 180;
		glutPostRedisplay();
		break;
	case 'y': // tay đóng cửa chính
		alpha6 -= 5;
		if (alpha6 < 0) alpha6 = 0;
		glutPostRedisplay();
		break;


	//Xe bus
	case '1': // mở cửa xe bus 
		alpha7 -= 5;
		if (alpha7 < -90) alpha7 = -90;
		glutPostRedisplay();
		break;
	case '2': // đóng cửa xe bus
		alpha7 += 5;
		if (alpha7 > 0) alpha7 = 0;
		glutPostRedisplay();
		break;


	//Đèn đường
	case '5': //bật đèn
		mauden = vec4(1, 1, 0.49, 1);
		break;
	case '6': //tắt đèn
		mauden = vec4(0, 0, 0, 1);
		break;


	//Cần cẩu
	case 'h': //di chuyen pa lang di xuong
		aa += 0.1;
		yy -= 0.05;
		break;
	case 'j': //di chuyen pa lang cau di len
		aa -= 0.1;
		yy += 0.05;
		break;

	case 'k': //di chuyen xe con canh tay vao trong
		xx += 0.1;
		if (xx >= 2.8) xx = 2.8;
		break;
	case 'l': //di chuyen xe con canh tay ra ngoai
		xx -= 0.1;
		if (xx <= -1.3) xx = -1.3;
		break;


	//Ngăn kéo bàn
	case '3': // mở ngăn kéo 
		zKeoTu += 0.1;
		if (zKeoTu > 0.5) zKeoTu = 0.5;
		if (zKeoTu < 0) zKeoTu = 0;
		glutPostRedisplay();
		break;
	case '4': // đóng ngăn kéo 
		zKeoTu -= 0.1;
		if (zKeoTu < 0) zKeoTu = 0;
		glutPostRedisplay();
		break;
	}
	
	glutPostRedisplay();	
}

//chuyển động của xe ô tô con
void updateCarPosition() {
	carPositionX += carSpeed * carDirection;

	// Điều kiện để xe đi ngược chiều khi đạt đến giới hạn
	if (carPositionX > 4.0) {   // Giới hạn bên phải
		carDirection = -1;  // Đổi hướng
	}
	else if (carPositionX < -4.0) {  // Giới hạn bên trái
		carDirection = 1;  // Đổi hướng ngược lại
	}

	glutPostRedisplay();  // Yêu cầu vẽ lại
}

//chuyển động của xe buýt
void updateBusPosition() {
	busPositionX += busSpeed * busDirection;

	// Đổi hướng khi đạt giới hạn
	if (busPositionX > 3.0) {    // Giới hạn trên
		busDirection = -1;       // Chuyển sang lùi
	}
	else if (busPositionX < -3.0) { // Giới hạn dưới
		busDirection = 1;        // Chuyển sang tiến
	}

	glutPostRedisplay();  // Cập nhật màn hình sau khi thay đổi vị trí
}

//chuyển động của xe tải
void updateTruckposition() {
	truckPositionZ += truckSpeed * truckDirection;

	// Đổi hướng khi đạt giới hạn
	if (truckPositionZ > 0.0) {    // Giới hạn trên
		truckDirection = -1;       // Chuyển sang lùi
	}
	else if (truckPositionZ < -6.0) { // Giới hạn dưới
		truckDirection = 1;        // Chuyển sang tiến
	}

	glutPostRedisplay();  // Cập nhật màn hình sau khi thay đổi vị trí
}

//Chuyển động của cần cẩu
void updateCraneRotation() {
	craneRotationAngle += 0.5;  // Tốc độ xoay

	if (craneRotationAngle > 360.0) {
		craneRotationAngle -= 360.0;  // Đặt lại góc khi vượt quá 360 độ
	}
	glutPostRedisplay();  // Yêu cầu vẽ lại sau khi cập nhật góc quay
}

void updateScene() {
	updateCarPosition();     // Cập nhật vị trí xe con
	updateBusPosition();     // Cập nhật vị trí xe buýt
	updateTruckposition();   // Cập nhật vị trí xe tải
	updateCraneRotation();   // Cập nhật góc quay cần cẩu
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(640, 640);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Working Room");

	glewInit();

	generateGeometry();
	initGPUBuffers();
	shaderSetup();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(updateScene);  // Cập nhật cả xe con, xe buýt và cần cẩu
	glutMainLoop();
	return 0;
}




 