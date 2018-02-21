#include <Game.h>

static bool flip;

Game::Game() : window(VideoMode(800, 600), "OpenGL Cube Vertex and Fragment Shaders")
{
}

Game::~Game() {}

void Game::run()
{

	initialize();

	Event event;

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				transitionFactorY += 0.01;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				transitionFactorY -= 0.01;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				transitionFactorX -= 0.01;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				transitionFactorX += 0.01;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				rotationAngleY -= 0.5f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				rotationAngleY += 0.5f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				rotationAngleX -= 0.5f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				rotationAngleX += 0.5f;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			{
				scaleFactor += 0.1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			{
				scaleFactor -= 0.1;
			}
		}
		update();
		render();
	}

}

typedef struct
{
	float coordinate[3];
	float color[4];
} Vertex;

Vertex vertex[36];
GLubyte triangles[36];

/* Variable to hold the VBO identifier and shader data */
GLuint	index, //Index to draw
		vsid, //Vertex Shader ID
		fsid, //Fragment Shader ID
		progID, //Program ID
		vao = 0, //Vertex Array ID
		vbo[1], // Vertex Buffer ID
		positionID, //Position ID
		colorID; // Color ID


void Game::initialize()
{
	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	glewInit();

	/* Vertices counter-clockwise winding */
	initializeFrontBackFaces();
	initializeTopBottomFaces();
	initializeLeftRightFaces();

	/*Index of Poly / Triangle to Draw */
	for (int i = 0; i < 36; i++)
	{
		triangles[i] = i;

		m_startPoints[i].x = vertex[i].coordinate[0];
		m_startPoints[i].y = vertex[i].coordinate[1];
		m_startPoints[i].z = vertex[i].coordinate[2];
	}

	/* Create a new VBO using VBO id */
	glGenBuffers(1, vbo);

	/* Bind the VBO */
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	/* Upload vertex data to GPU */
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &index);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLubyte) * 36, triangles, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Vertex Shader which would normally be loaded from an external file */
	const char* vs_src = "#version 400\n\r"
		"in vec4 sv_position;"
		"in vec4 sv_color;"
		"out vec4 color;"
		"void main() {"
		"	color = sv_color;"
		"	gl_Position = sv_position;"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER); //Create Shader and set ID
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL); // Set the shaders source
	glCompileShader(vsid); //Check that the shader compiles

	//Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	/* Fragment Shader which would normally be loaded from an external file */
	const char* fs_src = "#version 400\n\r"
		"in vec4 color;"
		"out vec4 fColor;"
		"void main() {"
		"	fColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
		"}"; //Fragment Shader Src

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src, NULL);
	glCompileShader(fsid);
	//Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();	//Create program in GPU
	glAttachShader(progID, vsid); //Attach Vertex Shader to Program
	glAttachShader(progID, fsid); //Attach Fragment Shader to Program
	glLinkProgram(progID);

	//Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader Link Error");
	}

	// Use Progam on GPU
	// https://www.opengl.org/sdk/docs/man/html/glUseProgram.xhtml
	glUseProgram(progID);

	// Find variables in the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml
	positionID = glGetAttribLocation(progID, "sv_position");
	colorID = glGetAttribLocation(progID, "sv_color");
}

