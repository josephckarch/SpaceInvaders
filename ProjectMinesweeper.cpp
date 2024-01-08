#include <SFML/Graphics.hpp>
#include "Random.h"
#include "TextureManager.h"
#include "Board.h"
#include "Tile.h"
#include <iostream>
#include <fstream>
using namespace std;

int GetBoardData(ifstream& file);
void DrawBoard(sf::RenderWindow& window, Board& board);
void FillBlankBoard(Board& board);
void ConnectTiles(Board& board);
void DrawSmile(sf::RenderWindow& window, Board& board);
bool Duplicate(vector<int> &mines, int num);
void GetMines(Board& board);
void DrawMines(sf::RenderWindow& window, Board& board);
void DrawButtons(sf::RenderWindow& window, Board& board);
void DrawDigits(sf::RenderWindow& window, Board& board);
void DrawNegative(sf::RenderWindow& window, Board& board);
void AssignDigit(Board& board, sf::Sprite& sprite, int digit, int sequence);
void RevealTile(Board& board, sf::Vector2i mousePos);
void RecursiveReveal(Board& board, Tile* currTile);
void FlagTile(Board& board, sf::Vector2i mousePos);
void ToggleMines(Board& board, sf::Vector2i mousePos);
void ResetGame(Board& board, sf::Vector2i mousePos);
void TestBoard(Board& board, sf::Vector2i mousePos);
void CopyBoard(Board& board, ifstream& inFile);
void CheckGameWon(sf::RenderWindow& window, Board& board);
void CheckGameLost(sf::RenderWindow& window, Board& board);
void StagnantBoard(sf::RenderWindow& window, Board& board);
void DrawToWindow(sf::RenderWindow& window, Board& board);


int main()
{
    // Get window size from "config.cfg"
    ifstream boardData;
    boardData.open("boards/config.cfg");
    Board board;
    board.width = GetBoardData(boardData);
    board.height = GetBoardData(boardData);
    board.numMines = GetBoardData(boardData);
    boardData.close();

    // Number of tiles in game board; for use later
    board.numTiles = board.width * board.height;
    sf::RenderWindow window(sf::VideoMode(board.width * 32, (board.height * 32) + 100), "Minesweeper!");

    // Fills board's vector<Tile> tiles with tile_hidden textures
    FillBlankBoard(board);

    // Make vector of length [mines] that contains random numbers between (0, width*height) 
    GetMines(board);

    // Make vector of 5 sprite objects for the bottom buttons
    for (unsigned int i = 0; i < 5; i++) {
        sf::Sprite newSprite;
        if (i == 0)
            newSprite.setTexture(TextureManager::GetTexture("face_happy"));
        board.buttonSprites.push_back(newSprite);
    }
    
    while (window.isOpen())
    {
        // Event loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // Click event
            if (event.type == sf::Event::MouseButtonPressed) {   
                // get current mouse position
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Right Click
                if (event.mouseButton.button == sf::Mouse::Right) {
                    FlagTile(board, mousePos);
                }
                // Left Click
                if (event.mouseButton.button == sf::Mouse::Left) {
                    RevealTile(board, mousePos);
                    ToggleMines(board, mousePos);
                    ResetGame(board, mousePos);
                    TestBoard(board, mousePos);
                }    

            }

            // Close window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        CheckGameLost(window, board);
        CheckGameWon(window, board);

        //  Check pdf expectations for functionality --
        // 
        // Need to add functionality to: 
        //  Implement game ending features
        //  End game if bomb is revealed; need a way to have an infinite loop when a bomb is revealed
        //     and break the loop when user clickes smiley

        // Draw all tiles stored in the board object
        DrawToWindow(window, board);
    }

    TextureManager::Clear();
    return 0;
}

// ========== Function Definitions ==========

