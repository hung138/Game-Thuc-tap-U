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

#include <windows.h>
using namespace std;

list<int> deckk;

int soDem[108]; // 0->9, 10 - skip , 11 - reverse, 12 - Draw 2, 13 - Wild, 14 - Wild Draw 4
int mauu[108]; // 0 - 4 colors, 1 - red, 2 - green, 3 - blue, 4 - cam 
string cardName[108]; // ten cua card
string cardSpi[108]; // duong dan den sprite cua card
string duongMau[5] = { "/0/", "/1red/","/2green/","/3blue/","/4cam/" };

string duoiFile = ".tga";

float vt0 = Globals::screenWidth / 2 - 200;

Vector2 noiPla[5] = { Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2),
Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2 + 220),
Vector2(Globals::screenWidth / 2 + 310, Globals::screenHeight / 2) ,
Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2 - 220) ,
Vector2(Globals::screenWidth / 2 - 310, Globals::screenHeight / 2) };

Vector4 mauMe[5] = { Vector4(0,0,0,1),
Vector4(255, 0, 0, 1),
Vector4(0, 255, 0, 1),
Vector4(0, 0, 255, 1),
Vector4(255, 165, 0, 1)
};

string tenPlayer[4] = { "You", "Lee","Nam","Linh" };

int baiRut, baiDanh;
int luot, huong;
int soT, mauT;
shared_ptr<GameButton> bai, bai2;
shared_ptr<Sprite2D> rever;
shared_ptr<Sprite2D> ShowDB;
list<shared_ptr<GameButton>> conDB;
list<shared_ptr<GameButton>> mauDB;

bool toiCh = false;
int toiPl;
float toiR;

Vector2 van;
Vector2 toiT(0, 0);

// chia bai luc dau
bool khoiDau, hitBai;
int dem;
int check;
int maxDraw;

float tim, timMax = 1;

bool isRandomDanh;
float tim2;

//
list<int>	de1;
list<int>	de2;
list<int>	de3;
list<int>	de4;

list<shared_ptr<GameButton>>	card1;
list<shared_ptr<GameButton>>	card2;
list<shared_ptr<GameButton>>	card3;
list<shared_ptr<GameButton>>	card4;

int thuCard1 = 0;
bool isChon, isClick = false;

