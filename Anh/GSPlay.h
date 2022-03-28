#pragma once
#include "GameStateBase.h"

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
	void	Update(float deltaTime) override;
	void	Draw() override;
	void    ShowCard0(int c);
	void    GetDataCard();
	void    MixBai();
	void    ResetBai();
	void    ChiaBai(int cho);
    int m_Test;
	Vector2 vanToc(float giay, Vector2 tu, Vector2 toi);

private:
	std::shared_ptr<Sprite2D>	m_background;
	std::shared_ptr<Sprite2D>	m_dek;
	std::shared_ptr<Sprite2D>	m_card0;
	std::shared_ptr<Sprite2D>	m_den;

	//std::shared_ptr<Text>		m_score;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	//std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

};

