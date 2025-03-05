#include <iostream>
#include <vector>
#include <windows.h>
using namespace std;

enum class Color { WHITE, BLACK };
enum class XY {
    startX = 2,
    startY = 4,
    diffX = 4,
    diffY = 8,
    term = 8
};

const int MAP_SIZE_X = 33;
const int MAP_SIZE_Y = 65;
char board[MAP_SIZE_X][MAP_SIZE_Y];
bool checkmate = false;

class Piece {
protected:
    string pieceName;
    Color color;
    int pieceX, pieceY;
public:
    Piece(string name, Color c, int x, int y)
    {
        this->pieceName = name;
        this->color = c;
        this->pieceX = x;
        this->pieceY = y;
    }
    virtual ~Piece() {}

    virtual bool move(int x, int y) = 0;

    string getName()
    {
        return pieceName;
    }
    Color getColor()
    {
        return color;
    }
    int getX()
    {
        return pieceX;
    }
    int getY()
    {
        return pieceY;
    }
    void setPosition(int x, int y)
    {
        pieceX = x; pieceY = y;
    }
};
void setcolor(unsigned short text, unsigned short back)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text | (back << 4));
}



class Pawn : public Piece {
public:
    // 생성자: 폰을 초기 위치에 배치
    Pawn(Color c, int x, int y) : Piece("P", c, x, y) {
        // 폰을 보드의 초기 위치에 배치
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';
    }

    bool move(int x, int y) override {
        int direction = (color == Color::WHITE) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);  // 흰색은 -1, 검정색은 1 방향으로 이동

        // 현재 위치에서 폰을 제거
        board[pieceY][pieceX] = ' ';

        // 일반 이동: 1칸 전진
        if (x == pieceX && (y == pieceY + direction || y == pieceY - direction)) {
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // 새로운 위치에 폰 배치
            return true;
        }

        // 첫 번째 이동에서 2칸 전진 (앞으로만)
        if (/*pieceY == (color == Color::WHITE ? 6 : 1) &&*/ x == pieceX && abs(y - pieceY) == 8) {
            int middleY = pieceY + direction;  // 중간 칸
            // 중간 칸이 비어 있는지 확인
            if (board[middleY][pieceX] == ' ') {
                pieceX = x; pieceY = y;
                board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // 새로운 위치에 폰 배치
                return true;
            }
        }

        // 대각선으로 상대방 말을 잡는 경우
        if (abs(x - pieceX) == 8 && (y == pieceY + direction || y == pieceY - direction) && board[y][x] != ' ') {
            // 상대 색의 말이 있는지 확인하고, 잡을 수 있으면 잡기
            /*if ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B')) {*/
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // 상대 말을 제거
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // 새로운 위치에 폰 배치
            return true;
            //}
        }

        // 이동이 불가능한 경우, 원래 위치로 되돌리기
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';
        return false;
    }
};
class Rook : public Piece {
public:
    Rook(Color c, int x, int y) : Piece("R", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'R' : 'r';
    }

    bool move(int x, int y) override {
        // 룩은 가로 또는 세로로 직선 이동
        if (x == pieceX) {
            // 세로 이동
            int direction = (y > pieceY) ? (int)(XY::diffX) : -(int)(XY::diffX);
            for (int i = pieceY + direction; i != y; i += direction) {
                if (board[i][x] != ' ') return false;  // 다른 기물이 있으면 막힘
            }
        }
        else if (y == pieceY) {
            // 가로 이동
            int direction = (x > pieceX) ? (int)(XY::diffY) : -(int)(XY::diffY);
            for (int i = pieceX + direction; i != x; i += direction) {
                if (board[y][i] != ' ') return false;  // 다른 기물이 있으면 막힘
            }
        }
        else {
            return false;  // 룩은 대각선으로 이동할 수 없음
        }

        // 상대 기물이 있을 경우 잡기
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // 상대 기물 제거
        }

        // 룩 이동
        board[pieceY][pieceX] = ' ';
        pieceX = x; pieceY = y;
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'R' : 'r';