int cMau[4] = { 1,2,3,4 };
int bamMau = 0;

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
	auto texture = ResourceManagers::GetInstance()->GetTexture("nut_background.tga");

	// background
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	m_background = std::make_shared<Sprite2D>(model, shader, texture);
	m_background->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_background->SetSize(Globals::screenWidth, Globals::screenHeight);
	// huong

	// button close
	texture = ResourceManagers::GetInstance()->GetTexture("btn_close.tga");
	std::shared_ptr<GameButton>  button = std::make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(Globals::screenWidth - 50, 50);
	button->SetSize(50, 50);
	button->SetOnClick([this]() {
		//GameStateMachine::GetInstance()->PopState();
		exit(0);
		});
	m_listButton.push_back(button);

	texture = ResourceManagers::GetInstance()->GetTexture("btn_restart.tga");
	std::shared_ptr<GameButton>  button2 = std::make_shared<GameButton>(model, shader, texture);
	button2->Set2DPosition(50, 50);
	button2->SetSize(50, 50);
	button2->SetOnClick([this]() {
		ResetBai();
		});
	m_listButton.push_back(button2);

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

	// ten
	for (int i = 0; i < sizeof(tenPlayer); i++)
	{
		auto shader2 = ResourceManagers::GetInstance()->GetShader("TextShader");
		shared_ptr<Font> font2 = ResourceManagers::GetInstance()->GetFont("arialbd.ttf");
		shared_ptr<Text> htTen = make_shared< Text>(shader2, font2, "" + tenPlayer[i], TextColor::RED, 0.8);
		if (i == 0) {
			htTen->Set2DPosition(Vector2(noiPla[i + 1].x - 20, noiPla[i + 1].y - 70));
		}
		else if (i == 2) {
			htTen->Set2DPosition(Vector2(noiPla[i + 1].x - 20, noiPla[i + 1].y + 90));
		}
		else if (i == 1) {
			htTen->SetRotation(Vector3(0, 0, 3.14 / 2));
			htTen->Set2DPosition(Vector2(noiPla[i + 1].x + 70, noiPla[i + 1].y));
		}
		else if (i == 3) {
			htTen->SetRotation(Vector3(0, 0, -3.14 / 2));
			htTen->Set2DPosition(Vector2(noiPla[i + 1].x + 70, noiPla[i + 1].y));
		}

		m_ten.push_back(htTen);
	}

	// bai2
	texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
	bai2 = make_shared<GameButton>(model, shader, texture);
	bai2->Set2DPosition(0, 0);
	bai2->SetSize(60, 90);
	bai2->SetRotation(Vector3(0, 0, 0));

	// set deck
	texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	m_dek = std::make_shared<GameButton>(model, shader, texture);
	m_dek->Set2DPosition((float)Globals::screenWidth / 2 - 100, (float)Globals::screenHeight / 2);
	m_dek->SetSize(60, 90);

	texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
	m_card0 = std::make_shared<Sprite2D>(model, shader, texture);
	m_card0->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_card0->SetSize(60, 90);

	texture = ResourceManagers::GetInstance()->GetTexture("1red/den.tga");
	m_den = std::make_shared<Sprite2D>(model, shader, texture);
	m_den->Set2DPosition((float)Globals::screenWidth / 2 + 80, (float)Globals::screenHeight / 2);
	m_den->SetSize(30, 30);

	toiT = noiPla[1];

	ResourceManagers::GetInstance()->PlaySound("UNO OST Music.mp3", true);
	// deck
	ResetBai();
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
		if (button->HandleTouchEvents(x, y, bIsPressed))
		{
			break;
		}
	}

	if (!isClick && !isChon && !isRandomDanh) {
		for (auto button : card1)
		{
			if (button->HandleTouchEvents(x, y, bIsPressed))
			{
				PlayerDanh(button);
			}
		}
	}

	if ((soT == 13 || soT == 14) && !hitBai && !toiCh && bamMau == 0 && luot == 1) {
		for (auto button : mauDB)
		{
			if (button->HandleTouchEvents(x, y, bIsPressed))
			{
				SelectMau(button);
			}
		}
	}

	/*	if (m_dek->HandleTouchEvents(x, y, bIsPressed)) {
			//cout << 5;
		}*/

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

	if (khoiDau) {
		if (!toiCh) {
			if (dem != 7 * 4) {
				ChiaBai(luot);
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					}
					else {
						luot++;
					}
				}
				else {
					if (luot == 1) {
						luot = 4;
					}
					else {
						luot--;
					}
				}
				dem++;
			}
			else {
				luot = 0;
				khoiDau = false;
				ChiaBai(0);
				dem = 0;
			}
		}
	}

	if (hitBai) {
		if (!toiCh) {
			if (dem != maxDraw) {
				ChiaBai(luot);
				dem++;
			}
			else {
				hitBai = false;
				dem = 0;
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					}
					else {
						luot++;
					}
				}
				else {
					if (luot == 1) {
						luot = 4;
					}
					else {
						luot--;
					}
				}
				RandomDanhBai();
			}
		}
	}

	if (isChon) {
		tim += deltaTime;

		if (tim >= timMax) {
			if (soT != 13 && soT != 14) {
				conDB.clear();
				isChon = false;
				tim = 0;
				bamMau = 0;
				if (mauDB.size() > 0) {
					for (int i = 0; i < 4; i++)
					{
						cMau[i] = 0;
					}
				}

				if (soT != 15) {
					AnShowCard(soT);
				}
				else
				{
					maxDraw = 4;
					hitBai = true;
					soT = 14;
				}

			}
			else {
				if (bamMau == 1) {
					for (int i = 0; i < 4; i++)
					{
						if (i != mauT - 1) {
							cMau[i] = 0;
						}
						else
						{
							cMau[i] = i + 1;
						}
					}

					auto texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauT] + "/den.tga");
					m_den->SetTexture(texture);

					tim = 0;
					bamMau++;
				}
				else if (bamMau == 2)
				{
					conDB.clear();
					isChon = false;
					tim = 0;
					bamMau = 0;
					if (mauDB.size() > 0) {
						for (int i = 0; i < 4; i++)
						{
							cMau[i] = 0;
						}
					}

					AnShowCard(soT);
				}
			}
		}
	}

	if (isRandomDanh) {
		tim2 += deltaTime;
		if (tim2 > timMax) {
			tim2 = 0;
			isRandomDanh = false;
			RandomDanhBai();
		}
	}

	if (toiCh) {
		float tg = deltaTime;
		if (tg > 0.1f) {
			tg = 0.015;
		}

		float kcX = abs(bai2->GetPosition().x - toiT.x) * abs(bai2->GetPosition().x - toiT.x);
		float kcY = abs(bai2->GetPosition().y - toiT.y) * abs(bai2->GetPosition().y - toiT.y);
		float kc = sqrt(kcX + kcY);
		//cout << kc;
		//cout << "\n";
		if (kc < 25) {
			if (kc < 10) {
				van = vanToc(0.03, Vector2(bai2->GetPosition().x, bai2->GetPosition().y), toiT);
			}
			else {
				van = vanToc(0.1, Vector2(bai2->GetPosition().x, bai2->GetPosition().y), toiT);
			}
		}
		else
		{
			van = vanToc(0.15, Vector2(bai2->GetPosition().x, bai2->GetPosition().y), toiT);
		}
		
	/*	cout << "van(" + to_string(van.x) + ", " + to_string(van.y) + ")";
		cout << "\n";
		cout << tg;
		cout << "\n";*/

		bai2->Set2DPosition(bai2->GetPosition().x + van.x * tg, bai2->GetPosition().y + van.y * tg);

		if (kc < 2) {
			if (isClick) {
				isClick = false;
				shared_ptr<GameButton> butn2 = *next(card1.begin(), thuCard1);
				card1.remove(butn2);
			}
			
			toiCh = false;
			bai2->Set2DPosition(toiT);

			if (toiPl == 0) {
				if (luot > 0 && !khoiDau && (card1.size() == 0 || card2.size() == 0 || card3.size() == 0 || card4.size() == 0)) {
					// co nguoi win
					cout << "End";
				}
				else {
					auto texture = ResourceManagers::GetInstance()->GetTexture(cardSpi[baiDanh]);
					m_card0->SetTexture(texture);

					texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauu[baiDanh]] + "/den.tga");
					m_den->SetTexture(texture);

					texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
					bai2->SetTexture(texture);
					bai2->Set2DPosition(0, 0);

					soT = soDem[baiDanh];
					mauT = mauu[baiDanh];

					//cout << "card: " + cardName[baiDanh];
					//cout << "\n";

					if (mauT > 0) {
						if (soT == 10 || soT == 11 || soT == 12) {  // skip
							ShowCardDB(soT);
						}
						else {
							if (luot == 0) {
								luot = 1;
							}
							else {
								if (huong == 1) {
									if (luot == 4) {
										luot = 1;
									}
									else {
										luot++;
									}
								}
								else {
									if (luot == 1) {
										luot = 4;
									}
									else {
										luot--;
									}
								}
							}

							if (luot == 1) {
								RandomDanhBai();
							}
							else
							{
								isRandomDanh = true;
							}
							//delay(2);
						}
					}
					else {
						// card wild
						if (soT == 13) {  // chon mau
							ShowCardDB(soT);
						}
						else if (soT == 14) {  // chon mau, + 4
							ShowCardDB(soT);
						}
					}
				}

			}
			else {
				DichBai();
				if (!khoiDau && !hitBai) {
					if (luot == 1) {
						RandomDanhBai();
					}
					else
					{
						isRandomDanh = true;
					}
					//delay(2);
				}

			}
		}
		else {
			// chon bai de danh

		}
	}
}

