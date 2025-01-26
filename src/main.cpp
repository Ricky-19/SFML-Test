#include "../lib/MainFrame.h"
#include "../lib/SfmlFun.h"
using namespace std;
using namespace sf;

int main ()
{
    // Creazione della finestra
    RenderWindow window(VideoMode::getDesktopMode(), "Relics & Ruins", Style::Default);
    window.setPosition(Vector2i(0, 0)); // Posiziona la finestra nell'angolo in alto a sinistra
    window.setFramerateLimit(60);

    const auto cursorHand = Cursor::createFromSystem(Cursor::Type::Hand).value();
    const auto cursorArrow = Cursor::createFromSystem(Cursor::Type::Arrow).value();
    Vector2i mousePosition;

    Texture textBoxTexture;
    if (!textBoxTexture.loadFromFile("../assets/Textures/Backgrounds/TextBoxBackground.png")) 
        return -1;
    
    // TextBox e posizione relativa alla finestra
    RectangleShape textBox;
    textBox.setFillColor(Color::White);
    textBox.setTexture(&textBoxTexture);
    const float marginX = 20.0f;
    const float marginY = 10.0f;
    const Vector2f rectangleSize(
        window.getSize().x - 2 * marginX,
        window.getSize().y / 4 // Rettangolo alto un quarto della finestra
    );
    textBox.setSize(rectangleSize);
    textBox.setPosition(Vector2f(marginX, window.getSize().y - rectangleSize.y - marginY));

    Font textBoxFont;
    if (!textBoxFont.openFromFile("../assets/Fonts/TextBox.ttf"))
        return -1; 

    Text textBoxText(textBoxFont, "Hello World", 100);
    textBoxText.setFont(textBoxFont);
    textBoxText.setCharacterSize(24); // Dimensione del testo
    textBoxText.setFillColor(Color::Black);
     

    string fullText = "";
    string currentText = "";
    Clock clock;
    float delay = 0.02f; // Delay in secondi tra una lettera e l'altra
    bool hasJustStarted = true; // Flag per l'inizializzazione
    bool isPrintingText = false; // Flag per gestire la stampa graduale del testo

    Text tutChoiceText(textBoxFont, "Do you want to start from scratch?", 40);
    tutChoiceText.setPosition(Vector2f(textBox.getPosition().x + 10, textBox.getPosition().y + 10));
    tutChoiceText.setFillColor(Color::Black);

    RectangleShape tutBoxSelectionYes;
    tutBoxSelectionYes.setFillColor(Color(0,0,0,0));
    tutBoxSelectionYes.setSize(Vector2f(100, 55));
    tutBoxSelectionYes.setPosition(Vector2f(textBox.getPosition().x + 900, textBox.getPosition().y + 10));
    Text tutSelectionYes(textBoxFont, "YES", 40);
    tutSelectionYes.setPosition(Vector2f(tutBoxSelectionYes.getPosition().x + 5, tutBoxSelectionYes.getPosition().y + 5));
    tutSelectionYes.setFillColor(Color::Black);
    
    RectangleShape tutBoxSelectionNo;
    tutBoxSelectionNo.setFillColor(Color(0,0,0,0));
    tutBoxSelectionNo.setSize(Vector2f(66, 55));
    tutBoxSelectionNo.setPosition(Vector2f(textBox.getPosition().x + 1040, textBox.getPosition().y + 10));
    Text tutSelectionNo(textBoxFont, "NO", 40);
    tutSelectionNo.setPosition(Vector2f(tutBoxSelectionNo.getPosition().x + 5, tutBoxSelectionNo.getPosition().y + 5));
    tutSelectionNo.setFillColor(Color::Black);


    // Sfondo
    Texture backgroundTextureValoria;
    if (!backgroundTextureValoria.loadFromFile("../assets/Textures/Backgrounds/Valoria/CapitalLobby.png")) 
    {
        cerr << "Errore: impossibile caricare lo sfondo!" << endl;
        return -1;
    }
    Sprite backgroundSprite(backgroundTextureValoria);

    string selection = "";

    
    bool inputBoxSelected = false;

    Text characterSelectionText(textBoxFont, "Select your character", 40);
    characterSelectionText.setPosition(Vector2f(textBox.getPosition().x + 10, textBox.getPosition().y - 550));
    characterSelectionText.setFillColor(Color::Black);

    RectangleShape inputBox;
    Texture inputBoxTexture;
    inputBox.setFillColor(Color(0,0,0,0));
    inputBox.setOutlineColor(Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setSize(Vector2f(textBox.getSize().x-50, 70));
    inputBox.setPosition(Vector2f(characterSelectionText.getPosition().x, characterSelectionText.getPosition().y + 70));

    String playerInput;
    Text playerText (textBoxFont, "", 36);
    playerText.setFillColor(Color::Black);
    playerText.setPosition(Vector2f(inputBox.getPosition().x + 10, inputBox.getPosition().y + 10));

    string characterName = "";
    int characterCreationStep = 1;

    Text characterNamesList(textBoxFont, "No Characters Found", 40);
    characterNamesList.setFillColor(Color::Black);
    characterNamesList.setPosition(Vector2f(inputBox.getPosition().x, inputBox.getPosition().y + 150));

    Text newCharacterCreationText(textBoxFont, "Create Your Character\n\n\n Name:\n\n Race:\n\n Sex:\n\n Difficulty:", 40);
    newCharacterCreationText.setFillColor(Color::Black);
    newCharacterCreationText.setPosition(Vector2f(textBox.getPosition().x + 10, textBox.getPosition().y - 550));

    string newCharacterName = "";
    Text newCharacterNameText(textBoxFont, "", 40);
    newCharacterNameText.setFillColor(Color::Black);
    newCharacterNameText.setPosition(Vector2f(newCharacterCreationText.getPosition().x + 155, newCharacterCreationText.getPosition().y +160));

    string newCharacterRace = "";
    Text newCharacterRaceText(textBoxFont, "", 40);
    newCharacterRaceText.setFillColor(Color::Black);
    newCharacterRaceText.setPosition(Vector2f(newCharacterCreationText.getPosition().x + 150, newCharacterCreationText.getPosition().y + 265));

    string newCharacterSex = "";
    Text newCharacterSexText(textBoxFont, "", 40);
    newCharacterSexText.setFillColor(Color::Black);
    newCharacterSexText.setPosition(Vector2f(newCharacterCreationText.getPosition().x + 120, newCharacterCreationText.getPosition().y + 370));

    string newCharacterDifficulty = "";
    Text newCharacterDifficultyText(textBoxFont, "", 40);
    newCharacterDifficultyText.setFillColor(Color::Black);
    newCharacterDifficultyText.setPosition(Vector2f(newCharacterCreationText.getPosition().x + 270, newCharacterCreationText.getPosition().y + 475));

    bool characterCreationFinished = false;

    Text invalidInsertionText(textBoxFont, "Invalid Insertion", 40);
    invalidInsertionText.setFillColor(Color::Red);
    invalidInsertionText.setPosition(Vector2f(inputBox.getPosition().x + 10, inputBox.getPosition().y + 10));


    // Ciclo principale del gioco
    while (window.isOpen()) 
    {
        // Gestione degli eventi
        optional<Event> event;
        while ((event = window.pollEvent())) {
            if (event->is<Event::Closed>()) {
                window.close();
            } else if (const auto* textEvent = event->getIf<Event::TextEntered>()) {
                if (textEvent->unicode < 128 && inputBoxSelected) {
                    playerInput += static_cast<char>(textEvent->unicode);
                    playerText.setString(playerInput);
                }
            }
        }
        mousePosition = Mouse::getPosition(window);

        // Sfondo
        resizeBackground(backgroundSprite, window);
        window.clear();
        window.draw(backgroundSprite);

        //Tutorial
        if (hasJustStarted) 
        {
            hasJustStarted = false;
            isPrintingText = true;
            initializeTextBoxForTutorial(textBox, textBoxText, window, fullText, currentText);
        } 
        if (isPrintingText) 
        {
            isPrintingText = !handleTextPrintingForTutorial(clock, delay, fullText, currentText, textBoxText, window);
            window.draw(textBox);
            window.draw(textBoxText);
        } else if (selection == "YES")
        {
            window.clear();
            window.draw(backgroundSprite);
            window.draw(textBox);
            window.draw(newCharacterCreationText);
            window.draw(inputBox);
            window.draw(playerText);
            switch (characterCreationStep)
            {
                case 1:
                    newCharacterNameText.setString(playerText.getString());
                    break;
                case 2:
                    newCharacterRaceText.setString(playerText.getString());
                    break;
                case 3:
                    newCharacterSexText.setString(playerText.getString());
                    break;
                case 4:
                    newCharacterDifficultyText.setString(playerText.getString());
                    break;
                default:
                    break;
            }
            window.draw(newCharacterNameText);
            window.draw(newCharacterRaceText);
            window.draw(newCharacterSexText);
            window.draw(newCharacterDifficultyText);

            if(checkForMouseClick(inputBox, window, mousePosition))
            {
                inputBoxSelected = true;
                inputBox.setOutlineColor(Color(20, 90, 200));
            } else if(Mouse::isButtonPressed(Mouse::Button::Left))
            {
                inputBoxSelected = false;
                inputBox.setOutlineColor(Color::Black);
            }

            if (inputBox.getOutlineColor() == Color(20, 90, 200) && Keyboard::isKeyPressed(Keyboard::Key::Enter))
            {
                switch (characterCreationStep)
                {
                    case 1:
                        newCharacterName = newCharacterNameText.getString();
                        characterCreationStep ++;
                        break;
                    case 2:
                        if (find(racesFirst, racesLast, newCharacterRaceText.getString()) != RACES.end()) 
                        {
                            // Se la razza è valida, salvala e passa allo step successivo
                            newCharacterRace = newCharacterRaceText.getString();
                            characterCreationStep++;
                        } else 
                        {
                            // Se la razza è invalida, mostra il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Red);
                            window.draw(invalidInsertionText);
                            window.display();
                            Clock clock;
                            while (clock.getElapsedTime().asSeconds() < 3.0f) 
                            {
                                // Lascio che il programma risponda agli eventi durante l'attesa
                                 optional<Event> event;
                                while ((event = window.pollEvent())) {
                                    if (event->is<Event::Closed>()) {
                                        window.close();
                                    }
                                }
                            }
                            // Nascondi il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Transparent);
                            window.draw(invalidInsertionText);
                            window.display();
                        }
                        break;
                    case 3:
                        if (newCharacterSexText.getString() == "M" || newCharacterSexText.getString() == "F") 
                        {
                            // Se il sesso è valido, salvalo e passa allo step successivo
                            newCharacterSex = newCharacterSexText.getString();
                            characterCreationStep++;
                        } else 
                        {
                            // Se il sesso è invalido, mostra il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Red);
                            window.draw(invalidInsertionText);
                            window.display();
                            Clock clock;
                            while (clock.getElapsedTime().asSeconds() < 3.0f) 
                            {
                                // Lascio che il programma risponda agli eventi durante l'attesa
                                optional<Event> event;
                                while ((event = window.pollEvent())) {
                                    if (event->is<Event::Closed>()) {
                                        window.close();
                                    }
                                }
                            }
                            // Nascondi il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Transparent);
                            window.draw(invalidInsertionText);
                            window.display();
                        }
                        break;
                    case 4:
                        if (find(difficultiesFirst, difficultiesLast, newCharacterDifficultyText.getString()) != DIFFICULTIES.end()) 
                        {
                            // Se la difficoltà è valida, salvala e passa allo step successivo
                            newCharacterDifficulty = newCharacterDifficultyText.getString();
                            characterCreationStep++;
                            characterCreationFinished = true;
                        } else 
                        {
                            // Se la difficoltà è invalida, mostra il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Red);
                            window.draw(invalidInsertionText);
                            window.display();
                            Clock clock;
                            while (clock.getElapsedTime().asSeconds() < 3.0f) 
                            {
                                // Lascio che il programma risponda agli eventi durante l'attesa
                                optional<Event> event;
                                while ((event = window.pollEvent())) {
                                    if (event->is<Event::Closed>()) {
                                        window.close();
                                    }
                                }
                            }
                            // Nascondi il messaggio di errore
                            invalidInsertionText.setFillColor(sf::Color::Transparent);
                            window.draw(invalidInsertionText);
                            window.display();
                        }
                        break;
                    default:
                        break;
                }
                inputBoxSelected = false;
                inputBox.setOutlineColor(Color::Black);
                playerInput.clear();
                playerText.setString(playerInput);
            
                //Character playerCharacter = fromJSONtoCharacter(characterName);
                //start_game(playerCharacter); 
            } else if (characterCreationFinished)
            {
                Character playerCharacter (newCharacterName, newCharacterRace, newCharacterSex, newCharacterDifficulty);
                playerCharacter.write_character_to_json(playerCharacter);
                //start_game(playerCharacter);
            }
        } else if (selection == "NO")
        {
            window.clear();
            window.draw(backgroundSprite);
            window.draw(textBox);
            window.draw(characterSelectionText);
            window.draw(inputBox);
            window.draw(playerText);

            if(checkForMouseClick(inputBox, window, mousePosition))
            {
                inputBoxSelected = true;
                inputBox.setOutlineColor(Color(20, 90, 200));
            } else if(Mouse::isButtonPressed(Mouse::Button::Left))
            {
                inputBoxSelected = false;
                inputBox.setOutlineColor(Color::Black);
            }
            if (inputBox.getOutlineColor() == Color(20,90,200) && Keyboard::isKeyPressed(Keyboard::Key::Enter))
            {
                inputBoxSelected = false;
                inputBox.setOutlineColor(Color::Black);
                characterName = playerText.getString().toAnsiString();
                playerInput.clear();
                playerText.setString(playerInput);
                
                Character playerCharacter = fromJSONtoCharacter(characterName);
                //start_game(playerCharacter); 
            }

            characterNamesList = selectCharacter(characterNamesList);
            window.draw(characterNamesList);
        } else 
        {
            window.draw(textBox);
            window.draw(textBoxText);
            window.draw(tutChoiceText);
            window.draw(tutBoxSelectionYes);
            window.draw(tutSelectionYes);
            window.draw(tutBoxSelectionNo);
            window.draw(tutSelectionNo);
            // Check for mouse hover over rectangle
            if (isMouseHovering(tutBoxSelectionYes, window, mousePosition))
                window.setMouseCursor(cursorHand);
            else if (isMouseHovering(tutBoxSelectionNo, window, mousePosition))
                window.setMouseCursor(cursorHand);
            else
                window.setMouseCursor(cursorArrow);

            // Check for mouse click rectangle
            if (checkForMouseClick(tutBoxSelectionYes, window, mousePosition)){
                selection = "YES";
                window.setMouseCursor(cursorArrow);
            }else if (checkForMouseClick(tutBoxSelectionNo, window, mousePosition)){
                selection = "NO";
                window.setMouseCursor(cursorArrow);
            }
        }

        window.display();
    }
    return 0;
}
