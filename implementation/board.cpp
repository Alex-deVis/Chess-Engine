#include "../skeleton/board.h"

Board::Board() {
    // Initialise Board and maps
    board = new std::vector<std::vector<Piece*>>(8, std::vector<Piece*>(SIZE, nullptr));

    // White pieces
    add_piece(new Rook(Color::WHITE, Position('a', WHITE_PIECES_ROW)));
    add_piece(new Knight(Color::WHITE, Position('b', WHITE_PIECES_ROW)));
    add_piece(new Bishop(Color::WHITE, Position('c', WHITE_PIECES_ROW)));
    add_piece(new Queen(Color::WHITE, Position('d', WHITE_PIECES_ROW)));
    add_piece(new King(Color::WHITE, Position('e', WHITE_PIECES_ROW)));
    add_piece(new Bishop(Color::WHITE, Position('f', WHITE_PIECES_ROW)));
    add_piece(new Knight(Color::WHITE, Position('g', WHITE_PIECES_ROW)));
    add_piece(new Rook(Color::WHITE, Position('h', WHITE_PIECES_ROW)));
    for (int i = 0; i < 8; i++) {
        add_piece(new Pawn(Color::WHITE, Position('a' + i, WHITE_PAWNS_ROW)));
    }
    // Black pieces
    add_piece(new Rook(Color::BLACK, Position('a', BLACK_PIECES_ROW)));
    add_piece(new Knight(Color::BLACK, Position('b', BLACK_PIECES_ROW)));
    add_piece(new Bishop(Color::BLACK, Position('c', BLACK_PIECES_ROW)));
    add_piece(new Queen(Color::BLACK, Position('d', BLACK_PIECES_ROW)));
    add_piece(new King(Color::BLACK, Position('e', BLACK_PIECES_ROW)));
    add_piece(new Bishop(Color::BLACK, Position('f', BLACK_PIECES_ROW)));
    add_piece(new Knight(Color::BLACK, Position('g', BLACK_PIECES_ROW)));
    add_piece(new Rook(Color::BLACK, Position('h', BLACK_PIECES_ROW)));
    for (int i = 0; i < 8; i++) {
        add_piece(new Pawn(Color::BLACK, Position('a' + i, BLACK_PAWNS_ROW)));
    }

    // Piece position
    white_pieces.emplace(Type::K, std::vector<Piece*>());
    white_pieces[K].push_back(get_piece_at(Position('e', WHITE_PIECES_ROW)));
    white_pieces.emplace(Type::Q, std::vector<Piece*>());
    white_pieces[Q].push_back(get_piece_at(Position('d', WHITE_PIECES_ROW)));
    white_pieces.emplace(Type::R, std::vector<Piece*>());
    white_pieces[R].push_back(get_piece_at(Position('a', WHITE_PIECES_ROW)));
    white_pieces[R].push_back(get_piece_at(Position('h', WHITE_PIECES_ROW)));
    white_pieces.emplace(Type::B, std::vector<Piece*>());
    white_pieces[B].push_back(get_piece_at(Position('c', WHITE_PIECES_ROW)));
    white_pieces[B].push_back(get_piece_at(Position('f', WHITE_PIECES_ROW)));
    white_pieces.emplace(Type::N, std::vector<Piece*>());
    white_pieces[N].push_back(get_piece_at(Position('b', WHITE_PIECES_ROW)));
    white_pieces[N].push_back(get_piece_at(Position('g', WHITE_PIECES_ROW)));
    white_pieces.emplace(Type::P, std::vector<Piece*>());
    for (int i = 0; i < 8; i++) {
        white_pieces[P].push_back(get_piece_at(Position('a' + i, WHITE_PAWNS_ROW)));
    }

    black_pieces.emplace(Type::K, std::vector<Piece*>());
    black_pieces[K].push_back(get_piece_at(Position('e', BLACK_PIECES_ROW)));
    black_pieces.emplace(Type::Q, std::vector<Piece*>());
    black_pieces[Q].push_back(get_piece_at(Position('d', BLACK_PIECES_ROW)));
    black_pieces.emplace(Type::R, std::vector<Piece*>());
    black_pieces[R].push_back(get_piece_at(Position('a', BLACK_PIECES_ROW)));
    black_pieces[R].push_back(get_piece_at(Position('h', BLACK_PIECES_ROW)));
    black_pieces.emplace(Type::B, std::vector<Piece*>());
    black_pieces[B].push_back(get_piece_at(Position('c', BLACK_PIECES_ROW)));
    black_pieces[B].push_back(get_piece_at(Position('f', BLACK_PIECES_ROW)));
    black_pieces.emplace(Type::N, std::vector<Piece*>());
    black_pieces[N].push_back(get_piece_at(Position('b', BLACK_PIECES_ROW)));
    black_pieces[N].push_back(get_piece_at(Position('g', BLACK_PIECES_ROW)));
    black_pieces.emplace(Type::P, std::vector<Piece*>());
    for (int i = 0; i < 8; i++) {
        black_pieces[P].push_back(get_piece_at(Position('a' + i, BLACK_PAWNS_ROW)));
    }
}

Board::~Board() {
    for (auto line : (*board)) {
        for (auto p : line) {
            if (p != nullptr) {
                delete p;
            }
        }
    }
    delete board;
}

void Board::add_piece(Piece *piece) {
    assert("Can`t place a piece on an occupied square." && !is_piece_at(piece->pos));
    (*board)[SIZE - piece->pos.row][piece->pos.col - 'a'] = piece;
}

void Board::add_piece(Piece *piece, Position position) {
    assert("Can`t place a piece on an occupied square." && !is_piece_at(position));
    (*board)[SIZE - position.row][position.col - 'a'] = piece;
}

bool Board::is_piece_at(Position position) {
    return (*board)[SIZE - position.row][position.col - 'a'] != nullptr;
}

Piece* Board::get_piece_at(Position position) {
    return (*board)[SIZE - position.row][position.col - 'a'];
}

void Board::print_board() {
    for (int i = SIZE; i >= 1; i--) {
        for (char j = 'a'; j <= 'h'; j++) {
            Piece *p = get_piece_at(Position(j, i));
            if (p) {
                switch (p->type) {
                    case K:
                        std::cout << ((p->color == Color::WHITE) ? 'K' : 'k');
                        break;
                    case Q:
                        std::cout << ((p->color == Color::WHITE) ? 'Q' : 'q');
                        break;
                    case R:
                        std::cout << ((p->color == Color::WHITE) ? 'R' : 'r');
                        break;
                    case B:
                        std::cout << ((p->color == Color::WHITE) ? 'B' : 'b');
                        break;
                    case N:
                        std::cout << ((p->color == Color::WHITE) ? 'N' : 'n');
                        break;
                    case P:
                        std::cout << ((p->color == Color::WHITE) ? 'P' : 'p');
                        break;
                    default:
                        std::cout << p->pos << "\n";
                        assert("Invalid piece on the board");
                }
                std::cout << " ";
            } else {
                std::cout << "* ";
            }
        }
        std::cout << "\n";
    }
}

/*
    Capture:    LSB set
    Castle:     second LSB set
    Promotion:  third LSB set
*/
Configuration::Configuration(std::string move_string, bool capture, bool promotion, bool castle) :
        start(Position(move_string[0], atoi(move_string.substr(1, 1).c_str()))),
        end(Position(move_string[0], atoi(move_string.substr(1, 1).c_str()))) {
    this->move_string = move_string;
    value = 0;
    value |= capture ? 1 : 0;
    value |= promotion ? 2 : 0;
    value |= castle ? 4 : 0;
}