void GSPlay::Draw()
{
	m_background->Draw();
	rever->Draw();
	//m_score->Draw();
	m_dek->Draw();
	m_card0->Draw();
	m_den->Draw();
	bai2->Draw();

	for (auto it : m_listButton)
	{
		it->Draw();
	}

	for (auto it : m_ten)
	{
		it->Draw();
	}

	for (auto it : card1)
	{
		it->Draw();
	}

	for (auto it : card2)
	{
		it->Draw();
	}

	for (auto it : card3)
	{
		it->Draw();
	}

	for (auto it : card4)
	{
		it->Draw();
	}

	if (isChon) {
		ShowDB->Draw();
		for (auto it : conDB)
		{
			it->Draw();
		}

		if (soT == 13 || soT == 14) {
			int damm = 0;
			for (auto it : mauDB)
			{
				if (damm == cMau[damm] - 1) {
					it->Draw();
				}
				damm++;
			}
		}
	}

	/*	for (auto it : m_listAnimation)
		{
			it->Draw();
		}*/
}

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
			cardName[dem * 27 + i] = mma + std::to_string(i - 9);// (i - 9);
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
				//mauu[dem * 27 + i] = 1;
			}
			else if (i == 26) {
				cardName[dem * 27 + i] = "Wild Draw 4";
				cardSpi[dem * 27 + i] = duongMau[dem + 1] + to_string(14) + duoiFile;
				soDem[dem * 27 + i] = 14;
				mauu[dem * 27 + i] = 0;
				//mauu[dem * 27 + i] = 1;

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

		//cout << ch;
		//cout << "\n";

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

void GSPlay::RandomDanhBai() {

	// hien thi ten nguoi choi sang xanh

	int demm = 0;
	for (auto it : m_ten)
	{
		if (demm == luot - 1) {
			it->SetColor(Vector4(0, 255, 0, 1));
		}
		else {
			it->SetColor(Vector4(255, 0, 0, 1));
		}
		demm++;
	}
	//
	demm = 0;

	int dekCua[50];
	int coThe[50];
	int thuTu[50];
	if (luot == 1) {
		for (auto it : de1)
		{
			dekCua[demm] = it;
			demm++;
		}
	}
	else if (luot == 2)
	{
		for (auto it : de2)
		{
			dekCua[demm] = it;
			demm++;
		}
	}
	else if (luot == 3)
	{
		for (auto it : de3)
		{
			dekCua[demm] = it;
			demm++;
		}
	}
	else if (luot == 4)
	{
		for (auto it : de4)
		{
			dekCua[demm] = it;
			demm++;
		}
	}
	int soCon = demm;
	demm = 0;

	if (soCon > 0) {
		for (int i = 0; i < soCon; i++) {
			int b = dekCua[i];
			if (mauu[b] == 0 || (mauu[b] == mauT) || (soDem[b] == soDem[baiDanh])) {
				coThe[demm] = b;
				thuTu[demm] = i;
				demm++;
			}
		}
	}
	int demCoT = demm;

	if (demCoT > 0) {
		check = 0;

		if (luot != 1) {
		srand(time(NULL));
		int ran = rand() % demCoT;

		shared_ptr<GameButton> btn;
		if (luot == 1) {
			btn = *next(card1.begin(), ran);
		}
		else if (luot == 2) {
			btn = *next(card2.begin(), ran);
		}
		else if (luot == 3) {
			btn = *next(card3.begin(), ran);
		}
		else if (luot == 4) {
			btn = *next(card4.begin(), ran);
		}

		DanhBai(luot, coThe[ran], soCon, thuTu[ran]);
		}
	}
	else {
		// rut bai
		check++;
		if (check == 2) {
			check = 0;

			if (luot == 0) {
				luot = 1;
			}
			else {
				if (huong == 1) {
					if (luot == 4) {
						luot = 1;
					}
					else {
						luot++;
					}
				}
				else {
					if (luot == 1) {
						luot = 4;
					}
					else {
						luot--;
					}
				}
			}

			RandomDanhBai();
		}
		else {
			ChiaBai(luot);
		}
	}
}

void GSPlay::DanhBai(int cho, int baiDanhRa, int soC, int thuMay) {
	toiR = 0;
	toiT = noiPla[0];

	deckk.push_back(baiDanh);
	baiDanh = baiDanhRa;

	toiPl = 0;

	int soCon = soC;
	int namO = thuMay;
	int demm = 0;

	if (soCon > 0) {
		if (cho == 1) {
			for (auto it : card1) {
				if (demm < namO) {
					it->Set2DPosition(it->GetPosition().x + 28, it->GetPosition().y);
				}
				else
				{
					it->Set2DPosition(it->GetPosition().x - 28, it->GetPosition().y);
				}
				demm++;

				if (demm == soCon) {
					break;
				}
			}
		}
		else if (cho == 3) {
			for (auto it : card3) {
				if (demm < namO) {
					it->Set2DPosition(it->GetPosition().x - 20, it->GetPosition().y);
				}
				else
				{
					it->Set2DPosition(it->GetPosition().x + 20, it->GetPosition().y);
				}
				demm++;

				if (demm == soCon) {
					break;
				}
			}
		}
		else if (cho == 2) {
			for (auto it : card2) {
				if (demm < namO) {
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y - 8);
				}
				else
				{
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y + 8);
				}
				demm++;

				if (demm == soCon) {
					break;
				}
			}
		}
		else if (cho == 4) {
			for (auto it : card4) {
				if (demm < namO) {
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y + 8);
				}
				else
				{
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y - 8);
				}
				demm++;

				if (demm == soCon) {
					break;
				}
			}
		}

	}

	Vector2 tuu(0, 0);
	if (cho == 1) {
		de1.remove(baiDanhRa);

		thuCard1 = thuMay;
		shared_ptr<GameButton> butn2 = *next(card1.begin(), thuMay);
		tuu = Vector2(butn2->GetPosition().x, butn2->GetPosition().y);

		auto texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
		butn2->SetTexture(texture);
	}
	else if (cho == 2) {
		de2.remove(baiDanhRa);
		shared_ptr<GameButton> butn2 = *next(card2.begin(), thuMay);
		tuu = Vector2(butn2->GetPosition().x, butn2->GetPosition().y);
		card2.remove(butn2);
	}
	else if (cho == 3) {
		de3.remove(baiDanhRa);
		shared_ptr<GameButton> butn2 = *next(card3.begin(), thuMay);
		tuu = Vector2(butn2->GetPosition().x, butn2->GetPosition().y);
		card3.remove(butn2);
	}
	else if (cho == 4) {
		de4.remove(baiDanhRa);
		shared_ptr<GameButton> butn2 = *next(card4.begin(), thuMay);
		tuu = Vector2(butn2->GetPosition().x, butn2->GetPosition().y);
		card4.remove(butn2);
	}

	Instantiate(tuu, luot, 0, baiDanhRa, 0);
	toiCh = true;
	ResourceManagers::GetInstance()->PlaySound("draw.mp3");
	//rever.GetComponent <AudioSource>().Play();
}

