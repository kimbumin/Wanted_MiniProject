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
    // ������: ���� �ʱ� ��ġ�� ��ġ
    Pawn(Color c, int x, int y) : Piece("P", c, x, y) {
        // ���� ������ �ʱ� ��ġ�� ��ġ
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';
    }

    bool move(int x, int y) override {
        int direction = (color == Color::WHITE) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);  // ����� -1, �������� 1 �������� �̵�

        // ���� ��ġ���� ���� ����
        board[pieceY][pieceX] = ' ';

        // �Ϲ� �̵�: 1ĭ ����
        if (x == pieceX && (y == pieceY + direction || y == pieceY - direction)) {
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // ���ο� ��ġ�� �� ��ġ
            return true;
        }

        // ù ��° �̵����� 2ĭ ���� (�����θ�)
        if (/*pieceY == (color == Color::WHITE ? 6 : 1) &&*/ x == pieceX && abs(y - pieceY) == 8) {
            int middleY = pieceY + direction;  // �߰� ĭ
            // �߰� ĭ�� ��� �ִ��� Ȯ��
            if (board[middleY][pieceX] == ' ') {
                pieceX = x; pieceY = y;
                board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // ���ο� ��ġ�� �� ��ġ
                return true;
            }
        }

        // �밢������ ���� ���� ��� ���
        if (abs(x - pieceX) == 8 && (y == pieceY + direction || y == pieceY - direction) && board[y][x] != ' ') {
            // ��� ���� ���� �ִ��� Ȯ���ϰ�, ���� �� ������ ���
            /*if ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B')) {*/
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // ��� ���� ����
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'P' : 'p';  // ���ο� ��ġ�� �� ��ġ
            return true;
            //}
        }

        // �̵��� �Ұ����� ���, ���� ��ġ�� �ǵ�����
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
        // ���� ���� �Ǵ� ���η� ���� �̵�
        if (x == pieceX) {
            // ���� �̵�
            int direction = (y > pieceY) ? (int)(XY::diffX) : -(int)(XY::diffX);
            for (int i = pieceY + direction; i != y; i += direction) {
                if (board[i][x] != ' ') return false;  // �ٸ� �⹰�� ������ ����
            }
        }
        else if (y == pieceY) {
            // ���� �̵�
            int direction = (x > pieceX) ? (int)(XY::diffY) : -(int)(XY::diffY);
            for (int i = pieceX + direction; i != x; i += direction) {
                if (board[y][i] != ' ') return false;  // �ٸ� �⹰�� ������ ����
            }
        }
        else {
            return false;  // ���� �밢������ �̵��� �� ����
        }

        // ��� �⹰�� ���� ��� ���
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // ��� �⹰ ����
        }

        // �� �̵�
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
        // ����� �밢������ �̵�, x�� y�� ���̰� ���ƾ߸� ��ȿ�� �̵�
        if (abs(x - pieceX) == abs(y - pieceY)) return false;  // �밢���� �ƴϸ� �̵� �Ұ�

        int xDirection = (x > pieceX) ? (int)(XY::diffX) : -(int)(XY::diffX);
        int yDirection = (y > pieceY) ? (int)(XY::diffY) : -(int)(XY::diffY);

        int i = pieceX + xDirection;
        int j = pieceY + yDirection;
        while (i != x && j != y) {
            if (board[j][i] != ' ') return false;  // �ٸ� �⹰�� ������ ����
            i += xDirection;
            j += yDirection;
        }

        // ��� �⹰�� ���� ��� ���
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            board[y][x] = ' ';  // ��� �⹰ ����
        }

        // ��� �̵�
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
        // ����Ʈ�� "L"�� ���·� �̵�: �� �������� 2ĭ, �ٸ� �������� 1ĭ
        if ((abs(x - pieceX) == 2 * (int)(XY::diffX) && abs(y - pieceY) == 1 * (int)(XY::diffY)) || (abs(x - pieceX) == 1 * (int)(XY::diffX) && abs(y - pieceY) == 2 * (int)(XY::diffY))) {
            // ��� �⹰�� ���� ��� ���
            if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
                if (board[y][x] == 'K' || board[y][x] == 'k')
                {
                    checkmate = true;
                }
                board[y][x] = ' ';  // ��� �⹰ ����
            }

            // ����Ʈ �̵�
            board[pieceY][pieceX] = ' ';
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'N' : 'n';
            return true;
        }

        return false;  // ����Ʈ�� �߸��� �̵�
    }
};

