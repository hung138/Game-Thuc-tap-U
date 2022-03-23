#include "GSPlay.h"

#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"
#include "Sprite2D.h"
#include "Sprite3D.h"
#include "Text.h"
#include "GameButton.h"
#include "SpriteAnimation.h"
using namespace std;

Vector2 toiNoi(0, 0);
float vt0 = Globals::screenWidth / 2 - 200;
bool diToi = false;
shared_ptr<GameButton> cardDi;

GSPlay::GSPlay()
{
}


GSPlay::~GSPlay()
{
}


void GSPlay::Init()
{
	m_Test = 1;
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto texture = ResourceManagers::GetInstance()->GetTexture("bg_play1.tga");

	// background
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_background->SetSize(Globals::screenWidth, Globals::screenHeight);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	std::shared_ptr<GameButton>  button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth - 50, 50);
	button->SetSize(50, 50);
	button->SetOnClick([this]() {
			GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

	// score
/*	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Brightly Crush Shine.otf");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TextColor::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));

	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("Actor1_2.tga");
	std::shared_ptr<SpriteAnimation> obj = std::make_shared<SpriteAnimation>(model, shader, texture, 9, 6, 5, 0.1f);
	
	obj->Set2DPosition(240, 400);
	obj->SetSize(334, 223);
	//obj->SetRotation(Vector3(0.0f, 3.14f, 0.0f));
	m_listAnimation.push_back(obj);*/



	// set deck
	texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	m_dek = std::make_shared<GameButton>(model, shader, texture);
	m_dek->Set2DPosition((float)Globals::screenWidth / 2 - 80, (float)Globals::screenHeight / 2);
	m_dek->SetSize(50, 70);
	m_dek->SetOnClick([this]() {
		cout << 5;
		if (!diToi) {
			Instantiate(Vector2(m_dek->GetPosition().x, m_dek->GetPosition().y));
			diToi = true;
		}
    });

	texture = ResourceManagers::GetInstance()->GetTexture("/0/v3.tga");
	m_card0 = std::make_shared<Sprite2D>(model, shader, texture);
	m_card0->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_card0->SetSize(50, 70);

	toiNoi = Vector2(vt0, Globals::screenHeight / 2 + 150);
}

void GSPlay::Exit()
{
	printf("%d", m_Test);
}


void GSPlay::Pause()
{
}

void GSPlay::Resume()
{
}


void GSPlay::HandleEvents()
{
}

void GSPlay::HandleKeyEvents(int key, bool bIsPressed)
{
}

void GSPlay::HandleTouchEvents(int x, int y, bool bIsPressed)
{
	for (auto button : m_listButton)
	{
		if(button->HandleTouchEvents(x, y, bIsPressed))
		{
			break;
		}
	}

	if (m_dek->HandleTouchEvents(x, y, bIsPressed)) {
		//cout << 5;
	}

}

void GSPlay::HandleMouseMoveEvents(int x, int y)
{
}

void GSPlay::Update(float deltaTime)
{
	for (auto it : m_listButton)
	{
		it->Update(deltaTime);
	}
/*	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
	}*/

	if (diToi) {
		Vector2 van = vanToc(0.1f, Vector2(cardDi->GetPosition().x, cardDi->GetPosition().y), toiNoi);

		cardDi->Set2DPosition(cardDi->GetPosition().x + van.x*deltaTime, cardDi->GetPosition().y + van.y * deltaTime);
		if (abs(cardDi->GetPosition().x - toiNoi.x) < 10 && abs(cardDi->GetPosition().y - toiNoi.y) < 10) {
			diToi = false;
			cardDi->Set2DPosition(toiNoi);

			toiNoi = Vector2(toiNoi.x + 40, toiNoi.y);

			cout << "Da di toi";
		}
	}
}

void GSPlay::Draw()
{
	m_background->Draw();
	//m_score->Draw();
	m_dek->Draw();
	m_card0->Draw();

	for (auto it : m_listButton)
	{
		it->Draw();
	}

	for (auto it : m_player1)
	{
		it->Draw();
	}

/*	for (auto it : m_listAnimation)
	{
		it->Draw();
	}*/
}

Vector2 GSPlay::vanToc(float giay, Vector2 tu, Vector2 toi) {
	float vX = (toi.x - tu.x) / giay;
	float vY = (toi.y - tu.y) / giay;

	return Vector2(vX, vY);
}

void GSPlay::Instantiate(Vector2 noiK) {
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("/0/v3.tga");

	shared_ptr<GameButton>  button = make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(noiK);
	button->SetSize(50, 70);

	button->SetOnClick([this]() {
		
		});
	cardDi = button;
	m_player1.push_back(button);
}