        return true;
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c, int x, int y) : Piece("B", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'B' : 'b';
    }

    bool move(int x, int y) override {
        // 비숍은 대각선으로 이동, x와 y의 차이가 같아야만 유효한 이동
        if (abs(x - pieceX) == abs(y - pieceY)) return false;  // 대각선이 아니면 이동 불가

        int xDirection = (x > pieceX) ? (int)(XY::diffX) : -(int)(XY::diffX);
        int yDirection = (y > pieceY) ? (int)(XY::diffY) : -(int)(XY::diffY);

        int i = pieceX + xDirection;
        int j = pieceY + yDirection;
        while (i != x && j != y) {
            if (board[j][i] != ' ') return false;  // 다른 기물이 있으면 막힘
            i += xDirection;
            j += yDirection;
        }

        // 상대 기물이 있을 경우 잡기
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            board[y][x] = ' ';  // 상대 기물 제거
        }

        // 비숍 이동
        board[pieceY][pieceX] = ' ';
        pieceX = x; pieceY = y;
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'B' : 'b';

        return true;
    }
};

class Knight : public Piece {
public:
    Knight(Color c, int x, int y) : Piece("N", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'N' : 'n';
    }

    bool move(int x, int y) override {
        // 나이트는 "L"자 형태로 이동: 한 방향으로 2칸, 다른 방향으로 1칸
        if ((abs(x - pieceX) == 2 * (int)(XY::diffX) && abs(y - pieceY) == 1 * (int)(XY::diffY)) || (abs(x - pieceX) == 1 * (int)(XY::diffX) && abs(y - pieceY) == 2 * (int)(XY::diffY))) {
            // 상대 기물이 있을 경우 잡기
            if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
                if (board[y][x] == 'K' || board[y][x] == 'k')
                {
                    checkmate = true;
                }
                board[y][x] = ' ';  // 상대 기물 제거
            }

            // 나이트 이동
            board[pieceY][pieceX] = ' ';
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'N' : 'n';
            return true;
        }

        return false;  // 나이트의 잘못된 이동
    }
};

class Queen : public Piece {
public:
    Queen(Color c, int x, int y) : Piece("Q", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'Q' : 'q';
    }

    bool move(int x, int y) override {
        // 퀸은 룩과 비숍의 움직임을 합친 형태로 이동
        if (x == pieceX || y == pieceY) {
            // 룩처럼 가로 또는 세로로 이동
            int direction = (y > pieceY) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);
            if (x == pieceX) {
                // 세로 이동
                for (int i = pieceY + direction; i <= y; i += direction) {
                    if (board[i][x] != ' ') return false;  // 다른 기물이 있으면 막힘
                }
            }
            else {
                // 가로 이동
                int direction = (x > pieceX) ? 1 : -1;
                for (int i = pieceX + direction; i != x; i += direction) {
                    if (board[y][i] != ' ') return false;  // 다른 기물이 있으면 막힘
                }
            }
        }
        else if (abs(x - pieceX) != abs(y - pieceY)) {
            // 비숍처럼 대각선으로 이동
            int xDirection = (x > pieceX) ? 1 * (int)(XY::diffY) : -1 * (int)(XY::diffY);
            int yDirection = (y > pieceY) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);

            int i = pieceX + xDirection;
            int j = pieceY + yDirection;
            while (i != x && j != y) {
                if (board[j][i] != ' ') return false;  // 다른 기물이 있으면 막힘
                i += xDirection;
                j += yDirection;
            }
        }
        else {
            return false;  // 퀸은 룩과 비숍의 규칙을 따라야 함
        }

        // 상대 기물이 있을 경우 잡기
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // 상대 기물 제거
        }

        // 퀸 이동
        board[pieceY][pieceX] = ' ';
        pieceX = x; pieceY = y;
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'Q' : 'q';

        return true;
    }
};

class King : public Piece {
public:
    King(Color c, int x, int y) : Piece("K", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'K' : 'k';
    }

    bool move(int x, int y) override {
        // 킹은 한 번에 한 칸씩 모든 방향으로 이동
        if (abs(x - pieceX) <= 1 * (int)(XY::diffY) && abs(y - pieceY) <= 1 * (int)(XY::diffX)) {
            // 상대 기물이 있을 경우 잡기
            if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
                if (board[y][x] == 'K' || board[y][x] == 'k')
                {
                    checkmate = true;
                }
                board[y][x] = ' ';  // 상대 기물 제거
            }

            // 킹 이동
            board[pieceY][pieceX] = ' ';
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'K' : 'k';

            return true;
        }

        return false;  // 킹의 잘못된 이동
    }
};