void Game::update()
{
	elapsed = clock.getElapsedTime();

	if (elapsed.asSeconds() >= 1.0f)
	{
		clock.restart();
	}

	m_rotationXMatrix = m_rotationXMatrix.RotationX(rotationAngleY);
	m_rotationYMatrix = m_rotationYMatrix.RotationY(rotationAngleX);

	m_scaleMatrix = m_scaleMatrix.Scale3D(scaleFactor);

	m_translationMatrix = m_translationMatrix.Translate(transitionFactorX, transitionFactorY);

	Vector3D m_translationVector = Vector3D(transitionFactorX, transitionFactorY, 0);
	for (int i = 0; i < 36; i++)
	{
		m_currentPoints[i] = m_startPoints[i];
		m_currentPoints[i] = m_rotationXMatrix * m_currentPoints[i];
		m_currentPoints[i] = m_rotationYMatrix * m_currentPoints[i];
		m_currentPoints[i] = m_scaleMatrix * m_currentPoints[i];
		m_currentPoints[i] = m_translationMatrix * m_currentPoints[i];
		
		m_currentPoints[i] = m_currentPoints[i] + m_translationVector;

		vertex[i].coordinate[0] = m_currentPoints[i].x;
		vertex[i].coordinate[1] = m_currentPoints[i].y;
		vertex[i].coordinate[2] = m_currentPoints[i].z;
	}

	//Change vertex data
	/*vertex[0].coordinate[0] += -0.0001f;
	vertex[0].coordinate[1] += -0.0001f;
	vertex[0].coordinate[2] += -0.0001f;

	vertex[1].coordinate[0] += -0.0001f;
	vertex[1].coordinate[1] += -0.0001f;
	vertex[1].coordinate[2] += -0.0001f;

	vertex[2].coordinate[0] += -0.0001f;
	vertex[2].coordinate[1] += -0.0001f;
	vertex[2].coordinate[2] += -0.0001f;*/

#if (DEBUG >= 2)
	DEBUG_MSG("Update up...");
#endif

}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Drawing...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index);

	/*	As the data positions will be updated by the this program on the
		CPU bind the updated data to the GPU for drawing	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 36, vertex, GL_STATIC_DRAW);
	/*	Draw Triangle from VBO	(set where to start from as VBO can contain
		model components that 'are' and 'are not' to be drawn )	*/

	// Set pointers for each parameter
	// https://www.opengl.org/sdk/docs/man4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	//Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (char*)NULL + 0);

	window.display();

}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDeleteProgram(progID);
	glDeleteBuffers(1, vbo);
}


void Game::initializeFrontBackFaces()
{
	vertex[0].coordinate[0] = -0.5f;
	vertex[0].coordinate[1] = -0.5f;
	vertex[0].coordinate[2] = -0.5f;

	vertex[1].coordinate[0] = -0.5f;
	vertex[1].coordinate[1] = 0.5f;
	vertex[1].coordinate[2] = -0.5f;

	vertex[2].coordinate[0] = 0.5f;
	vertex[2].coordinate[1] = 0.5f;
	vertex[2].coordinate[2] = -0.5f;

	vertex[3].coordinate[0] = 0.5f;
	vertex[3].coordinate[1] = 0.5f;
	vertex[3].coordinate[2] = -0.5f;

	vertex[4].coordinate[0] = 0.5f;
	vertex[4].coordinate[1] = -0.5f;
	vertex[4].coordinate[2] = -0.5f;

	vertex[5].coordinate[0] = -0.5f;
	vertex[5].coordinate[1] = -0.5f;
	vertex[5].coordinate[2] = -0.5f;

	vertex[6].coordinate[0] = -0.5f;
	vertex[6].coordinate[1] = -0.5f;
	vertex[6].coordinate[2] = 0.5f;

	vertex[7].coordinate[0] = -0.5f;
	vertex[7].coordinate[1] = 0.5f;
	vertex[7].coordinate[2] = 0.5f;

	vertex[8].coordinate[0] = 0.5f;
	vertex[8].coordinate[1] = 0.5f;
	vertex[8].coordinate[2] = 0.5f;

	vertex[9].coordinate[0] = 0.5f;
	vertex[9].coordinate[1] = 0.5f;
	vertex[9].coordinate[2] = 0.5f;

	vertex[10].coordinate[0] = 0.5f;
	vertex[10].coordinate[1] = -0.5f;
	vertex[10].coordinate[2] = 0.5f;

	vertex[11].coordinate[0] = -0.5f;
	vertex[11].coordinate[1] = -0.5f;
	vertex[11].coordinate[2] = 0.5f;

	vertex[0].color[0] = 0.1f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 0.0f;
	vertex[0].color[3] = 1.0f;

	vertex[1].color[0] = 0.2f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 0.0f;
	vertex[1].color[3] = 1.0f;

	vertex[2].color[0] = 0.3f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 0.0f;
	vertex[3].color[3] = 1.0f;

	vertex[3].color[0] = 0.4f;
	vertex[3].color[1] = 1.0f;
	vertex[3].color[2] = 0.0f;

	vertex[4].color[0] = 0.5f;
	vertex[4].color[1] = 1.0f;
	vertex[4].color[2] = 0.0f;

	vertex[5].color[0] = 0.6f;
	vertex[5].color[1] = 1.0f;
	vertex[5].color[2] = 0.0f;

	vertex[6].color[0] = 1.0f;
	vertex[6].color[1] = 0.1f;
	vertex[6].color[2] = 0.0f;

	vertex[7].color[0] = 1.0f;
	vertex[7].color[1] = 0.2f;
	vertex[7].color[2] = 0.0f;

	vertex[8].color[0] = 1.0f;
	vertex[8].color[1] = 0.3f;
	vertex[8].color[2] = 0.0f;

	vertex[9].color[0] = 1.0f;
	vertex[9].color[1] = 0.4f;
	vertex[9].color[2] = 0.0f;

	vertex[10].color[0] = 1.0f;
	vertex[10].color[1] = 0.5f;
	vertex[10].color[2] = 0.0f;

	vertex[11].color[0] = 1.0f;
	vertex[11].color[1] = 0.6f;
	vertex[11].color[2] = 0.0f;
}