void GSPlay::ChiaBai(int cho) {
	toiR = 0;
	int soCon = 0;
	if (cho == 1) {
		toiR = 0;
		soCon = de1.size();
	}
	else if (cho == 2) {
		toiR = -3.14f / 2;
		soCon = de2.size();
	}
	else if (cho == 3) {
		toiR = 3.14f;
		soCon = de3.size();
	}
	else if (cho == 4) {
		toiR = 3.14f / 2;
		soCon = de4.size();
	}
	
	baiRut = deckk.front();
	deckk.pop_front();

	Instantiate(Vector2(m_dek->GetPosition().x, m_dek->GetPosition().y), cho, toiR, baiRut, 1);

	if (cho > 0) {
		if (soCon >= 1) {
			if (cho == 1) {
				for (auto it : card1) {
					it->Set2DPosition(it->GetPosition().x - 28, it->GetPosition().y);
				}
			}
			else if (cho == 3) {
				for (auto it : card3) {
					it->Set2DPosition(it->GetPosition().x + 20, it->GetPosition().y);
				}
			}
			else if (cho == 2) {
				for (auto it : card2) {
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y + 8);
				}
			}
			else if (cho == 4) {
				for (auto it : card4) {
					it->Set2DPosition(it->GetPosition().x, it->GetPosition().y - 8);
				}
			}

			if (cho == 1) {
				Vector2 vtM2(card1.back()->GetPosition().x + 56, card1.back()->GetPosition().y);
				toiT = vtM2;
			}
			else if (cho == 3) {
				Vector2 vtM2(card3.back()->GetPosition().x - 40, card3.back()->GetPosition().y);
				toiT = vtM2;
			}
			else if (cho == 2) {
				Vector2 vtM2(card2.back()->GetPosition().x, card2.back()->GetPosition().y - 16);
				toiT = vtM2;
			}
			else if (cho == 4) {
				Vector2 vtM2(card4.back()->GetPosition().x, card4.back()->GetPosition().y + 16);
				toiT = vtM2;
			}
		}
		else {
			toiT = noiPla[cho];
		}
	}
	else {
		toiT = noiPla[cho];
	}

	if (cho == 0) {
		baiDanh = baiRut;
	}
	else {
		if (cho == 1) {
			de1.push_back(baiRut);
		}
		else if (cho == 2) {
			de2.push_back(baiRut);
		}
		else if (cho == 3) {
			de3.push_back(baiRut);
		}
		else if (cho == 4) {
			de4.push_back(baiRut);
		}
	}
	toiPl = cho;
	toiCh = true;

	ResourceManagers::GetInstance()->PlaySound("draw.mp3");
}