class Queen : public Piece {
public:
    Queen(Color c, int x, int y) : Piece("Q", c, x, y) {
        board[pieceY][pieceX] = (color == Color::WHITE) ? 'Q' : 'q';
    }

    bool move(int x, int y) override {
        // ���� ��� ����� �������� ��ģ ���·� �̵�
        if (x == pieceX || y == pieceY) {
            // ��ó�� ���� �Ǵ� ���η� �̵�
            int direction = (y > pieceY) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);
            if (x == pieceX) {
                // ���� �̵�
                for (int i = pieceY + direction; i <= y; i += direction) {
                    if (board[i][x] != ' ') return false;  // �ٸ� �⹰�� ������ ����
                }
            }
            else {
                // ���� �̵�
                int direction = (x > pieceX) ? 1 : -1;
                for (int i = pieceX + direction; i != x; i += direction) {
                    if (board[y][i] != ' ') return false;  // �ٸ� �⹰�� ������ ����
                }
            }
        }
        else if (abs(x - pieceX) != abs(y - pieceY)) {
            // ���ó�� �밢������ �̵�
            int xDirection = (x > pieceX) ? 1 * (int)(XY::diffY) : -1 * (int)(XY::diffY);
            int yDirection = (y > pieceY) ? 1 * (int)(XY::diffX) : -1 * (int)(XY::diffX);

            int i = pieceX + xDirection;
            int j = pieceY + yDirection;
            while (i != x && j != y) {
                if (board[j][i] != ' ') return false;  // �ٸ� �⹰�� ������ ����
                i += xDirection;
                j += yDirection;
            }
        }
        else {
            return false;  // ���� ��� ����� ��Ģ�� ����� ��
        }

