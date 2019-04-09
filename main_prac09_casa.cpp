/*---------------------------------------------------------*/
/*---------------------------------------------------------------*/
/*----------- Alumna: Galindo Chavez Lisset America -------------*/
/*-----------------  Version: Visual Studio 2017 ----------------*/
/* ---------------------   Práctica 9  --------------------------*/
/*-----------------  Instrucciones de uso -----------------------*/
/*--------     Tecla     ----     Movimiento   -------------------*/
/*--------       D       ----      Derecha     -------------------*/
/*--------       A       ----     Izquierda    -------------------*/
/*--------       W       ----      Aleja       -------------------*/
/*--------       S       ----      Acerca      -------------------*/
/*--------    Mouse      ----  Rota la camara  -------------------*/
/*--------       I       ----    Directional   -------------------*/
/*--------       O       ----    Positional    -------------------*/
/*--------       P       ----    Spotlight     -------------------*/
/*--------       R       ----  Aumenta rojo    -------------------*/
/*--------       G       ----  Aumenta verde   -------------------*/
/*--------       B       ----  Aumenta azul    -------------------*/
/*--------       T       ----Aumenta intensidad-------------------*/
/*-------- Shift Izq + T ---- Resta intensidad -------------------*/
/*-------------- Fecha de entrega: 08/04/2019 -------------------*/
/*-----------------       2019-2      ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/
//textures.com
//crear normal map online. Normalmap-online
#define STB_IMAGE_IMPLEMENTATION
#include "esfera.h"
#include "camera.h"
#include "stb_image.h"
#include "string.h"
//using namespace std;

Esfera my_sphere(1.0f);

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
double	lastX = 0.0f,
		lastY = 0.0f;
bool firstMouse = true;

//Timing
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 3.0f, 0.0f); //Posicion de la luz
glm::vec3 lightDirection(0.0f, 0.0f, -3.0f);//Hacia a donde va a apuntar
glm::vec3 lightaux = lightDirection;
glm::vec3 lightaux2;

void myData(void);
void display(void);
void getResolution(void);
void animate(void);
void LoadTextures(void);
unsigned int generateTextures(char*, bool);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -5.0f,
		rotX = 0.0f,
		intensidad = 1.0f,
		red = 0.2f,
		green = 0.2f,
		blue = 0.2f;

//Texture
unsigned int	t_pasto,
				t_toalla,
				t_unam,
				t_pared;

int aux = 0;

char	shadervs[37] = "shaders/shader_texture_light_dir.vs",
		shaderfs[37] = "shaders/shader_texture_light_dir.fs",
		lightname[] = "light.direction",
		lightname2[] = "light.position",
		lightname3[] = "light.cutOff";

float lightaux3 = glm::cos(glm::radians(12.5f));


unsigned int generateTextures(const char* filename, bool alfa)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (alfa)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		return textureID;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return 100;
	}

	stbi_image_free(data);
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;

	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;

}

void LoadTextures()
{

	t_unam = generateTextures("Texturas/Central.png", 1);
	t_pasto = generateTextures("Texturas/pasto.jpg", 0);
	t_pared = generateTextures("Texturas/bricks.jpg", 0);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, t_unam);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, t_pasto);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, t_pared);
}

void myData()
{	
	float vertices[] = {
		// positions          // normals           // texture coords
		//ATRAS
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.865f,  0.025f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f,  0.025f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.5f,  0.84f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.865f,  0.84f,
		//ENFRENTE
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  0.025f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.365f,  0.025f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.365f,  0.84f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  0.0f,  0.84f,
		//IZQUIERDA
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  1.0f,  0.99f,  0.84f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  1.0f,  0.865f,  0.84f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  1.0f,  0.865f,  0.025f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  1.0f,  0.99f,  0.025f,
		//DERECHA
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.365f,  0.84f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.84f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.025f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.365f,  0.025f,
		 //ABAJO
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		//ARRIBA
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,


		//////Segunda caja

		//ATRAS
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.734f,  0.84f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.62f,  0.84f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.62f,  0.995f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.734f,  0.995f,
		//ENFRENTE
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.122f,  0.84f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.235f,  0.84f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.235f,  0.995f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.122f,  0.995f,
		//IZQUIERDA
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.864f, 0.99f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.91f,  0.99f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.91f,  0.84f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.864f,  0.84f,

		//DERECHA
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.4519f,  0.995f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  1.0f,  0.4911f,  0.995f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  1.0f,  0.4911f,  0.84f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  1.0f,  0.4519f,  0.84F,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,

		/*//Cara de atrás
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f,  0.0f, -1.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		0.0f,  0.0f, -1.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		0.0f,  0.0f, -1.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.0f,  0.0f, -1.0f,//Top Left

		//Cara de enfrente
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		0.0f,  0.0f, 1.0f,//Bottom Left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		0.0f,  0.0f, 1.0f,//Bottom Right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		0.0f,  0.0f, 1.0f,//Top Right
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		0.0f,  0.0f, 1.0f,//Top Left

		//Cara de la izquierda
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		-1.0f,  0.0f, 0.0f,//Bottom Left
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,		-1.0f,  0.0f, 0.0f,//Bottom Right
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,		-1.0f,  0.0f, 0.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		-1.0f,  0.0f, 0.0f,//Top Left

		//Cara de la derecha
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,		1.0f,  0.0f, 0.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,		1.0f,  0.0f, 0.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		1.0f,  0.0f, 0.0f,//Top Right
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,		1.0f,  0.0f, 0.0f,//Top Left

		//Cara de abajo
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,		0.0f,  -1.0f, 0.0f,//Bottom Left
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,		0.0f,  -1.0f, 0.0f,//Bottom Right
		0.5f, -0.5f,  0.5f,  1.0f,  1.0f,		0.0f,  -1.0f, 0.0f,//Top Right
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,		0.0f,  -1.0f, 0.0f,//Top Left

		//Cara de arriba
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,		0.0f,  1.0f, 0.0f,//Bottom Left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,		0.0f,  1.0f, 0.0f,//Bottom Right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,		0.0f,  1.0f, 0.0f,//Top Right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,		0.0f,  1.0f, 0.0f,//Top Left*/
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void animate(void)
{

}