void GSPlay::DichBai() {
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture(cardSpi[baiRut]);
	if (toiPl > 1) {
		texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	}

	shared_ptr<GameButton>	m_nut = make_shared<GameButton>(model, shader, texture);

	m_nut->SetRotation(Vector3(0, 0, toiR));
	m_nut->Set2DPosition(toiT);
	m_nut->SetSize(60, 90);

	if (toiPl == 1) {
		m_nut->SetOnClick([this]() {

		});
		card1.push_back(m_nut);
	}
	else if (toiPl == 2) {
		card2.push_back(m_nut);
	}
	else if (toiPl == 3) {
		card3.push_back(m_nut);
	}
	else if (toiPl == 4) {
		card4.push_back(m_nut);
	}
}

void GSPlay::ResetBai() {
	ResetChiSo();

	cout << "Reset bai";

	GetDataCard();

	srand(time(NULL));
	huong = rand() % 10;
	cout << huong;
	cout << "\n";

	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture0 = ResourceManagers::GetInstance()->GetTexture("/0/reverse0.tga");
	auto texture1 = ResourceManagers::GetInstance()->GetTexture("/0/reverse1.tga");

	rever = make_shared<Sprite2D>(model, shader, texture0);
	rever->SetSize(500, 400);
	rever->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);

	if (huong < 5) {
		huong = 1;
		rever->SetTexture(texture0);
	}
	else {
		huong = 0;
		rever->SetTexture(texture1);
	}

	MixBai();
	check = 0;
	baiDanh = -1;
	luot = 1;

	khoiDau = true;
}