class Player {

private:

    string name;
    char color = 'q';

public:

    int piece_x, piece_y;
    int change_x, change_y;
    string pieceName;
    vector<Pawn> PPawn;
    vector<Rook> PRook;
    vector<Knight>PKnight;
    vector<Bishop>PBishop;
    King Pking = King(Color::WHITE, 0, 0);
    Queen Pqueen = Queen(Color::WHITE, 0, 0);

    Player() {}

    void Pawninit(Pawn p1, Pawn p2, Pawn p3, Pawn p4, Pawn p5, Pawn p6, Pawn p7, Pawn p8)
    {

        PPawn.push_back(p1);
        PPawn.push_back(p2);
        PPawn.push_back(p3);
        PPawn.push_back(p4);
        PPawn.push_back(p5);
        PPawn.push_back(p6);
        PPawn.push_back(p7);
        PPawn.push_back(p8);

    }
    void anoinit(Rook r1, Rook r2, Knight n1, Knight n2, Bishop b1, Bishop b2)
    {
        PRook.push_back(r1);
        PRook.push_back(r2);

        PKnight.push_back(n1);
        PKnight.push_back(n2);
        PBishop.push_back(b1);
        PBishop.push_back(b2);

    }
    void movement()//피스의 정보를 가지고 이동시키는 것
    {
        cout << pieceName << "(" << piece_y << piece_x << ") 이동합니다" << endl;


    }
    string getName()
    {
        return name;
    }
    void setName()
    {
        cout << "플레이어의 이름을 입력해주세요: ";
        string playerName;
        cin >> playerName;

        this->name = playerName;
    }

    void setColor(char _color)
    {
        color = _color;
    }
    char getColor()
    {
        return color;
    }


};

char choiceColor(Player& p)
{
    char color, anotherColor;
    cout << p.getName() << "플레이어님 색을 정해 주세요(W/B): ";
    cin >> color;
    p.setColor(color);
    color == 'w' ? anotherColor = 'b' : anotherColor = 'w';
    return anotherColor;
}
bool order(Player p1)//선공 결정
{
    if (p1.getColor() == 'w')
        return true;
    else if (p1.getColor() == 'b')
        return false;
}


class Board {
private:
    XY putPiece;
public:
    char moveableXY[MAP_SIZE_X][MAP_SIZE_Y];


    // 보드 세팅
    void settingBoard() {
        for (int i = 0; i < MAP_SIZE_X; i++) {
            for (int j = 0; j < MAP_SIZE_Y; j++) {

                moveableXY[i][j] = ' ';

                // 내부 격자 표시
                if (i == 0 && j == 0) {
                    board[i][j] = '+';
                }
                else if (i == 0 && j == (MAP_SIZE_Y - 1)) {
                    board[i][j] = '+';
                }
                else if (i == (MAP_SIZE_X - 1) && j == 0) {
                    board[i][j] = '+';
                }
                else if (i == (MAP_SIZE_X - 1) && j == (MAP_SIZE_Y - 1)) {
                    board[i][j] = '+';
                }
                // 상, 하, 좌, 우 테두리 설정
                else if ((i == 0 || i == (MAP_SIZE_X - 1))) {
                    board[i][j] = '-';
                }
                else if (j == 0 || j == (MAP_SIZE_Y - 1)) {
                    board[i][j] = '|';
                }

                // 내부 8x8 체스판 모양 설정
                else if ((i % 4) == 0) {
                    board[i][j] = '-';
                }
                else if ((j % 8) == 0) {
                    board[i][j] = '|';
                }

                // 빈 칸 설정
                else
                    board[i][j] = ' ';
            }
        }
    }


