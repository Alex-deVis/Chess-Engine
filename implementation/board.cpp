#include "../skeleton/board.h"

Board::Board() : w_king(Position('e', WHITE_PIECES_ROW)), b_king(Position('e', BLACK_PIECES_ROW)) {
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
}

Board::Board(Board *b) : w_king(b->w_king), b_king(b->b_king) {
    board = new std::vector<std::vector<Piece*>>(8, std::vector<Piece*>(SIZE, nullptr));
    Piece *p;

    for (int r=1; r<=8; r++) {
        for (char c='a'; c<='h'; c++) {
            if (b->is_piece_at(Position(c, r))) {
                p = b->get_piece_at(Position(c, r));
                switch (p->type) {
                    case K:
                        this->add_piece(new King(p->color, p->pos));
                        break;
                    case Q:
                        this->add_piece(new Queen(p->color, p->pos));
                        break;
                    case R:
                        this->add_piece(new Rook(p->color, p->pos));
                        break;
                    case B:
                        this->add_piece(new Bishop(p->color, p->pos));
                        break;
                    case N:
                        this->add_piece(new Knight(p->color, p->pos));
                        break;
                    case P:
                        this->add_piece(new Pawn(p->color, p->pos));
                        break;
                    default:
                        std::cout << p->pos << "\n";
                        assert("Invalid piece on the board");
                }
            }
        }
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

void Board::set_piece_at(Piece *piece, Position position) {
    (*board)[SIZE - position.row][position.col - 'a'] = piece;
}

void Board::move_piece(Piece *piece, Position position) {
    if (piece) {
        set_piece_at(nullptr, piece->pos);
        piece->pos = position;
        set_piece_at(piece, piece->pos);
        if (piece->type == Type::K) {
            if (piece->color == Color::WHITE) {
                w_king = piece->pos;
            } else {
                b_king = piece->pos;
            }
        }
    } else {
        set_piece_at(piece, position);
    }
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

Transition::Transition(std::string move_string, bool p, bool c, bool e, bool f, Piece* tp = nullptr) :
        start(Position(move_string[0], atoi(move_string.substr(1, 1).c_str()))),
        end(Position(move_string[2], atoi(move_string.substr(3, 1).c_str()))) {
    this->move_string = move_string;
    promotion = p;
    castle = c;
    enPassant = e;
    forced = f;
    this->taken_piece = tp;
}
