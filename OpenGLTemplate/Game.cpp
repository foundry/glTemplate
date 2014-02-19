/* 
OpenGL Template for INM376 / IN3005
City University London, School of Informatics
Source code drawn from a number of sources and examples, including contributions from
 - Ben Humphrey (gametutorials.com), Michal Bubner (mbsoftworks.sk), Christophe Riccio (glm.g-truc.net)
 - Christy Quinn, Sam Kellett and others

 For educational use by School of Informatics, City University London UK.

 This template contains a skybox, simple terrain, camera, lighting, shaders, texturing

 Potential ways to modify the code:  Add new geometry types, shaders, change the terrain, load new meshes, change the lighting, 
 different camera controls, different shaders, etc.
 
 Template version 2.0a 31/01/2014
 Dr. Greg Slabaugh (gregory.slabaugh.1@city.ac.uk) 
*/


#include "game.h"
#include "Utils.h"

#ifdef WIN32
// Setup includes
#include "HighResolutionTimer.h"
#include "GameWindow.h"
#include "FreeTypeFont.h"

#endif

// Game includes
#include "Camera.h"
#include "Skybox.h"
#include "Plane.h"
#include "Shaders.h"
#include "Sphere.h"
#include "MatrixStack.h"
#include "OpenAssetImportMesh.h"
#include "Grid.h"

// Constructor
Game::Game()
{
	m_pSkybox = NULL;
	m_pCamera = NULL;
	m_pShaderPrograms = NULL;
	m_pSphere = NULL;
	m_pGrid = NULL;
     m_elapsedTime = 0;
	 m_levels = 5;
	m_dt = 0.0;
	m_iFramesPerSecond = 0;
#ifdef _WIN32
    m_pHighResolutionTimer = NULL;
	m_pFtFont = NULL;
#endif
}

// Destructor
Game::~Game() 
{ 
	//game objects
	delete m_pCamera;
	delete m_pSkybox;
	delete m_pSphere;
	delete m_pGrid;

	if (m_pShaderPrograms != NULL) {
		for (unsigned int i = 0; i < m_pShaderPrograms->size(); i++)
			delete (*m_pShaderPrograms)[i];
	}
	delete m_pShaderPrograms;

	//setup objects
#ifdef _WIN32
	delete m_pHighResolutionTimer;
    delete m_pFtFont;
#endif
}

// Initialisation:  This method only runs once at startup
void Game::Initialise() 
{
	// Set the clear colour and depth
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);

	/// Create objects
	m_pCamera = new CCamera;
	m_pSkybox = new CSkybox;
	m_pShaderPrograms = new vector <CShaderProgram *>;
    m_pSphere = new CSphere;
	m_pGrid = new CGrid;
#ifdef _WIN32
	m_pFtFont = new CFreeTypeFont;
	RECT dimensions = m_gameWindow.GetDimensions();
	m_width = dimensions.right - dimensions.left;
	m_height = dimensions.bottom - dimensions.top;