    // 기물 놓을 수 있는 위치
    void moveablePiece(Player& p1, Player& p2)
    {
        for (int i = 0; i < static_cast<int>(XY::term); i++) {
            for (int j = 0; j < static_cast<int>(XY::term); j++) {
                int x = static_cast<int>(XY::startX) + (i * static_cast<int>(XY::diffX));    // startX 2 dift 4
                int y = static_cast<int>(XY::startY) + (j * static_cast<int>(XY::diffY));    // starty 4 dift 8

                // 말 놓을 때 범위 안 벗어 나기
                if (x >= 0 && x < MAP_SIZE_X && y >= 0 && y < MAP_SIZE_Y)
                {
                    board[x][y] = ' ';

                }

            }
        }

        Pawn whitePawn1(Color::WHITE, 4, 6);
        Pawn whitePawn2(Color::WHITE, 12, 6);
        Pawn whitePawn3(Color::WHITE, 20, 6);
        Pawn whitePawn4(Color::WHITE, 28, 6);
        Pawn whitePawn5(Color::WHITE, 36, 6);
        Pawn whitePawn6(Color::WHITE, 44, 6);
        Pawn whitePawn7(Color::WHITE, 52, 6);
        Pawn whitePawn8(Color::WHITE, 60, 6);

        // 흰색 기타 기물 초기화
        Rook whiteRook1(Color::WHITE, 4, 2);
        Knight whiteKinght1(Color::WHITE, 12, 2);
        Bishop whiteBishop1(Color::WHITE, 20, 2);
        Queen whiteQueen(Color::WHITE, 28, 2);
        King whiteKing(Color::WHITE, 36, 2);
        Bishop whiteBishop2(Color::WHITE, 44, 2);
        Knight whiteKinght2(Color::WHITE, 52, 2);
        Rook whiteRook2(Color::WHITE, 60, 2);

        // 검은색 폰 초기화
        Pawn blackPawn1(Color::BLACK, 4, 26);
        Pawn blackPawn2(Color::BLACK, 12, 26);
        Pawn blackPawn3(Color::BLACK, 20, 26);
        Pawn blackPawn4(Color::BLACK, 28, 26);
        Pawn blackPawn5(Color::BLACK, 36, 26);
        Pawn blackPawn6(Color::BLACK, 44, 26);
        Pawn blackPawn7(Color::BLACK, 52, 26);
        Pawn blackPawn8(Color::BLACK, 60, 26);

        // 검은색 기타 기물 초기화
        Rook blackRook1(Color::BLACK, 4, 30);
        Knight blackKinght1(Color::BLACK, 12, 30);
        Bishop blackBishop1(Color::BLACK, 20, 30);
        Queen blackQueen(Color::BLACK, 28, 30);
        King blacking(Color::BLACK, 36, 30);
        Bishop blackBishop2(Color::BLACK, 44, 30);
        Knight blackKinght2(Color::BLACK, 52, 30);
        Rook blackRook2(Color::BLACK, 60, 30);


        p2.setColor(choiceColor(p1));       //플레이어 색상 고르기
        if (p1.getColor() == 'W' || p1.getColor() == 'w')
        {
            p1.Pawninit(whitePawn1, whitePawn2, whitePawn3, whitePawn4, whitePawn5, whitePawn6, whitePawn7, whitePawn8);
            p1.anoinit(whiteRook1, whiteRook2, whiteKinght1, whiteKinght2, whiteBishop1, whiteBishop2);
            p1.Pking = whiteKing; p1.Pqueen = whiteQueen;
            p2.Pawninit(blackPawn1, blackPawn2, blackPawn3, blackPawn4, blackPawn5, blackPawn6, blackPawn7, blackPawn8);
            p2.anoinit(blackRook1, blackRook2, blackKinght1, blackKinght2, blackBishop1, blackBishop2);
            p2.Pking = blacking; p2.Pqueen = blackQueen;
        }
        else
        {
            p2.Pawninit(whitePawn1, whitePawn2, whitePawn3, whitePawn4, whitePawn5, whitePawn6, whitePawn7, whitePawn8);
            p2.anoinit(whiteRook1, whiteRook2, whiteKinght1, whiteKinght2, whiteBishop1, whiteBishop2);
            p2.Pking = whiteKing; p2.Pqueen = whiteQueen;
            p1.Pawninit(blackPawn1, blackPawn2, blackPawn3, blackPawn4, blackPawn5, blackPawn6, blackPawn7, blackPawn8);
            p1.anoinit(blackRook1, blackRook2, blackKinght1, blackKinght2, blackBishop1, blackBishop2);
            p1.Pking = blacking; p1.Pqueen = blackQueen;
        }

    }



    // 보드 보여주기
    void showBoard()
    {
        system("cls");
        for (int i = 0; i < MAP_SIZE_X; i++)
        {
            for (int j = 0; j < MAP_SIZE_Y; j++)
            {
                cout << board[i][j];
            }
            cout << endl;
        }
    }

};

