#pragma once

#include "Common.h"
#if defined (WIN32)
#include "GameWindow.h"
#endif
// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to NULL and in Game::Initialise, create a new object.  Don't forget to 
// delete the object in the destructor.

typedef enum {
    KeyUpArrow,
    KeyDownArrow,
    KeyLeftArrow,
    KeyRightArrow
} KeyType;

class CCamera;
class CSkybox;
class CShader;
class CShaderProgram;
class CPlane;
#ifdef WIN32
class CFreeTypeFont;
class CHighResolutionTimer;
#endif
class CSphere;
class COpenAssetImportMesh;
class CGrid;



class Game {
private:


	// Pointers to game objects.  They will get allocated in Game::Initialise()
	CSkybox *m_pSkybox;
	CCamera *m_pCamera;
	vector <CShaderProgram *> *m_pShaderPrograms;
#if defined (WIN32)
	CFreeTypeFont *m_pFtFont;
    CHighResolutionTimer *m_pHighResolutionTimer;
#endif	
    CSphere *m_pSphere;
	CGrid *m_pGrid;

	// Some other member variables
	double m_dt;
	int m_iFramesPerSecond;
	bool m_bAppActive;
	float m_elapsedTime;
	int m_levels;
    int m_width;
    int m_height;


#ifdef __APPLE__

#endif


public:
	Game();
	~Game();
	static Game& GetInstance();

    // Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void Initialise();
	void Update();
	void Render();
    void keyPress (KeyType);

#ifdef _WIIN32
    //Windows interactiviy
	LRESULT ProcessEvents(HWND window,UINT message, WPARAM w_param, LPARAM l_param);
	void SetHinstance(HINSTANCE hinstance);
	WPARAM Execute();
#endif

#ifdef __APPLE__
    //OSX interactivity
	void ProcessKeyDownEvent(int keyDownEvent);
    void UpdateWithTime(double time);
    void SetSize (int width, int height);
    void InitialiseWithSize(int width, int height);
#endif

private:

void SwapBuffers();


#ifdef __WIN32
    static const int FPS = 60;
	void DisplayFrameRate();
	void GameLoop();
	GameWindow m_gameWindow;
	HINSTANCE m_hHinstance;
#endif

};