#endif

	// Set the orthographic and perspective projection matrices based on the image size
	m_pCamera->SetOrthographicProjectionMatrix(m_width, m_height);
	m_pCamera->SetPerspectiveProjectionMatrix(45.0f, (float) m_width / (float) m_height, 0.5f, 5000.0f);

	// Load shaders
	vector<CShader> shShaders;
	vector<string> sShaderFileNames;
	sShaderFileNames.push_back("mainShader.vert");
	sShaderFileNames.push_back("mainShader.frag");
	sShaderFileNames.push_back("textShader.vert");
	sShaderFileNames.push_back("textShader.frag");
	sShaderFileNames.push_back("sphereShader.vert");
	sShaderFileNames.push_back("sphereShader.frag");

	for (int i = 0; i < (int) sShaderFileNames.size(); i++) {
		string sExt = sShaderFileNames[i].substr((int) sShaderFileNames[i].size()-4, 4);
		int iShaderType;
		if (sExt == "vert") iShaderType = GL_VERTEX_SHADER;
		else if (sExt == "frag") iShaderType = GL_FRAGMENT_SHADER;
		else if (sExt == "geom") iShaderType = GL_GEOMETRY_SHADER;
		else if (sExt == "tcnl") iShaderType = GL_TESS_CONTROL_SHADER;
		else iShaderType = GL_TESS_EVALUATION_SHADER;
		CShader shader;
        std::string shaderFile = resourcePath(sShaderFileNames[i], "shaders", "");
		shader.LoadShader(shaderFile, iShaderType);
		shShaders.push_back(shader);
	}

	// Create the main shader program
	CShaderProgram *pMainProgram = new CShaderProgram;
	pMainProgram->CreateProgram();
	pMainProgram->AddShaderToProgram(&shShaders[0]);
	pMainProgram->AddShaderToProgram(&shShaders[1]);
	pMainProgram->LinkProgram();
	m_pShaderPrograms->push_back(pMainProgram);
	// Create a shader program for fonts
	CShaderProgram *pFontProgram = new CShaderProgram;
	pFontProgram->CreateProgram();
	pFontProgram->AddShaderToProgram(&shShaders[2]);
	pFontProgram->AddShaderToProgram(&shShaders[3]);
	pFontProgram->LinkProgram();
	m_pShaderPrograms->push_back(pFontProgram);
#ifdef _WIN32

    m_pFtFont->LoadSystemFont("arial.ttf", 32);
	m_pFtFont->SetShaderProgram(pFontProgram);
#endif
	// Create the sphere shader program
	CShaderProgram *pSphereProgram = new CShaderProgram;
	pSphereProgram->CreateProgram();
	pSphereProgram->AddShaderToProgram(&shShaders[4]);
	pSphereProgram->AddShaderToProgram(&shShaders[5]);
	pSphereProgram->LinkProgram();
	m_pShaderPrograms->push_back(pSphereProgram);

	// Create a sphere
    std::string textureDir = resourcePath("", "textures", "");

	m_pSphere->Create(textureDir, "dirtpile01.jpg", 35, 35);  // Texture downloaded from http://www.psionicgames.com/?page_id=26 on 24 Jan 2013
	glEnable(GL_CULL_FACE);
	
	// Create the grid
	m_pGrid->Create(500, 500, 15);

}

