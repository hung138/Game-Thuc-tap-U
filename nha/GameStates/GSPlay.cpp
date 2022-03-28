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
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");

	// background
	auto texture = ResourceManagers::GetInstance()->GetTexture("nut_background.tga");
	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_background->SetSize(Globals::screenWidth, Globals::screenHeight);

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	std::shared_ptr<GameButton>  button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth - 30, 30);
	button->SetSize(50, 50);
	button->SetOnClick([this]() {
			GameStateMachine::GetInstance()->PopState();
		});
	m_listButton.push_back(button);

	// score
/*	shader = ResourceManagers::GetInstance()->GetShader("TextShader");
	std::shared_ptr<Font> font = ResourceManagers::GetInstance()->GetFont("Brightly Crush Shine.otf");
	m_score = std::make_shared< Text>(shader, font, "score: 10", TextColor::RED, 1.0);
	m_score->Set2DPosition(Vector2(5, 25));*/

/*	shader = ResourceManagers::GetInstance()->GetShader("Animation");
	texture = ResourceManagers::GetInstance()->GetTexture("Actor1_2.tga");
	std::shared_ptr<SpriteAnimation> obj = std::make_shared<SpriteAnimation>(model, shader, texture, 9, 6, 5, 0.1f);
	
	obj->Set2DPosition(240, 400);
	obj->SetSize(334, 223);
	//obj->SetRotation(Vector3(0.0f, 3.14f, 0.0f));
	m_listAnimation.push_back(obj);*/

	// set bai
	GetDataCard();

	texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	m_bobai = std::make_shared<Sprite2D>(model, shader, texture);
	m_bobai->Set2DPosition(Globals::screenWidth/2 - 80, Globals::screenHeight/2);
	m_bobai->SetSize(50, 72);

	texture = ResourceManagers::GetInstance()->GetTexture("/0/hide.tga");
	m_card0 = std::make_shared<Sprite2D>(model, shader, texture);
	m_card0->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);
	m_card0->SetSize(50, 72);

	texture = ResourceManagers::GetInstance()->GetTexture("/1red/den.tga");
	m_den = std::make_shared<Sprite2D>(model, shader, texture);
	m_den->Set2DPosition(Globals::screenWidth / 2 + 60, Globals::screenHeight / 2);
	m_den->SetSize(30, 30);

	ShowCard0(42);
	MixBai();
}

void ChiaBai() {

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
}

void GSPlay::Draw()
{
	m_background->Draw();
	m_bobai->Draw();
	m_card0->Draw();
	m_den->Draw();

	//m_score->Draw();
	for (auto it : m_listButton)
	{
		it->Draw();
	}

/*	for (auto it : m_listAnimation)
	{
		it->Draw();
	}*/
}

list<int> deckk;

int soDem[108]; // 0->9, 10 - skip , 11 - reverse, 12 - Draw 2, 13 - Wild, 14 - Wild Draw 4
int mauu[108]; // 0 - 4 colors, 1 - red, 2 - green, 3 - blue, 4 - cam 
string cardName[108]; // ten cua card
string cardSpi[108]; // duong dan den sprite cua card
string duongMau[5] = {"/0/", "/1red/","/2green/","/3blue/","/4cam/"};

string duoiFile = ".tga";

void GSPlay::GetDataCard() {
	int dem = 0;
	for (int i = 0; i < 27; i++) {
		string mma = "";

		if (dem == 0) {
			mma = "red";
			mauu[dem * 27 + i] = 1;
		}
		else if (dem == 1) {
			mma = "green";
			mauu[dem * 27 + i] = 2;
		}
		else if (dem == 2) {
			mma = "blue";
			mauu[dem * 27 + i] = 3;
		}
		else if (dem == 3) {
			mma = "yellow";
			mauu[dem * 27 + i] = 4;
		}
		if (i < 10) {
			cardName[dem * 27 + i] = mma + to_string(i);
			cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(i) + duoiFile;
			soDem[dem * 27 + i] = i;
		}
		else if (i < 19) {
			cardName[dem * 27 + i] = mma + std::to_string(i-9);// (i - 9);
			cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(i - 9) + duoiFile;
			soDem[dem * 27 + i] = i - 9;
		}
		else {
			if (i == 19 || i == 20) {
				cardName[dem * 27 + i] = mma + "Skip";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(10) + duoiFile;
				soDem[dem * 27 + i] = 10;
			}
			else if (i == 21 || i == 22) {
				cardName[dem * 27 + i] = mma + "Reverse";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(11) + duoiFile;
				soDem[dem * 27 + i] = 11;
			}
			else if (i == 23 || i == 24) {
				cardName[dem * 27 + i] = mma + "Draw 2";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(12) + duoiFile;
				soDem[dem * 27 + i] = 12;
			}
			else if (i == 25) {
				cardName[dem * 27 + i] = "Wild";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(13) + duoiFile;
				soDem[dem * 27 + i] = 13;
				mauu[dem * 27 + i] = 0;
			}
			else if (i == 26) {
				cardName[dem * 27 + i] = "Wild Draw 4";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(14) + duoiFile;
				soDem[dem * 27 + i] = 14;
				mauu[dem * 27 + i] = 0;

				i = -1;
				dem++;
			}
		}

		if (dem == 4) {
			break;
		}
	}
}

void GSPlay::MixBai() {
	deckk.clear();
	list<int> deckk2;

	for (int i = 0; i < 108; i++)
	{
		deckk2.push_back(i);
	}

	srand(time(NULL));
	for (int i = 0; i < 108; i++) {
		int ran = rand() % deckk2.size();

		//int ch = deckk2[ran];
		int ch = *std::next(deckk2.begin(), ran);

		cout << ch;
		cout << "\n";

		deckk.push_back(ch);
		deckk2.remove(ch);
	}
}

void GSPlay::ShowCard0(int c) {
	auto texture = ResourceManagers::GetInstance()->GetTexture("" + cardSpi[c]);
	m_card0->SetTexture(texture);

	if (mauu[c] == 0) {
	
	}
	else {
		texture = ResourceManagers::GetInstance()->GetTexture("" + duongMau[mauu[c]] + "den.tga");
		m_den->SetTexture(texture);
	}
}