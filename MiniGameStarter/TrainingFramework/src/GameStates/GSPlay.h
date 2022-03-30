#pragma once
#include "GameStateBase.h"
using namespace std;

class Sprite2D;
class Sprite3D;
class Text;
class GameButton;
class SpriteAnimation;

class GSPlay :
	public GameStateBase
{
public:
	GSPlay();
	~GSPlay();

	void	Init() override;
	void	Exit() override;

	void	Pause() override;
	void	Resume() override;

	void	HandleEvents() override;
	void	HandleKeyEvents(int key, bool bIsPressed) override;
	void	HandleTouchEvents(int x, int y, bool bIsPressed) override;
	void	HandleMouseMoveEvents(int x, int y) override;
	void	Draw() override;
	void	Update(float deltaTime) override;
	void    ShowCard0(int c);
	void    GetDataCard();
	void    MixBai();
	void    ResetBai();
	void    DichBai();
	void    RandomDanhBai();
	void    DanhBai(int cho, int baiDanhRa, int soC, int thuMay);
	void    ChiaBai(int cho);
	void    Instantiate(Vector2 noiK, int pla, float rr, int bRut, int chia);
	void    delay(int a);
	void    ShowCardDB(int cho);
	void    AnShowCard(int cho);
	void    PlayerDanh(shared_ptr<GameButton> btnn);
	void    ResetChiSo();
	void    SelectMau(shared_ptr<GameButton> btnn);
	int m_Test;
	Vector2 vanToc(float giay, Vector2 tu, Vector2 toi);

private:
	std::shared_ptr<Sprite2D>	m_background;
	std::shared_ptr<Sprite2D>	m_dek;
	std::shared_ptr<Sprite2D>	m_card0;
	std::shared_ptr<Sprite2D>	m_den;

	//std::shared_ptr<Text>		m_score;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	list<std::shared_ptr<Text>>	m_ten;
	//std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

};