void Game::initializeTopBottomFaces()
{
	vertex[12].coordinate[0] = -0.5f;
	vertex[12].coordinate[1] = 0.5f;
	vertex[12].coordinate[2] = 0.5f;

	vertex[13].coordinate[0] = -0.5f;
	vertex[13].coordinate[1] = 0.5f;
	vertex[13].coordinate[2] = -0.5f;

	vertex[14].coordinate[0] = 0.5f;
	vertex[14].coordinate[1] = 0.5f;
	vertex[14].coordinate[2] = -0.5f;

	vertex[15].coordinate[0] = 0.5f;
	vertex[15].coordinate[1] = 0.5f;
	vertex[15].coordinate[2] = -0.5f;

	vertex[16].coordinate[0] = 0.5f;
	vertex[16].coordinate[1] = 0.5f;
	vertex[16].coordinate[2] = 0.5f;

	vertex[17].coordinate[0] = -0.5f;
	vertex[17].coordinate[1] = 0.5f;
	vertex[17].coordinate[2] = 0.5f;

	vertex[18].coordinate[0] = -0.5f;
	vertex[18].coordinate[1] = -0.5f;
	vertex[18].coordinate[2] = 0.5f;

	vertex[19].coordinate[0] = -0.5f;
	vertex[19].coordinate[1] = -0.5f;
	vertex[19].coordinate[2] = -0.5f;

	vertex[20].coordinate[0] = 0.5f;
	vertex[20].coordinate[1] = -0.5f;
	vertex[20].coordinate[2] = -0.5f;

	vertex[21].coordinate[0] = 0.5f;
	vertex[21].coordinate[1] = -0.5f;
	vertex[21].coordinate[2] = -0.5f;

	vertex[22].coordinate[0] = 0.5f;
	vertex[22].coordinate[1] = -0.5f;
	vertex[22].coordinate[2] = 0.5f;

	vertex[23].coordinate[0] = -0.5f;
	vertex[23].coordinate[1] = -0.5f;
	vertex[23].coordinate[2] = 0.5f;

	vertex[12].color[0] = 0.1f;
	vertex[12].color[1] = 1.0f;
	vertex[12].color[2] = 0.0f;

	vertex[13].color[0] = 0.2f;
	vertex[13].color[1] = 1.0f;
	vertex[13].color[2] = 0.0f;

	vertex[14].color[0] = 0.3f;
	vertex[14].color[1] = 1.0f;
	vertex[14].color[2] = 0.0f;

	vertex[15].color[0] = 0.4f;
	vertex[15].color[1] = 1.0f;
	vertex[15].color[2] = 0.0f;

	vertex[16].color[0] = 0.5f;
	vertex[16].color[1] = 1.0f;
	vertex[16].color[2] = 0.0f;

	vertex[17].color[0] = 0.6f;
	vertex[17].color[1] = 1.0f;
	vertex[17].color[2] = 0.0f;

	vertex[18].color[0] = 1.0f;
	vertex[18].color[1] = 0.1f;
	vertex[18].color[2] = 0.0f;

	vertex[19].color[0] = 1.0f;
	vertex[19].color[1] = 0.2f;
	vertex[19].color[2] = 0.0f;

	vertex[20].color[0] = 1.0f;
	vertex[20].color[1] = 0.3f;
	vertex[20].color[2] = 0.0f;

	vertex[21].color[0] = 1.0f;
	vertex[21].color[1] = 0.4f;
	vertex[21].color[2] = 0.0f;

	vertex[22].color[0] = 1.0f;
	vertex[22].color[1] = 0.5f;
	vertex[22].color[2] = 0.0f;

	vertex[23].color[0] = 1.0f;
	vertex[23].color[1] = 0.6f;
	vertex[23].color[2] = 0.0f;
}

