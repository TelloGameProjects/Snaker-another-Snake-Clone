#include "../../LogicLayer/include/Game.h"

Game::Game() 
	: m_window{ sf::VideoMode{WINDOW_X, WINDOW_Y}, "Snaker" }, m_world{ sf::Vector2u{WINDOW_X, WINDOW_Y} }, 
	 m_elapsed{0.f}, m_fps{60.f}, m_frameTime{ 1.f / m_world.getSnake()->GetSpeed() }, 
	m_isHighScore{ false }, m_highScoreFilePath{ "../../../Game/data/saves/highscore.txt" }, m_gameView{ sf::FloatRect{0.f, 0.f, WINDOW_X, WINDOW_Y} },
	m_state{State::PLAYING}, m_fontPool{nullptr}
{

	m_window.setKeyRepeatEnabled(false);

	
		/*m_scoreText.setFont(m_fontPool->getFont("default_font"));
		m_scoreText.setCharacterSize(30);
		m_scoreText.setFillColor(sf::Color::White);
		m_scoreText.setOutlineColor(sf::Color::Black);
		m_scoreText.setPosition(sf::Vector2f{4.f, WINDOW_Y * .90f});

		m_highScoreText.setFont(m_fontPool->getFont("default_font"));
		m_highScoreText.setCharacterSize(30);
		m_highScoreText.setFillColor(sf::Color::White);
		m_highScoreText.setOutlineColor(sf::Color::Black);
		m_highScoreText.setPosition(sf::Vector2f{ WINDOW_X * .40f, WINDOW_Y * .90f });

		m_livesText.setFont(m_fontPool->getFont("default_font"));
		m_livesText.setCharacterSize(24);
		m_livesText.setFillColor(sf::Color::White);
		m_livesText.setOutlineColor(sf::Color::Black);
		m_livesText.setPosition(sf::Vector2f(WINDOW_X * .80f, WINDOW_Y * .01f));

		m_loseMsg.setFont(m_fontPool->getFont("default_font"));
		m_loseMsg.setCharacterSize(30);
		m_loseMsg.setFillColor(sf::Color::Red);
		m_loseMsg.setOutlineColor(sf::Color::Black);
		m_loseMsg.setPosition(sf::Vector2f(WINDOW_X * .3f, WINDOW_Y * .5f));
		m_loseMsg.setString("You have lose...");

		m_pauseMsg = m_loseMsg;
		m_pauseMsg.setString("Paused...");*/


	


	std::ifstream ifs{ m_highScoreFilePath };
	ifs >> m_highScore;
	ifs.close();


	m_window.setView(m_gameView);	
	
}

Game::~Game()
{
	
	std::ofstream ofs{ m_highScoreFilePath };
	ofs << m_highScore;
	ofs.close();
}

void Game::Run()
{
	InitFontPool();
	InitMessageDatabase();
	while (m_window.isOpen())
	{
		Input();
		if (m_elapsed >= m_frameTime)
		{			
			Update();
			m_elapsed -= m_frameTime;
		}
		
		Render();
		m_elapsed += m_clock.restart().asSeconds();
	}
}

void Game::Input()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == event.Closed)
			m_window.close();
	}

	switch (m_state)
	{
	case PENDING:
		break;
	case PLAYING:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			m_world.getSnake()->SetDirection(Direction::Up);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			m_world.getSnake()->SetDirection(Direction::Left);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			m_world.getSnake()->SetDirection(Direction::Down);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			m_world.getSnake()->SetDirection(Direction::Right);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_state = State::PAUSED;
		}
		break;
	case PAUSED:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_state = State::PLAYING;
		}
		break;
	case LOSE:
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			m_world.getSnake()->Lose();
			m_state = State::PLAYING;
		}
		break;
	default:
		break;
	};
	
	
}