// Render method runs repeatedly in a loop
void Game::Render() 
{
	
	// Clear the buffers and enable depth testing (z-buffering)
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Set up a matrix stack
	glutil::MatrixStack modelViewMatrixStack;
	modelViewMatrixStack.SetIdentity();
	modelViewMatrixStack.LookAt(m_pCamera->GetPosition(), m_pCamera->GetView(), m_pCamera->GetUpVector());


	// Use the main shader program 
	CShaderProgram *pMainProgram = (*m_pShaderPrograms)[0];
	pMainProgram->UseProgram();

	// Set light and materials in main shader program
	glm::vec4 vPosition(-100, 100, 50, 1);
	glm::mat3 normalMatrix = m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top());
	glm::vec4 vLightEye = modelViewMatrixStack.Top()*vPosition;
	pMainProgram->SetUniform("light1.position", vLightEye);		// Position of light source in eye coordinates
	pMainProgram->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pMainProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pMainProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pMainProgram->SetUniform("material1.Ma", glm::vec3(0.5f, 0.5f, 0.5f));
	pMainProgram->SetUniform("material1.Md", glm::vec3(0.5f, 0.5f, 0.5f));
	pMainProgram->SetUniform("material1.Ms", glm::vec3(0.5f, 0.5f, 0.5f));
	pMainProgram->SetUniform("material1.shininess", 15.0f);
	pMainProgram->SetUniform("bUseTexture", false);
	pMainProgram->SetUniform("sampler0", 0);


	// Render the grid
	modelViewMatrixStack.Push();
		pMainProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pMainProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pMainProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		m_pGrid->Render();
	modelViewMatrixStack.Pop();

	
	// Switch to the sphere program
	CShaderProgram *pSphereProgram = (*m_pShaderPrograms)[2];
	pSphereProgram->UseProgram();

	// Set light and materials in sphere programme
	pSphereProgram->SetUniform("material1.Ma", glm::vec3(0.0f, 1.0f, 0.0f));
	pSphereProgram->SetUniform("material1.Md", glm::vec3(0.0f, 1.0f, 0.0f));
	pSphereProgram->SetUniform("material1.Ms", glm::vec3(1.0f, 1.0f, 1.0f));
	pSphereProgram->SetUniform("material1.shininess", 50.0f);
	pSphereProgram->SetUniform("light1.La", glm::vec3(0.15f, 0.15f, 0.15f));
	pSphereProgram->SetUniform("light1.Ld", glm::vec3(1.0f, 1.0f, 1.0f));
	pSphereProgram->SetUniform("light1.Ls", glm::vec3(1.0f, 1.0f, 1.0f));
	pSphereProgram->SetUniform("light1.position", vLightEye); 

	// Render the sphere
	modelViewMatrixStack.Push();
		modelViewMatrixStack.Translate(glm::vec3(0.0f, 5.0f, 50.0f));
		modelViewMatrixStack.Scale(5.0f);
		pSphereProgram->SetUniform("matrices.projMatrix", m_pCamera->GetPerspectiveProjectionMatrix());
		pSphereProgram->SetUniform("matrices.modelViewMatrix", modelViewMatrixStack.Top());
		pSphereProgram->SetUniform("matrices.normalMatrix", m_pCamera->ComputeNormalMatrix(modelViewMatrixStack.Top()));
		
		pSphereProgram->SetUniform("t",m_elapsedTime);
			
		pSphereProgram->SetUniform("levels",m_levels);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_pSphere->Render();
	modelViewMatrixStack.Pop();

#ifdef __WIN32
    void swapBuffers();
#endif
#ifdef __APPLE__
    glSwapAPPLE();
#endif

}



void Game::Update()
{
	// Update the camera using the amount of time that has elapsed to avoid framerate dependent motion
	m_pCamera->Update(m_dt);
	m_elapsedTime += (float)(0.01f * m_dt);
}




Game& Game::GetInstance()
{
	static Game instance;

	return instance;
}


#pragma mark - interacitivy


void Game::keyPress (KeyType key) {
    switch (key) {
        case (KeyUpArrow):
            m_levels += 1;
            if (m_levels > 255) m_levels = 255;
            break;
        case (KeyDownArrow):
            m_levels -= 1;
            if (m_levels < 2) m_levels =2;
            break;
        case (KeyLeftArrow):
            ;
            break;
        case (KeyRightArrow):
            ;
            break;
        default:
            break;
    }
}

#pragma mark - platform-specific run loops


#ifdef __APPLE__

void Game::SetSize (int width, int height) {
    m_width = width;
    m_height = height;
}

void Game::InitialiseWithSize(int width, int height) {
    SetSize(width, height);
    Initialise();
}

void Game::UpdateWithTime(double time)
{
    m_dt = time;
    Update();
    Render();
}



void Game::ProcessKeyDownEvent(int keyDownEvent) {
    if (keyDownEvent == 123) {
        keyPress(KeyRightArrow);
    }
    else if (keyDownEvent == 124) {
        keyPress(KeyLeftArrow);
    }
    else if (keyDownEvent == 126) {
        keyPress(KeyUpArrow);
           }
    else if (keyDownEvent == 125) {
        keyPress(KeyDownArrow);

    }

}
#endif

#ifdef _WIN2



