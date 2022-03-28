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

    int m_Test;

private:
	std::shared_ptr<Sprite2D>	m_background;
	std::shared_ptr<Sprite2D>	m_bobai;
	std::shared_ptr<Sprite2D>	m_card0;
	std::shared_ptr<Sprite2D>	m_den;
	//std::shared_ptr<Text>		m_score;
	std::list<std::shared_ptr<GameButton>>	m_listButton;
	//std::list<std::shared_ptr<SpriteAnimation>>	m_listAnimation;

	std::list<std::shared_ptr<GameButton>>	m_listCard0;
	std::list<std::shared_ptr<Sprite2D>>	m_listCard1;
	std::list<std::shared_ptr<Sprite2D>>	m_listCard2;
	std::list<std::shared_ptr<Sprite2D>>	m_listCard3;

	std::list<std::shared_ptr<int>>	m_mauCard0;
	std::list<std::shared_ptr<int>>	m_mauCard1;
	std::list<std::shared_ptr<int>>	m_mauCard2;
	std::list<std::shared_ptr<int>>	m_mauCard3;

	std::list<std::shared_ptr<int>>	m_soCard0;
	std::list<std::shared_ptr<int>>	m_soCard1;
	std::list<std::shared_ptr<int>>	m_soCard2;
	std::list<std::shared_ptr<int>>	m_soCard3;

};