/*if (!diToi) {
	Instantiate(Vector2(m_dek->GetPosition().x, m_dek->GetPosition().y));
	diToi = true;
}*/

Vector2 GSPlay::vanToc(float giay, Vector2 tu, Vector2 toi) {
	float vX = (toi.x - tu.x) / giay;
	float vY = (toi.y - tu.y) / giay;

	return Vector2(vX, vY);
}

void GSPlay::Instantiate(Vector2 noiK, int pla, float rr, int bRut, int chia) {
	auto texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	if ((pla < 2 && chia == 1) || chia == 0) {
		texture = ResourceManagers::GetInstance()->GetTexture(cardSpi[bRut]);
	}

	bai2->SetTexture(texture);
	bai2->Set2DPosition(noiK);
	bai2->SetRotation(Vector3(0, 0, rr));
}

void GSPlay::delay(int a) {
	int add = 0;
	int time = a * 100000000;

	for (int i = 0; i < time; i++)
	{
		add *= i;
		add++;
		add++;
	}
}

void GSPlay::ShowCardDB(int cho) {
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("nut_Show.tga");
	if (ShowDB == NULL) {
		ShowDB = make_shared<Sprite2D>(model, shader, texture);
		ShowDB->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
		ShowDB->SetSize(Globals::screenWidth, Globals::screenHeight); 
	}
	isChon = true;

	if (cho == 10) {// skip
		texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauu[baiDanh]] + "bann.tga");
		shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
		htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
		htS->SetSize(270, 270);

		conDB.push_back(htS);
	}
	else if (cho == 11) { // reverse
		if (huong == 1) {
			texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauu[baiDanh]] + "reverse1.tga");
			auto texture0 = ResourceManagers::GetInstance()->GetTexture(duongMau[0] + "reverse1.tga");
			rever->SetTexture(texture0);
		}
		else {
			texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauu[baiDanh]] + "reverse0.tga");
			auto texture0 = ResourceManagers::GetInstance()->GetTexture(duongMau[0] + "reverse0.tga");
			rever->SetTexture(texture0);
		}

		shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
		htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
		htS->SetSize(250, 250);

		conDB.push_back(htS);
	}
	else if (cho == 12) { // +2
		texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauu[baiDanh]] + "p2.tga");
		shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
		htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
		htS->SetSize(250, 150);

		conDB.push_back(htS);
	}
	else if (cho == 13) { // wild
		if (mauDB.size() == 0) {
			for (int i = 0; i < 4; i++)
			{
				texture = ResourceManagers::GetInstance()->GetTexture(duongMau[i + 1] + "den.tga");
				shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
				if (i == 0) {
					htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2 - 200);
				}
				else if (i == 1) {
					htS->Set2DPosition((float)Globals::screenWidth / 2 + 200, (float)Globals::screenHeight / 2);
				}
				else if (i == 2) {
					htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2 + 200);
				}
				else if (i == 3) {
					htS->Set2DPosition((float)Globals::screenWidth / 2 - 200, (float)Globals::screenHeight / 2);
				}
				htS->SetSize(150, 150);
				htS->SetOnClick([this]() {

					});

				mauDB.push_back(htS);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				cMau[i] = i + 1;
			}
		}

		if (luot != 1) {
			bamMau++;
			srand(time(NULL));
			int dimm = rand() % 4;
			mauT = dimm + 1;

			tim = 0;
			isChon = true;
		}
		else {
			bamMau = 0;
		}
	}
	else if (cho == 14) {  // wild + 4
		if (mauDB.size() == 0) {
			for (int i = 0; i < 4; i++)
			{
				texture = ResourceManagers::GetInstance()->GetTexture(duongMau[i + 1] + "den.tga");
				shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
				if (i == 0) {
					htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2 - 200);
				}
				else if (i == 1) {
					htS->Set2DPosition((float)Globals::screenWidth / 2 + 200, (float)Globals::screenHeight / 2);
				}
				else if (i == 2) {
					htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2 + 200);
				}
				else if (i == 3) {
					htS->Set2DPosition((float)Globals::screenWidth / 2 - 200, (float)Globals::screenHeight / 2);
				}
				htS->SetSize(150, 150);
				htS->SetOnClick([this]() {

					});

				mauDB.push_back(htS);
			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				cMau[i] = i + 1;
			}
		}

		if (luot != 1) {
			bamMau++;
			srand(time(NULL));
			int dimm = rand() % 4;
			mauT = dimm + 1;

			tim = 0;
			isChon = true;
		}
		else {
			bamMau = 0;
		}
	}
	else if (cho == 15) {  // +4
		AnShowCard(cho);
	}

	ResourceManagers::GetInstance()->PlaySound("btt.mp3");
}