int GetBoardData(ifstream& file) {
    string line;
    getline(file, line, '\n');
    return stoi(line);
}
void DrawBoard(sf::RenderWindow &window,Board &board) {
    for (unsigned int i = 0; i < board.tiles.size(); i++) {
        board.tiles[i].Draw(window);
    }
}
void FillBlankBoard(Board &board)
{
    // Fill board with hidden tile image
    // iterate for all number of tiles
    for (size_t i = 0; i < board.height; i++) {
        for (size_t j = 0; j < board.width; j++) {
            Tile newTile;
            newTile.SetPosition(j * 32, i * 32);
            board.tiles.push_back(newTile);            
        }
    }

    // Fill the vector<Tile*> in each Tile
    ConnectTiles(board);
}
void ConnectTiles(Board& board)
{
    // Fill the vector<Tile*> in each Tile
    // 
    // need to account for top, left, right, bottom edges individually 
    for (unsigned int i = 0; i < board.height; i++) {
        // new row
        for (unsigned int j = 0; j < board.width; j++) {
            // next column over
            int currIndex = (i * board.width) + j;
            Tile* currTile = &board.tiles[currIndex];

            if (j != 0) { // if not on left edge
                Tile* adjTile1 = &board.tiles[currIndex - 1];
                currTile->adjacentTiles.push_back(adjTile1); // left tile
                if (i != 0) { // if not on top edge
                    Tile* adjTile2 = &board.tiles[currIndex - board.width - 1];
                    currTile->adjacentTiles.push_back(adjTile2); // top left
                }
                if (i != board.height - 1) { // if not on bottom edge
                    Tile* adjTile3 = &board.tiles[currIndex + board.width - 1];
                    currTile->adjacentTiles.push_back(adjTile3); // bottom left
                }
            }
            if (j != board.width - 1) { // if not on right edge
                Tile* adjTile4 = &board.tiles[currIndex + 1];
                currTile->adjacentTiles.push_back(adjTile4); // right tile
                if (i != 0) { // if not on top edge
                    Tile* adjTile5 = &board.tiles[currIndex - board.width + 1];
                    currTile->adjacentTiles.push_back(adjTile5); // top right
                }
                if (i != board.height - 1) { // if not on bottom edge
                    Tile* adjTile6 = &board.tiles[currIndex + board.width + 1];
                    currTile->adjacentTiles.push_back(adjTile6); // bottom right
                }
            }
            if (i != 0) { // if not on top edge
                Tile* adjTile7 = &board.tiles[currIndex - board.width];
                currTile->adjacentTiles.push_back(adjTile7); // above
            }
            if (i != board.height - 1) { // if not on bottom edge
                Tile* adjTile8 = &board.tiles[currIndex + board.width];
                currTile->adjacentTiles.push_back(adjTile8); // below
            }
        }
    }
}
void DrawSmile(sf::RenderWindow &window, Board& board)
{
    // Draw it in correct location
    board.buttonSprites[0].setPosition((board.width * 32 / 2) - (board.buttonSprites[0].getGlobalBounds().width / 2), board.height * 32);
    window.draw(board.buttonSprites[0]);
}
bool Duplicate(vector<int> &mines, int num)
{
    for (unsigned int i = 0; i < mines.size(); i++) {
        if (mines[i] == num)
            return true;
    }
    return false;
}
void GetMines(Board& board)
{
    // clear out the old mineLocations
    board.mineLocations.clear();

    // Get new, random mine locations
    for (unsigned int i = 0; i < board.numMines; i++) {
        unsigned int randomNum;
        while (true) {
            randomNum = Random::Int(0, board.numTiles-1);
            // If it's a duplicate, get a new random number
            if (Duplicate(board.mineLocations, randomNum))
                continue;
            else {  // break out of while loop
                board.tiles[randomNum].hasMine = true;  // make Tile know it has a mine
                // let adjacent tiles know there is a mine here
                
                break;
            }
        }

        board.mineLocations.push_back(randomNum);
    }

    // Assign correct numberSprite to each tile here and at every resetGame; also correct numAdjMines
    for (unsigned int i = 0; i < board.tiles.size(); i++)
        board.tiles[i].AssignNumber();
}
void DrawMines(sf::RenderWindow& window, Board &board)
{
    // iterate thru board.tiles and draw mines on top of tiles
    if (board.showMines) {
        for (unsigned int i = 0; i < board.tiles.size(); i++) {
            board.tiles[i].DrawWithMines(window);
        }
    }
}
void DrawButtons(sf::RenderWindow& window, Board& board)
{
    // Draw the smiley face :)
    DrawSmile(window, board);

    // Need to draw 4 sprites
    board.buttonSprites[1].setTexture(TextureManager::GetTexture("debug"));
    board.buttonSprites[2].setTexture(TextureManager::GetTexture("test_1"));
    board.buttonSprites[3].setTexture(TextureManager::GetTexture("test_2"));
    board.buttonSprites[4].setTexture(TextureManager::GetTexture("test_3"));

    const int spriteWidth = board.buttonSprites[1].getGlobalBounds().width;
    const int xPos = ((board.width * 32) / 2) - (spriteWidth/2) + (spriteWidth * 2);
    const int yPos = board.height * 32;

    board.buttonSprites[1].setPosition(xPos, yPos);
    board.buttonSprites[2].setPosition(xPos + spriteWidth, yPos);
    board.buttonSprites[3].setPosition(xPos + 2*spriteWidth, yPos);
    board.buttonSprites[4].setPosition(xPos + 3*spriteWidth, yPos);

    // Draw the testing buttons
    for (unsigned int i = 1; i < 5; i++)
        window.draw(board.buttonSprites[i]);
    
}
void DrawDigits(sf::RenderWindow& window, Board& board)
{
    // draw corresponding digits to represent number of flags left; always 3 digits displayed (050)
        // need to account for negatives

    // number to display
    int flagsLeft = board.numMines + board.numFlags;
    sf::Sprite sprite1(TextureManager::GetTexture("digits"));
    sf::Sprite sprite2(TextureManager::GetTexture("digits"));
    sf::Sprite sprite3(TextureManager::GetTexture("digits"));

    bool negative = flagsLeft < 0;
    // Use Division and MOD to get digits
    int digit1 = flagsLeft / 100;     
    int digit2 = (flagsLeft % 100) / 10;
    int digit3 = flagsLeft % 10; 

    if (negative) // draw negative 
        DrawNegative(window, board);

    AssignDigit(board, sprite1, abs(digit1), 1);
    AssignDigit(board, sprite2, abs(digit2), 2);
    AssignDigit(board, sprite3, abs(digit3), 3);

    window.draw(sprite1);
    window.draw(sprite2);
    window.draw(sprite3);
}
void DrawNegative(sf::RenderWindow& window, Board& board) {
    sf::Sprite negativeSign(TextureManager::GetTexture("digits"));

    negativeSign.setPosition(0, board.height * 32);
    negativeSign.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));

    window.draw(negativeSign);
}
void AssignDigit(Board& board, sf::Sprite &sprite, int digit, int sequence)
{
    // Helper function to set the digit sprites the correct position and texture portion
    // multiply: sequence * 21 to get the correct xPos

    sprite.setPosition(sequence * 21, board.height * 32);  
    sprite.setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
}
void RevealTile(Board& board, sf::Vector2i mousePos) 
{
    // if click is on a tile, reveal it if it's hidden
    for (unsigned int i = 0; i < board.tiles.size(); i++) {
        if (board.tiles[i].tileSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            if (board.tiles[i].hidden) {
                // if tile is flagged, do nothing
                if (board.tiles[i].flagged)
                    break;

                // reveal tile
                board.tiles[i].tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                RecursiveReveal(board, &board.tiles[i]);
                board.tiles[i].hidden = false;

                // if the tile has a mine, the game is over
                if (board.tiles[i].hasMine)
                    board.gameLost = true;
            }
        }
    }

}
void RecursiveReveal(Board& board, Tile* currTile)
{
    // base cases:
    if (currTile->hasMine)
        return;
    if (!currTile->hidden)  // if already revealed
        return;
    if (currTile->flagged) // if flagged, don't reveal it
        return;
    if (currTile->numAdjMines > 0) {
        currTile->tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
        currTile->hidden = false;
        return;
    }
    
    // if currTile does not have a mine and has no nearby mines
    if (!currTile->hasMine) {
        currTile->tileSprite.setTexture(TextureManager::GetTexture("tile_revealed"));
        currTile->hidden = false;

        // If it's flagged, take the flag off of it
        if (currTile->flagged) {
            currTile->flagged = false;
            board.numFlags++;
        }

        // call RecursiveReveal on all nearby tiles
        for (unsigned int i = 0; i < currTile->adjacentTiles.size(); i++) {
            RecursiveReveal(board, currTile->adjacentTiles[i]);
        }

    }
}
void FlagTile(Board& board, sf::Vector2i mousePos) 
{
    // if the click is on a tile, toggle flag
    // loop for all tiles and check if it contains mousePos
    for (unsigned int i = 0; i < board.tiles.size(); i++) {
        if (board.tiles[i].tileSprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            // toggle flag
            board.tiles[i].flagged = !board.tiles[i].flagged;
            // if placing a flag, increment numFlags
            if (!board.tiles[i].flagged)
                board.IncFlags();
            else   // decrease numFlags
                board.DecFlags();
            break;
        }
    }
}
void ToggleMines(Board& board, sf::Vector2i mousePos)
{
    // if mouse click is on the reveal mines button
    if (board.buttonSprites[1].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        board.showMines = !board.showMines;
    }
}
void ResetGame(Board& board, sf::Vector2i mousePos) 
{
    // if click was on the smiley 
    if (board.buttonSprites[0].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        // Reset flags, mines, etc.
        ifstream inFile;
        inFile.open("boards/config.cfg");
        board.width = GetBoardData(inFile);
        board.height = GetBoardData(inFile);
        board.numMines = GetBoardData(inFile);
        inFile.close();

        board.Reset();

        for (unsigned int i = 0; i < board.tiles.size(); i++) {
            // Make all tiles hidden, !flagged, !hasMine
            board.tiles[i].hidden = true;
            board.tiles[i].flagged = false;
            board.tiles[i].hasMine = false;
            board.tiles[i].numAdjMines = 0;
            board.tiles[i].tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
        }

        // Create new mine locations
        GetMines(board);
    }
}
void TestBoard(Board& board, sf::Vector2i mousePos)
{
    ifstream inFile;

    // if test board#1
    if (board.buttonSprites[2].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        inFile.open("boards/testboard1.brd");

        // Wherever there is a 1, set that Tile.hasMine = true
        CopyBoard(board, inFile);
    }

    // if test board#2
    if (board.buttonSprites[3].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        inFile.open("boards/testboard2.brd");

        CopyBoard(board, inFile);
    }

    // if test board#3
    if (board.buttonSprites[4].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        inFile.open("boards/testboard3.brd");

        CopyBoard(board, inFile);
    }
}
void CopyBoard(Board& board, ifstream& inFile)
{
    // Wherever there is a 1, set that Tile.hasMine = true
    string fullLine;
    int numMines = 0;
    int width = 0;
    int height = 0;
    board.Reset();

        // Iterate for every row
    int i = 0;
    while (getline(inFile, fullLine)) {
        width = fullLine.size();
        // Iterate for every column
        for (unsigned int j = 0; j < width; j++) {
            int currIndex = (i * width) + j;
            Tile* currTile = &board.tiles[currIndex];

            if (fullLine.at(0) == '1') {
                currTile->hasMine = true;
                numMines++;
            }
            else if (fullLine.at(0) == '0')
                currTile->hasMine = false;

            // Reset other Tile data members like flagged, hidden, etc
            currTile->hidden = true;
            currTile->tileSprite.setTexture(TextureManager::GetTexture("tile_hidden"));
            currTile->flagged = false;
            currTile->numAdjMines = 0;

            fullLine = fullLine.substr(1); // cut off first char

        }
        i++;
    }

    // Need to AssignNumber again
    for (unsigned int i = 0; i < board.tiles.size(); i++)
        board.tiles[i].AssignNumber();

    // Also need to reset the digits according to numMines
    board.numMines = numMines;
    board.numFlags = 0;
}
void CheckGameWon(sf::RenderWindow& window, Board& board)
{
    bool winStatus = board.CheckTilesRevealed();

    // if all tiles have been revealed
    if (winStatus) {
        cout << "You win!\n";
        board.buttonSprites[0].setTexture(TextureManager::GetTexture("face_win"));

        // Mark all tiles with mines as flagged
        for (unsigned int i = 0; i < board.tiles.size(); i++) {
            if (board.tiles[i].hasMine) {
                if (!board.tiles[i].flagged) {
                    board.tiles[i].flagged = true;
                    board.numFlags--;
                }
            }
        }

        // Draw the board that will be stagnant until button is pressed
        DrawToWindow(window, board);

        // Make user unable to interact with board
        StagnantBoard(window, board);
    }
}
void CheckGameLost(sf::RenderWindow& window, Board& board)
{
    if (board.gameLost) {
        cout << "You lost!\n";
        board.buttonSprites[0].setTexture(TextureManager::GetTexture("face_lose"));

        // Draw the board that will be stagnant until button is pressed
        board.showMines = true;
        DrawToWindow(window, board);

        // Make user unable to interact with board
        StagnantBoard(window, board);
        board.showMines = false;
    }
}
void StagnantBoard(sf::RenderWindow& window, Board& board)
{
    bool breakLoop = false;

    // infinite loop until user clicks a button
    while (true) {
        if (breakLoop)
            break;

        sf::Event event;
        while (window.pollEvent(event)) {
            // Mouse position
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            if (event.mouseButton.button == sf::Mouse::Left) {
                if (board.buttonSprites[0].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    ResetGame(board, mousePos);
                    breakLoop = true;
                }
                if (board.buttonSprites[2].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    TestBoard(board, mousePos);
                    breakLoop = true;
                }
                if (board.buttonSprites[3].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    TestBoard(board, mousePos);
                    breakLoop = true;
                }
                if (board.buttonSprites[4].getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    TestBoard(board, mousePos);
                    breakLoop = true;
                }
            }
        }
    }
}
void DrawToWindow(sf::RenderWindow& window, Board& board)
{
    window.clear(sf::Color(255,255,255,255));
    DrawBoard(window, board);
    DrawMines(window, board);
    DrawButtons(window, board);
    DrawDigits(window, board);
    window.display();
}