void display(void)
{
	//Shader projectionShader("shaders/shader_light.vs", "shaders/shader_light.fs");
	//Shader projectionShader("shaders/shader_texture_color.vs", "shaders/shader_texture_color.fs");
	
	Shader lightingShader(shadervs, shaderfs); 
	lightingShader.use();

	Shader lampShader("shaders/shader_lamp.vs", "shaders/shader_lamp.fs");

	//To Use Lighting

	//If the light is Directional, we send the direction of the light
	//lightingShader.setVec3("light.direction", lightDirection);	
	//If the light is Positional, we send the position of the light
	//lightingShader.setVec3("light.position", lightPosition);
	//If the light is Spotlight, we put the light in the camera
	/*lightingShader.setVec3("light.position", camera.Position);
	lightingShader.setVec3("light.direction", camera.Front);
	lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	*/
	lightingShader.setVec3("light.position", camera.Position);
	lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
	lightingShader.setVec3(lightname, lightaux);
	
	lightingShader.setVec3("viewPos", camera.Position);

	// light properties
	lightingShader.setVec3("light.ambient", red, green, blue);//CAMBIA EL COLOR DE LA BIBLIOTECA
	lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
	lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	//For Positional and Spotlight
	lightingShader.setFloat("light.constant", 1.0f);
	lightingShader.setFloat("light.linear", 0.09f);
	lightingShader.setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader.setFloat("material_shininess", 32.0f);

	// create transformations and Projection
	glm::mat4 temp = glm::mat4(1.0f);
	glm::mat4 temp2 = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" to include Camera
	projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	// pass them to the shaders
	//lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setMat4("model", model);
	lightingShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	lightingShader.setMat4("projection", projection);


	glBindVertexArray(VAO);
	//Colocar código aquí
	lightingShader.setVec3("ambientColor", 0.0f, 0.0f, 0.0f);
	lightingShader.setVec3("diffuseColor", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("specularColor", 1.0f, 0.0f, 0.0f);
	lightingShader.setInt("material_diffuse", t_unam);
	//lightingShader.setInt("material_specular", t_caja_brillo);
	model = glm::scale(model, glm::vec3(3.0f, 4.0f, 2.0f));
	lightingShader.setInt("intensidad", intensidad);

	lightingShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 16);

	lightingShader.setInt("material_diffuse", t_pasto);
	glDrawArrays(GL_QUADS, 16, 4);

	lightingShader.setInt("material_diffuse", t_pared);
	glDrawArrays(GL_QUADS, 20, 4);

	//Parte de arriba
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.25f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 0.5f));
	lightingShader.setMat4("model", model);
	lightingShader.setInt("material_diffuse", t_unam);
	glDrawArrays(GL_QUADS, 24, 16);


	lightingShader.setInt("material_diffuse", t_pared);
	glDrawArrays(GL_QUADS, 40, 8);

	//Light
	lampShader.use();
	lampShader.setMat4("projection", projection);
	lampShader.setMat4("view", view);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPosition);
	model = glm::scale(model, glm::vec3(0.25f));
	lampShader.setMat4("model", model);

	//glBindVertexArray(lightVAO);
	glDrawArrays(GL_QUADS, 0, 24);	//Light

	
	glBindVertexArray(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 9", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//To Enable capture of our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	LoadTextures();
	myData();
	my_sphere.init();
	glEnable(GL_DEPTH_TEST);

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		// --------------------
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
        // input
        // -----
        my_input(window);
		animate();

        // render
        // Backgound color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	
	//Directional
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		strcpy_s(shadervs, "shaders/shader_texture_light_dir.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_dir.fs");
		strcpy_s(lightname, "light.direction");
		lightaux = lightDirection;
	}
	//Positional
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		strcpy_s(shadervs, "shaders/shader_texture_light_pos.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_pos.fs");
		strcpy_s(lightname, "light.position");
		lightaux = lightPosition;
	}
	//Spotlight
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		strcpy_s(shadervs, "shaders/shader_texture_light_spot.vs");
		strcpy_s(shaderfs, "shaders/shader_texture_light_spot.fs");
		strcpy_s(lightname, "light.direction");
		lightaux = camera.Front; 
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		if (red < 1)
		{
			red += 0.001f;
		}
		else
		{
			red = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (green < 1)
		{
			green += 0.001f;
		}
		else
		{
			green = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		if (blue < 1)
		{
			blue += 0.001f;
		}
		else
			blue = 0.0f;
	}

	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) {
			if (intensidad > -3)
			{
				intensidad -= 0.01f;
			}
			else
			{
				intensidad = 1.0f;
			}
		}
		else
		{
			if (intensidad < 5)
			{
				intensidad += 0.01f;
			}
			else
			{
				intensidad = 1.0f;
			}
		}
	}


}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}