        // ��� �⹰�� ���� ��� ���
        if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
            if (board[y][x] == 'K' || board[y][x] == 'k')
            {
                checkmate = true;
            }
            board[y][x] = ' ';  // ��� �⹰ ����
        }

        // �� �̵�
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
        // ŷ�� �� ���� �� ĭ�� ��� �������� �̵�
        if (abs(x - pieceX) <= 1 * (int)(XY::diffY) && abs(y - pieceY) <= 1 * (int)(XY::diffX)) {
            // ��� �⹰�� ���� ��� ���
            if (board[y][x] != ' ' && ((color == Color::WHITE && board[y][x] == 'b') || (color == Color::BLACK && board[y][x] == 'B'))) {
                if (board[y][x] == 'K' || board[y][x] == 'k')
                {
                    checkmate = true;
                }
                board[y][x] = ' ';  // ��� �⹰ ����
            }

            // ŷ �̵�
            board[pieceY][pieceX] = ' ';
            pieceX = x; pieceY = y;
            board[pieceY][pieceX] = (color == Color::WHITE) ? 'K' : 'k';

            return true;
        }

        return false;  // ŷ�� �߸��� �̵�
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
    void movement()//�ǽ��� ������ ������ �̵���Ű�� ��
    {
        cout << pieceName << "(" << piece_y << piece_x << ") �̵��մϴ�" << endl;


    }
    string getName()
    {
        return name;
    }
    void setName()
    {
        cout << "�÷��̾��� �̸��� �Է����ּ���: ";
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
    cout << p.getName() << "�÷��̾�� ���� ���� �ּ���(W/B): ";
    cin >> color;
    p.setColor(color);
    color == 'w' ? anotherColor = 'b' : anotherColor = 'w';
    return anotherColor;
}
bool order(Player p1)//���� ����
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


    // ���� ����
    void settingBoard() {
        for (int i = 0; i < MAP_SIZE_X; i++) {
            for (int j = 0; j < MAP_SIZE_Y; j++) {

                moveableXY[i][j] = ' ';

                // ���� ���� ǥ��
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
                // ��, ��, ��, �� �׵θ� ����
                else if ((i == 0 || i == (MAP_SIZE_X - 1))) {
                    board[i][j] = '-';
                }
                else if (j == 0 || j == (MAP_SIZE_Y - 1)) {
                    board[i][j] = '|';
                }

                // ���� 8x8 ü���� ��� ����
                else if ((i % 4) == 0) {
                    board[i][j] = '-';
                }
                else if ((j % 8) == 0) {
                    board[i][j] = '|';
                }

                // �� ĭ ����
                else
                    board[i][j] = ' ';
            }
        }
    }


    // �⹰ ���� �� �ִ� ��ġ
    void moveablePiece(Player& p1, Player& p2)
    {
        for (int i = 0; i < static_cast<int>(XY::term); i++) {
            for (int j = 0; j < static_cast<int>(XY::term); j++) {
                int x = static_cast<int>(XY::startX) + (i * static_cast<int>(XY::diffX));    // startX 2 dift 4
                int y = static_cast<int>(XY::startY) + (j * static_cast<int>(XY::diffY));    // starty 4 dift 8

                // �� ���� �� ���� �� ���� ����
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

        // ��� ��Ÿ �⹰ �ʱ�ȭ
        Rook whiteRook1(Color::WHITE, 4, 2);
        Knight whiteKinght1(Color::WHITE, 12, 2);
        Bishop whiteBishop1(Color::WHITE, 20, 2);
        Queen whiteQueen(Color::WHITE, 28, 2);
        King whiteKing(Color::WHITE, 36, 2);
        Bishop whiteBishop2(Color::WHITE, 44, 2);
        Knight whiteKinght2(Color::WHITE, 52, 2);
        Rook whiteRook2(Color::WHITE, 60, 2);

        // ������ �� �ʱ�ȭ
        Pawn blackPawn1(Color::BLACK, 4, 26);
        Pawn blackPawn2(Color::BLACK, 12, 26);
        Pawn blackPawn3(Color::BLACK, 20, 26);
        Pawn blackPawn4(Color::BLACK, 28, 26);
        Pawn blackPawn5(Color::BLACK, 36, 26);
        Pawn blackPawn6(Color::BLACK, 44, 26);
        Pawn blackPawn7(Color::BLACK, 52, 26);
        Pawn blackPawn8(Color::BLACK, 60, 26);

        // ������ ��Ÿ �⹰ �ʱ�ȭ
        Rook blackRook1(Color::BLACK, 4, 30);
        Knight blackKinght1(Color::BLACK, 12, 30);
        Bishop blackBishop1(Color::BLACK, 20, 30);
        Queen blackQueen(Color::BLACK, 28, 30);
        King blacking(Color::BLACK, 36, 30);
        Bishop blackBishop2(Color::BLACK, 44, 30);
        Knight blackKinght2(Color::BLACK, 52, 30);
        Rook blackRook2(Color::BLACK, 60, 30);


        p2.setColor(choiceColor(p1));       //�÷��̾� ���� ����
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



    // ���� �����ֱ�
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

void choice(Player& p)//� �ǽ��� ���� 
{
    int count = 0;
    cout << "�Է�: ";
    cin >> p.pieceName;

    cout << "��ǥ�� �Է��ϼ���: ";
    cin >> p.piece_x >> p.piece_y;
    cout << endl;
    cout << "�����Ͻ� ��ǥ�� �Է��ϼ���";
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
                    cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
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
                    cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
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
                    cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
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
                    cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
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
                cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
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
                cout << "������ �� �����ϴ�. �ٽ� �Է��� �ּ���" << endl;
                choice(p);
            }
        }

    }
    else
    {
        cout << "�ǽ��� ��ǥ�� �߸��Է��ϼ̽��ϴ� �ٽ� �Է����ּ���." << endl;
        choice(p);

    }

}

void gamePlay(Player& p1, Player& p2, Board board)
{
    while (1)
    {
        cout << p1.getName() << "���� �Դϴ�. �ǽ��� �Է��� �� �̵��� ��ġ�� �Է��� �ּ���" << endl;
        choice(p1);
        board.showBoard();
        if (checkmate)
        {
            cout << p1.getName() << "�� �¸��Դϴ�." << endl;
            break;
        }

        cout << p2.getName() << "���� �Դϴ�. �ǽ��� �Է��� �� �̵��� ��ġ�� �Է��� �ּ���" << endl;
        choice(p2);
        board.showBoard();
        if (checkmate)
        {
            cout << p2.getName() << "�� �¸��Դϴ�." << endl;
            break;
        }
    }


}









int main()
{
    Board board;            //���� ��ü ����
    Player p1, p2;          //p1,p2 ��ü ����

    p1.setName();           //�÷��̾�1 �̸�
    p2.setName();           //�÷��̾�2 �̸�
    cout << p1.getName() << "��" << p2.getName() << "�� ��Ⱑ ���� �˴ϴ�." << endl;

    board.settingBoard();   // ���� �� �ʱ�ȭ 
    board.moveablePiece(p1, p2);  // �⹰ ���� �� �ִ� ��ġ �׸���
    board.showBoard();      // ���� �� �׸���


    if (order(p1))// p1�� ����
    {
        gamePlay(p1, p2, board);
    }
    else//p2�� ���� 
    {
        gamePlay(p2, p1, board);
    }

    ////��� 

    return 0;
}