void Game::Update()
{

	switch (m_state)
	{
	case PENDING:
		break;
	case PLAYING:
		m_world.Update();
		if (m_world.getSnake()->GetScore() > m_highScore)
		{
			m_highScore = true;
			m_highScore = m_world.getSnake()->GetScore();
			m_scoreText.setFillColor(sf::Color::Yellow);
		}

		m_scoreText.setString("Score: " + std::to_string(m_world.getSnake()->GetScore()));
		m_highScoreText.setString("High Score: " + std::to_string(m_highScore));
		m_livesText.setString("Lives: " + std::to_string(m_world.getSnake()->GetLives()));

		if (m_world.getSnake()->HasLost())
		{
			m_state = State::LOSE;
		}
		break;
	case PAUSED:
		break;
	case LOSE:
		m_scoreText.setFillColor(sf::Color::White);
		break;
	default:
		break;
	};
	
}

void Game::Render()
{
	m_window.clear(sf::Color(84, 140, 58, 255));
	switch (m_state)
	{
	case PENDING:
		break;
	case PLAYING:
		m_world.Render(m_window);
		DrawShadowText();
		m_window.draw(m_scoreText);
		m_window.draw(m_highScoreText);
		m_window.draw(m_livesText);
		break;
	case PAUSED:
		m_window.draw(m_pauseMsg);
		break;
	case LOSE:
		m_window.draw(m_loseMsg);
		break;
	default:
		break;
	};
	m_window.display();
}

void Game::CheckCollision()
{

}



void Game::DrawShadowText()
{
	sf::Text shadowText{ m_scoreText };
	shadowText.setFillColor(sf::Color::Black);
	shadowText.setPosition(m_scoreText.getPosition().x + 2, m_scoreText.getPosition().y + 3);
	m_window.draw(shadowText);

	shadowText={ m_highScoreText };
	shadowText.setFillColor(sf::Color::Black);
	shadowText.setPosition(m_highScoreText.getPosition().x + 2, m_scoreText.getPosition().y + 3);
	m_window.draw(shadowText);

	shadowText = { m_livesText };
	shadowText.setFillColor(sf::Color::Black);
	shadowText.setPosition(m_livesText.getPosition().x + 2, m_livesText.getPosition().y + 3);
	m_window.draw(shadowText);
}

void Game::InitMessageDatabase()
{
	m_messageDatabase.createMessage(MessageType::SCORE, m_fontPool->getFont("default_font"), "Score: ", 30);
	//m_scoreText.setFont(m_scoreFont);
	//m_scoreText.setCharacterSize(30);
	//m_scoreText.setFillColor(sf::Color::White);
	//m_scoreText.setOutlineColor(sf::Color::Black);
	m_messageDatabase.getMessage(MessageType::SCORE).setPosition(sf::Vector2f{ 4.f, WINDOW_Y * .90f });

	m_highScoreText.setFont(m_scoreFont);
	m_highScoreText.setCharacterSize(30);
	m_highScoreText.setFillColor(sf::Color::White);
	m_highScoreText.setOutlineColor(sf::Color::Black);
	m_highScoreText.setPosition(sf::Vector2f{ WINDOW_X * .40f, WINDOW_Y * .90f });

	m_livesText.setFont(m_scoreFont);
	m_livesText.setCharacterSize(24);
	m_livesText.setFillColor(sf::Color::White);
	m_livesText.setOutlineColor(sf::Color::Black);
	m_livesText.setPosition(sf::Vector2f(WINDOW_X * .80f, WINDOW_Y * .01f));

	m_loseMsg.setFont(m_scoreFont);
	m_loseMsg.setCharacterSize(30);
	m_loseMsg.setFillColor(sf::Color::Red);
	m_loseMsg.setOutlineColor(sf::Color::Black);
	m_loseMsg.setPosition(sf::Vector2f(WINDOW_X * .3f, WINDOW_Y * .5f));
	m_loseMsg.setString("You have lose...");

	m_pauseMsg = m_loseMsg;
	m_pauseMsg.setString("Paused...");

	m_messageDatabase.createMessage(MessageType::SCORE, m_fontPool->getFont("default_font"), "Score: ");
}

void Game::InitFontPool()
{
	Config::FontLoader l_fontLoader;
	m_fontPool = l_fontLoader.load(m_fontConfigFilePath);
}

bool Game::IsHighScore() const
{
	return m_isHighScore;
}