void choice(Player& p)//어떤 피스를 선택 
{
    int count = 0;
    cout << "입력: ";
    cin >> p.pieceName;

    cout << "좌표를 입력하세요: ";
    cin >> p.piece_x >> p.piece_y;
    cout << endl;
    cout << "변경하실 좌표를 입력하세요";
    cin >> p.change_x >> p.change_y;


    if (p.pieceName == "pawn")
    {
        for (Pawn i : p.PPawn)
        {
            if (i.getX() == p.piece_x && i.getY() == p.piece_y)
            {
                if (i.move(p.change_x, p.change_y)) {

                    p.PPawn[count].setPosition(p.change_x, p.change_y);
                    break;
                }
                else
                {
                    cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                    choice(p);
                }
            }
            count++;
        }



    }
    else if (p.pieceName == "rook")
    {
        for (Rook i : p.PRook) {
            if (i.getX() == p.piece_x && i.getY() == p.piece_y)
            {
                if (i.move(p.change_x, p.change_y)) {

                    p.PRook[count].setPosition(p.change_x, p.change_y);
                    break;
                }
                else
                {
                    cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                    choice(p);
                }

            }
            count++;
        }
    }
    else if (p.pieceName == "bishop")
    {
        for (Bishop i : p.PBishop) {
            if (i.getX() == p.piece_x && i.getY() == p.piece_y)
            {
                if (i.move(p.change_x, p.change_y)) {

                    p.PBishop[count].setPosition(p.change_x, p.change_y);
                    break;
                }
                else
                {
                    cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                    choice(p);
                }

            }
            count++;
        }
    }
    else if (p.pieceName == "knight")
    {

        for (Knight i : p.PKnight) {
            if (i.getX() == p.piece_x && i.getY() == p.piece_y)
            {
                if (i.move(p.change_x, p.change_y)) {


                    p.PKnight[count].setPosition(p.change_x, p.change_y);
                    break;
                }
                else
                {
                    cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                    choice(p);
                }

            }

            count++;
        }

    }
    else if (p.pieceName == "king")
    {
        if (p.Pking.getX() == p.piece_x && p.Pking.getY() == p.piece_y) {

            if (p.Pking.move(p.change_x, p.change_y))
            {

                p.Pking.setPosition(p.change_x, p.change_y);
            }
            else
            {
                cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                choice(p);
            }

        }


    }
    else if (p.pieceName == "queen")
    {

        if (p.Pqueen.getX() == p.piece_x && p.Pqueen.getY() == p.piece_y) {
            if (p.Pqueen.move(p.change_x, p.change_y)) {

                p.Pqueen.setPosition(p.change_x, p.change_y);

            }
            else
            {
                cout << "움직일 수 없습니다. 다시 입력해 주세요" << endl;
                choice(p);
            }
        }

    }
    else
    {
        cout << "피스와 좌표를 잘못입력하셨습니다 다시 입력해주세요." << endl;
        choice(p);

    }

}

void gamePlay(Player& p1, Player& p2, Board board)
{
    while (1)
    {
        cout << p1.getName() << "순서 입니다. 피스를 입력한 후 이동할 위치를 입력해 주세요" << endl;
        choice(p1);
        board.showBoard();
        if (checkmate)
        {
            cout << p1.getName() << "의 승리입니다." << endl;
            break;
        }

        cout << p2.getName() << "순서 입니다. 피스를 입력한 후 이동할 위치를 입력해 주세요" << endl;
        choice(p2);
        board.showBoard();
        if (checkmate)
        {
            cout << p2.getName() << "의 승리입니다." << endl;
            break;
        }
    }


}









int main()
{
    Board board;            //보드 객체 생성
    Player p1, p2;          //p1,p2 객체 생성

    p1.setName();           //플레이어1 이름
    p2.setName();           //플레이어2 이름
    cout << p1.getName() << "과" << p2.getName() << "의 경기가 시작 됩니다." << endl;

    board.settingBoard();   // 보드 판 초기화 
    board.moveablePiece(p1, p2);  // 기물 놓을 수 있는 위치 그리기
    board.showBoard();      // 보드 판 그리기


    if (order(p1))// p1이 선공
    {
        gamePlay(p1, p2, board);
    }
    else//p2가 선공 
    {
        gamePlay(p2, p1, board);
    }

    ////결과 

    return 0;
}

