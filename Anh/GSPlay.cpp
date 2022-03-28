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

list<int> deckk;

int soDem[108]; // 0->9, 10 - skip , 11 - reverse, 12 - Draw 2, 13 - Wild, 14 - Wild Draw 4
int mauu[108]; // 0 - 4 colors, 1 - red, 2 - green, 3 - blue, 4 - cam 
string cardName[108]; // ten cua card
string cardSpi[108]; // duong dan den sprite cua card
string duongMau[5] = { "/0/", "/1red/","/2green/","/3blue/","/4cam/" };

string duoiFile = ".tga";

float vt0 = Globals::screenWidth / 2 - 200;

Vector2 noiPla[5] = { Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2),
Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2 + 150),
Vector2(Globals::screenWidth / 2 + 200, Globals::screenHeight / 2) ,
Vector2(Globals::screenWidth / 2, Globals::screenHeight / 2 - 150) ,
Vector2(Globals::screenWidth / 2 - 200, Globals::screenHeight / 2) };

int baiRut, baiDanh;
int luot, huong;
int soT, mauT;
shared_ptr<GameButton> bai, bai2;
shared_ptr<Sprite2D> rever;
shared_ptr<GameButton> ShowDB;

bool toiCh = false;
int toiPl;

Vector2 toiT(0, 0);
float toiR;

// chia bai luc dau
bool khoiDau, hitBai;
int dem;
int check;
int maxDraw;

//
list<int>	de1;
list<int>	de2;
list<int>	de3;
list<int>	de4;

list<shared_ptr<GameButton>>	card1;
list<shared_ptr<GameButton>>	card2;
list<shared_ptr<GameButton>>	card3;
list<shared_ptr<GameButton>>	card4;

shared_ptr<GameButton> Instantiate(Vector2 noiK, int pla, float rr) {
	auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
	auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
	auto texture = ResourceManagers::GetInstance()->GetTexture("/0/hide.tga");

	shared_ptr<GameButton>  button = make_shared<GameButton>(model, shader, texture);
	button->Set2DPosition(noiK);
	button->SetSize(50, 70);
	button->SetRotation(Vector3(0, 0, rr));

	return button;
}



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

	// 
	ResetBai();

	// set deck
	texture = ResourceManagers::GetInstance()->GetTexture("/0/bai.tga");
	m_dek = std::make_shared<GameButton>(model, shader, texture);
	m_dek->Set2DPosition((float)Globals::screenWidth / 2 - 80, (float)Globals::screenHeight / 2);
	m_dek->SetSize(50, 70);

	texture = ResourceManagers::GetInstance()->GetTexture("/0/card0.tga");
	m_card0 = std::make_shared<Sprite2D>(model, shader, texture);
	m_card0->Set2DPosition((float)Globals::screenWidth / 2, (float)Globals::screenHeight / 2);
	m_card0->SetSize(50, 70);

	texture = ResourceManagers::GetInstance()->GetTexture("/1red/den.tga");
	m_den = std::make_shared<Sprite2D>(model, shader, texture);
	m_den->Set2DPosition((float)Globals::screenWidth / 2 + 60, (float)Globals::screenHeight / 2);
	m_den->SetSize(30, 30);

	toiT = noiPla[1];
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
/*	for (auto it : m_listAnimation)
	{
		it->Update(deltaTime);
	}*/

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

/*	if (hitBai) {
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
				//RandomDanhBai();
			}
		}
	}*/

	if (toiCh) {
		Vector2 van = vanToc(0.2f, Vector2(bai2->GetPosition().x, bai2->GetPosition().y), toiT);
		bai2->Set2DPosition(bai2->GetPosition().x + van.x * deltaTime, bai2->GetPosition().y + van.y * deltaTime);

		if (abs(bai2->GetPosition().x - toiT.x) < 20 && abs(bai2->GetPosition().y - toiT.y) < 20) {
			toiCh = false;
			bai2->Set2DPosition(toiT);

		/*	if (toiPl == 0) {
				if (vt[0].transform.childCount >= 4) {
					Destroy(vt[0].transform.GetChild(3).gameObject);
				}
			}

		/*	bai2.transform.SetParent(vt[toiPl].transform);

			bai2.AddComponent(typeof(infoCard));
			bai2.GetComponent <infoCard>().stt = baiRut;
			bai2.GetComponent <infoCard>().so = alca.soDem[baiRut];
			bai2.GetComponent <infoCard>().mau = alca.mauu[baiRut];*/

			if (luot == 0) {
				auto texture = ResourceManagers::GetInstance()->GetTexture(cardSpi[baiRut]);
				m_card0->SetTexture(texture);
			}

		/*	if (toiPl == 0) {
				// check ket qua
				if (luot > 0 && vt[luot].transform.childCount == 1) {
					print("Winner: Player " + luot);
					cheEnd.transform.GetChild(0).GetComponent <Text>().text = "" + vt[luot].transform.GetChild(0).GetChild(0).GetComponent <TextMesh>().text
						+ " won";
					cheEnd.SetActive(true);
				}
				else {
					soT = alca.soDem[baiDanh];
					mauT = alca.mauu[baiDanh];
					if (mauT > 0) {
						vt[toiPl].transform.GetChild(2).GetComponent <SpriteRenderer>().sprite = mauu[mauT - 1];

						if (alca.soDem[baiDanh] == 10) {  // skip
							ShowCardDB(alca.soDem[baiDanh]);
						}
						else if (alca.soDem[baiDanh] == 11) {  // reverse
							ShowCardDB(alca.soDem[baiDanh]);
						}
						else if (alca.soDem[baiDanh] == 12) {  // +2
							ShowCardDB(alca.soDem[baiDanh]);
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

							RandomDanhBai();
						}
					}
					else {
						// card wild
						if (alca.soDem[baiDanh] == 13) {  // chon mau
							ShowCardDB(alca.soDem[baiDanh]);
						}
						else if (alca.soDem[baiDanh] == 14) {  // chon mau, + 4
							ShowCardDB(alca.soDem[baiDanh]);
						}
					}
				}
			}
			else {
				if (!khoiDau && !hitBai) {
					RandomDanhBai();
				}
			}*/
		}
	}
	else {

		// chon object
	
	}