void GSPlay::AnShowCard(int cho) {

	if (cho == 10) {  // skip
		if (luot == 0) {
			if (huong == 1) {
				luot = 2;
			}
			else {
				luot = 4;
			}
		}
		else {
			if (huong == 1) {
				if (luot == 4) {
					luot = 2;
				}
				else if (luot == 3) {
					luot = 1;
				}
				else {
					luot += 2;
				}
			}
			else {
				if (luot == 1) {
					luot = 3;
				}
				else if (luot == 2) {
					luot = 4;
				}
				else {
					luot -= 2;
				}
			}
		}
		RandomDanhBai();
	}
	else if (cho == 11) {  // reverse
		if (luot == 0) {
			luot = 1;
			if (huong == 1) {
				huong = 0;
			}
			else {
				huong = 1;
			}
		}
		else {
			if (huong == 1) {
				huong = 0;
				if (luot == 1) {
					luot = 4;
				}
				else {
					luot--;
				}
			}
			else {
				huong = 1;
				if (luot == 4) {
					luot = 1;
				}
				else {
					luot++;
				}
			}
		}
		RandomDanhBai();
	}
	else if (cho == 12) {  // +2
		if (luot == 0) {
			luot = 1;
		}
		else {
			if (huong == 1) {
				if (luot == 4) {
					luot = 1;
				}
				else {
					luot++;
				}
			}
			else {
				if (luot == 1) {
					luot = 4;
				}
				else {
					luot--;
				}
			}
		}
		maxDraw = 2;
		hitBai = true;
	}
	else if (cho == 13) {  // wild
		if (luot == 0) {
			luot = 1;
		}
		else {
			if (huong == 1) {
				if (luot == 4) {
					luot = 1;
				}
				else {
					luot++;
				}
			}
			else {
				if (luot == 1) {
					luot = 4;
				}
				else {
					luot--;
				}
			}
		}

		RandomDanhBai();
	} else if (cho == 14) {  // wild +4
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		auto texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauT] + "p4.tga");

		shared_ptr<GameButton> htS = make_shared<GameButton>(model, shader, texture);
		htS->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
		htS->SetSize(250, 150);

		conDB.push_back(htS);

		AnShowCard(15);
	}
	else if (cho == 15) {  // +4
		if (luot == 0) {
			luot = 1;
		}
		else {
			if (huong == 1) {
				if (luot == 4) {
					luot = 1;
				}
				else {
					luot++;
				}
			}
			else {
				if (luot == 1) {
					luot = 4;
				}
				else {
					luot--;
				}
			}
		}

		soT = 15;
		tim = 0;
		isChon = true;
	}
}