void Game::initializeLeftRightFaces()
{
	vertex[24].coordinate[0] = -0.5f;
	vertex[24].coordinate[1] = -0.5f;
	vertex[24].coordinate[2] = -0.5f;

	vertex[25].coordinate[0] = -0.5f;
	vertex[25].coordinate[1] = 0.5f;
	vertex[25].coordinate[2] = -0.5f;

	vertex[26].coordinate[0] = -0.5f;
	vertex[26].coordinate[1] = 0.5f;
	vertex[26].coordinate[2] = 0.5f;

	vertex[27].coordinate[0] = -0.5f;
	vertex[27].coordinate[1] = 0.5f;
	vertex[27].coordinate[2] = 0.5f;

	vertex[28].coordinate[0] = -0.5f;
	vertex[28].coordinate[1] = -0.5f;
	vertex[28].coordinate[2] = 0.5f;

	vertex[29].coordinate[0] = -0.5f;
	vertex[29].coordinate[1] = -0.5f;
	vertex[29].coordinate[2] = -0.5f;

	vertex[30].coordinate[0] = 0.5f;
	vertex[30].coordinate[1] = -0.5f;
	vertex[30].coordinate[2] = -0.5f;

	vertex[31].coordinate[0] = 0.5f;
	vertex[31].coordinate[1] = 0.5f;
	vertex[31].coordinate[2] = -0.5f;

	vertex[32].coordinate[0] = 0.5f;
	vertex[32].coordinate[1] = 0.5f;
	vertex[32].coordinate[2] = 0.5f;

	vertex[33].coordinate[0] = 0.5f;
	vertex[33].coordinate[1] = 0.5f;
	vertex[33].coordinate[2] = 0.5f;

	vertex[34].coordinate[0] = 0.5f;
	vertex[34].coordinate[1] = -0.5f;
	vertex[34].coordinate[2] = 0.5f;

	vertex[35].coordinate[0] = 0.5f;
	vertex[35].coordinate[1] = -0.5f;
	vertex[35].coordinate[2] = -0.5f;

	vertex[24].color[0] = 0.1f;
	vertex[24].color[1] = 1.0f;
	vertex[24].color[2] = 0.0f;

	vertex[25].color[0] = 0.2f;
	vertex[25].color[1] = 1.0f;
	vertex[25].color[2] = 0.0f;

	vertex[26].color[0] = 0.3f;
	vertex[26].color[1] = 1.0f;
	vertex[26].color[2] = 0.0f;

	vertex[27].color[0] = 0.4f;
	vertex[27].color[1] = 1.0f;
	vertex[27].color[2] = 0.0f;

	vertex[28].color[0] = 0.5f;
	vertex[28].color[1] = 1.0f;
	vertex[28].color[2] = 0.0f;

	vertex[29].color[0] = 0.6f;
	vertex[29].color[1] = 1.0f;
	vertex[29].color[2] = 0.0f;

	vertex[30].color[0] = 1.0f;
	vertex[30].color[1] = 0.1f;
	vertex[30].color[2] = 1.0f;

	vertex[31].color[0] = 1.0f;
	vertex[31].color[1] = 0.2f;
	vertex[31].color[2] = 1.0f;

	vertex[32].color[0] = 1.0f;
	vertex[32].color[1] = 0.3f;
	vertex[32].color[2] = 1.0f;

	vertex[33].color[0] = 1.0f;
	vertex[33].color[1] = 0.4f;
	vertex[33].color[2] = 1.0f;

	vertex[34].color[0] = 1.0f;
	vertex[34].color[1] = 0.5f;
	vertex[34].color[2] = 1.0f;

	vertex[35].color[0] = 1.0f;
	vertex[35].color[1] = 0.6f;
	vertex[35].color[2] = 1.0f;
}