/*	if (toiCh) {
		Vector2 van = vanToc(0.1f, Vector2(bai2->GetPosition().x, bai2->GetPosition().y), toiT);

		bai2->Set2DPosition(bai2->GetPosition().x + van.x*deltaTime, bai2->GetPosition().y + van.y * deltaTime);
		if (abs(bai2->GetPosition().x - toiT.x) < 10 && abs(bai2->GetPosition().y - toiT.y) < 10) {
			toiT = false;
			bai2->Set2DPosition(toiT);

			toiT = Vector2(toiT.x + 40, toiT.y);

			cout << "Da di toi";
		}
	}*/
}

void GSPlay::Draw()
{
	m_background->Draw();
	//m_score->Draw();
	m_dek->Draw();
	m_card0->Draw();
	m_den -> Draw();
	rever->Draw();
	bai2->Draw();

	for (auto it : m_listButton)
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

	/*for (auto it : m_player1)
	{
		it->Draw();
	}*/

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

void GSPlay::ChiaBai(int cho) {
	cout << luot;
	cout << "\n";

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

	shared_ptr<GameButton> nutt = Instantiate(Vector2(m_dek->GetPosition().x, m_dek->GetPosition().y), cho, toiR);

	//toiT = noiPla[cho];
	if (cho > 0) {
		if (soCon > 1) {
		/*	for (int i = 1; i < soCon; i++) {
				
				if (cho == 1) {
					shared_ptr<GameButton> ch = *std::next(card1.begin(), i);
					Vector2 vtM(ch->GetPosition().x, ch->GetPosition().y);

					ch->Set2DPosition(vtM.x - 50, vtM.y);
				}
				else if (cho == 3) {
					shared_ptr<GameButton> ch = *std::next(card3.begin(), i);
					Vector2 vtM(ch->GetPosition().x, ch->GetPosition().y);

					ch->Set2DPosition(vtM.x + 50, vtM.y);
				}
				else if (cho == 2) {
					shared_ptr<GameButton> ch = *std::next(card2.begin(), i);
					Vector2 vtM(ch->GetPosition().x, ch->GetPosition().y);

					ch->Set2DPosition(vtM.x, vtM.y + 50);
				}
				else if (cho == 4) {
					shared_ptr<GameButton> ch = *std::next(card4.begin(), i);
					Vector2 vtM(ch->GetPosition().x, ch->GetPosition().y);

					ch->Set2DPosition(vtM.x, vtM.y - 50);
				}
			}*/

			if (cho == 1) {
				Vector2 vtM2(card1.back()->GetPosition().x + 50, card1.back()->GetPosition().y);
				toiT = vtM2;
			}
			else if (cho == 3) {
				Vector2 vtM2(card3.back()->GetPosition().x - 50, card3.back()->GetPosition().y);
				toiT = vtM2;
			}
			else if (cho == 2) {
				Vector2 vtM2(card2.back()->GetPosition().x, card2.back()->GetPosition().y + 50);
				toiT = vtM2;
			}
			else if (cho == 4) {
				Vector2 vtM2(card4.back()->GetPosition().x, card4.back()->GetPosition().y - 50);
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

	bai2 = nutt;

	baiRut = deckk.front();
	deckk.pop_front();

	if (luot == 0) {
		baiDanh = baiRut;
	}
	else {
		auto model = ResourceManagers::GetInstance()->GetModel("Sprite2D.nfg");
		auto shader = ResourceManagers::GetInstance()->GetShader("TextureShader");
		auto texture = ResourceManagers::GetInstance()->GetTexture(cardSpi[baiRut]);

		shared_ptr<GameButton>	m_nut = make_shared<GameButton>(model, shader, texture);

		m_nut->Set2DPosition(toiT);
		m_nut->SetSize(50, 70);
		m_nut->SetRotation(Vector3(0, 0, toiR));

		if (luot == 1) {
			de1.push_back(baiRut);
			card1.push_back(m_nut);
		}
		else if (luot == 2) {
			de2.push_back(baiRut);
			card2.push_back(m_nut);
		}
		else if (luot == 3) {
			de3.push_back(baiRut);
			card3.push_back(m_nut);
		}
		else if (luot == 4) {
			de4.push_back(baiRut);
			card4.push_back(m_nut);
		}
	}

	toiPl = cho;
	toiCh = true;
}

void GSPlay::ResetBai() {
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
	rever->SetSize(600, 300);
	rever->Set2DPosition(Globals::screenWidth / 2, Globals::screenHeight / 2);

	if (huong < 5) {
		huong = 1;
		rever->SetTexture(texture0);
	}
	else {
		huong = -1;
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