void Game::DisplayFrameRate()
{
	static int frameCount = 0;
	static double elapsedTime = 0.0f;

	CShaderProgram *fontProgram = (*m_pShaderPrograms)[1];

	RECT dimensions = m_gameWindow.GetDimensions();
	int height = dimensions.bottom - dimensions.top;

	// Increase the elapsed time and frame counter
	elapsedTime += m_dt;
    frameCount++;

	// Now we want to subtract the current time by the last time that was stored
	// to see if the time elapsed has been over a second, which means we found our FPS.
    if(elapsedTime > 1000 )
    {
		elapsedTime = 0;
		m_iFramesPerSecond = frameCount;

		// Reset the frames per second
        frameCount = 0;
    }

	if (m_iFramesPerSecond > 0) {
		// Use the font shader program and render the text
		fontProgram->UseProgram();
		glDisable(GL_DEPTH_TEST);
		fontProgram->SetUniform("matrices.modelViewMatrix", glm::mat4(1));
		fontProgram->SetUniform("matrices.projMatrix", m_pCamera->GetOrthographicProjectionMatrix());
		fontProgram->SetUniform("vColour", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_pFtFont->Render(20, height - 20, 20, "FPS: %d", m_iFramesPerSecond);
	}
    
    
}


// The game loop runs repeatedly until game over
void Game::GameLoop()
{
	/*
	// Fixed timer
	dDt = pHighResolutionTimer->Elapsed();
	if (dDt > 1000.0 / (double) Game::FPS) {
		pHighResolutionTimer->Start();
		Update();
		Render();
	}
	*/
	
	
	// Variable timer
	m_pHighResolutionTimer->Start();
	Update();
	Render();
	m_dt = m_pHighResolutionTimer->Elapsed();
	

}


WPARAM Game::Execute() 
{
	m_pHighResolutionTimer = new CHighResolutionTimer;
	m_gameWindow.Init(m_hHinstance);

	if(!m_gameWindow.Hdc()) {
		return 1;
	}

	Initialise();

	m_pHighResolutionTimer->Start();

	
	MSG msg;

	while(1) {													
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { 
			if(msg.message == WM_QUIT) {
				break;
			}

			TranslateMessage(&msg);	
			DispatchMessage(&msg);
		} else if (m_bAppActive) {
			GameLoop();
		} 
		else Sleep(200); // Do not consume processor power if application isn't active
	}

	m_gameWindow.Deinit();

	return(msg.wParam);
}

LRESULT Game::ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param) 
{
	LRESULT result = 0;

	switch (message) {


	case WM_ACTIVATE:
	{
		switch(LOWORD(w_param))
		{
			case WA_ACTIVE:
			case WA_CLICKACTIVE:
				m_bAppActive = true;
				m_pHighResolutionTimer->Start();
				break;
			case WA_INACTIVE:
				m_bAppActive = false;
				break;
		}
		break;
		}

	case WM_SIZE:
			RECT dimensions;
			GetClientRect(window, &dimensions);
			m_gameWindow.SetDimensions(dimensions);
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(window, &ps);
		EndPaint(window, &ps);
		break;

        case WM_KEYDOWN:
            switch(w_param) {
                case VK_ESCAPE:
                    PostQuitMessage(0);
                    break;
                case VK_RIGHT:
                    keyPress(KeyRightArrow);
                    break;
                case VK_LEFT:
                    keyPress(KeyLeftArrow);
                    break;
                case VK_UP:
                    keyPress(KeyUpArrow);
                    break;
                case VK_DOWN:
                    keyPress(KeyDownArrow);
                    break;
                    
            }
            break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		result = DefWindowProc(window, message, w_param, l_param);
		break;
	}

	return result;
}


void Game::SetHinstance(HINSTANCE hinstance) 
{
	m_hHinstance = hinstance;
}

LRESULT CALLBACK WinProc(HWND window, UINT message, WPARAM w_param, LPARAM l_param)
{
	return Game::GetInstance().ProcessEvents(window, message, w_param, l_param);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int) 
{
	Game &game = Game::GetInstance();
	game.SetHinstance(hinstance);

	return game.Execute();
}
#endif