void GSPlay::PlayerDanh(shared_ptr<GameButton> btnn) {
	if (!isChon && luot == 1 && !toiCh && !khoiDau && !hitBai) {
		int soCon = de1.size();

		int thuTu = 0;
		int bDanh = 0;

		int demm = 0;
		for (auto it : card1) {
			if (it == btnn) {
				break;
			}
			demm++;
		}

		thuTu = demm;
		bDanh = *next(de1.begin(), thuTu);

		if (mauu[bDanh] == 0 || (mauu[bDanh] == mauT) || (soDem[bDanh] == soDem[baiDanh])) {
			cout << cardName[bDanh];
			cout << "-";
			cout << soCon;
			cout << "-";
			cout << thuTu;
			cout << "\n";

			isClick = true;
			DanhBai(1, bDanh, soCon, thuTu);
		}
	}
    
}

void GSPlay::SelectMau(shared_ptr<GameButton> btnn) {
	int dimm = 0;
	for (auto it : mauDB) {
		if (it == btnn) {
			break;
		}
		dimm++;
	}

	if (dimm == 0) {
		cout << "red";
	}
	else if (dimm == 1) {
		cout << "green";
	}
	else if (dimm == 2) {
		cout << "blue";
	}
	else if (dimm == 3) {
		cout << "cam";
	}

	for (int i = 0; i < 4; i++)
	{
		if (i != dimm) {
			cMau[i] = 0;
		}
		else
		{
			cMau[i] = i + 1;
		}
	}

	mauT = dimm + 1;

	auto texture = ResourceManagers::GetInstance()->GetTexture(duongMau[mauT] + "/den.tga");
	m_den->SetTexture(texture);

	bamMau += 2;

	cout << duongMau[mauT];
	cout << "\n";

	tim = 0;
	isChon = true;
}

void GSPlay::ResetChiSo() {
	for (auto it : m_ten) {
		it->SetColor(mauMe[1]);
	}

	card1.clear();
	card2.clear();
	card3.clear();
	card4.clear();
	de1.clear();
	de2.clear();
	de3.clear();
	de4.clear();

	bamMau = 0;
	baiRut = 0;
	baiDanh = 0;
	luot = 0;
	conDB.clear();
	toiCh = false;
	toiPl = 0;
	toiR = 0;

	khoiDau = false;
	hitBai = false;
	dem = 0;
	check = 0;
	maxDraw = 0;

	tim = 0;
	timMax = 1;

	tim2 = 0;
	isRandomDanh = false;

	int thuCard1 = 0;
	isChon = false;
	isClick = false;

	auto texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
	m_card0->SetTexture(texture);
	bai2->SetTexture(texture);
}
