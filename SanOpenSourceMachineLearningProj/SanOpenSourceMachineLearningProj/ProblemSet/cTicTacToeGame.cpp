#include"cTicTacToeGame.h"
using namespace San;
cTicTacToeGame::cTicTacToeGame(const uint32 BoardSize)
	:m_pBoardBuffer(nullptr),
	m_BoardSize(BoardSize),
	m_CurrentPlayer(1),
	m_LastMovePosition(BoardSize,BoardSize)
{
	/*Set board size, allocate board memory*/
	this->m_BoardSize = this->m_BoardSize <= 1 ? 3 : this->m_BoardSize;
	this->m_pBoardBuffer = new uint32*[this->m_BoardSize];
	for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
	{
		this->m_pBoardBuffer[seek] = new uint32[this->m_BoardSize];
	}
	this->iClearBoard();
}
cTicTacToeGame::~cTicTacToeGame()
{
	if (this->m_pBoardBuffer != nullptr)
	{
		for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
		{
			if (this->m_pBoardBuffer[seek] != nullptr)
			{
				delete[] this->m_pBoardBuffer[seek];
				this->m_pBoardBuffer[seek] = nullptr;
			}
		}
		delete[] this->m_pBoardBuffer;
		this->m_pBoardBuffer = nullptr;
	}
}
uint32 cTicTacToeGame::_Check(const uint32 PlayerID,const POSITION2D &Pos)
{
	/*Check order: Column, Row, Diagonal, did the available board position is empty, if so, return SR_DRAW*/
	uint32 count = 0;
	for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
	{
		if (this->m_pBoardBuffer[seek][Pos.y] == PlayerID)
		{
			count = count + 1;
		}
	}
	if (count == this->m_BoardSize)
	{
		return eCHESSRESULT::SR_WIN | (PlayerID & 0xff);
	}
	count = 0;
	for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
	{
		if (this->m_pBoardBuffer[Pos.x][seek] == PlayerID)
		{
			count = count + 1;
		}
	}
	if (count == this->m_BoardSize)
	{
		return eCHESSRESULT::SR_WIN | (PlayerID & 0xff);
	}
	if ((Pos.x == Pos.y) || ((this->m_BoardSize - 1 - Pos.x) == Pos.y))
	{
		count = 0;
		for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
		{
			if (this->m_pBoardBuffer[seek][seek] == PlayerID)
			{
				count = count + 1;
			}
		}
		if (count == this->m_BoardSize)
		{
			return eCHESSRESULT::SR_WIN | (PlayerID & 0xff);
		}
		count = 0;
		for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
		{
			if (this->m_pBoardBuffer[seek][this->m_BoardSize - 1 - seek] == PlayerID)
			{
				count = count + 1;
			}
		}
		if (count == this->m_BoardSize)
		{
			return eCHESSRESULT::SR_WIN | (PlayerID & 0xff);
		}
	}
	for (uint32 seek_x = 0; seek_x < this->m_BoardSize; seek_x = seek_x + 1)
	{
		for (uint32 seek_y = 0; seek_y < this->m_BoardSize; seek_y = seek_y + 1)
		{
			if (this->m_pBoardBuffer[seek_x][seek_y] == 0)
			{
				return 0;
			}
		}
	}
	return eCHESSRESULT::SR_DRAW;
}
uint32 cTicTacToeGame::iCreatePlayer(PLAYERMOVEFUNC pFunc,const SHANDLE Param)
{
	if (pFunc == nullptr)
	{
		return 0;
	}
	if (this->m_PlayerArray.size() >= 2)
	{
		return 0;
	}
	PLAYER Player;
	Player.PlayerID = this->m_PlayerArray.size() + 1;
	Player.pMoveFunc = pFunc;
	Player.Param = Param;
	this->m_PlayerArray.push_back(Player);
	this->iClearBoard();
	this->m_CurrentPlayer = 1;
	this->m_LastMovePosition = POSITION2D(this->m_BoardSize, this->m_BoardSize);
	return Player.PlayerID;
}
void cTicTacToeGame::iReleasePlayer(const uint32 PlayerID)
{
	if (PlayerID == 0)
	{
		return;
	}
	vector<PLAYER>::iterator pPlayer = this->m_PlayerArray.begin();
	while (pPlayer!=this->m_PlayerArray.end())
	{
		if (pPlayer->PlayerID == PlayerID)
		{
			pPlayer = this->m_PlayerArray.erase(pPlayer);
			return;
		}
		pPlayer++;
	}
	this->iClearBoard();
	this->m_CurrentPlayer = 1;
	this->m_LastMovePosition = POSITION2D(this->m_BoardSize, this->m_BoardSize);
}
void cTicTacToeGame::iClearPlayer()
{
	this->m_PlayerArray.clear();
	this->iClearBoard();
	this->m_CurrentPlayer = 1;
	this->m_LastMovePosition = POSITION2D(this->m_BoardSize, this->m_BoardSize);
}
void cTicTacToeGame::iSetPlayerParamValue(const uint32 PlayerID, const SHANDLE Param)
{
	if (PlayerID == 0)
	{
		return;
	}
	vector<PLAYER>::iterator pPlayer = this->m_PlayerArray.begin();
	while (pPlayer != this->m_PlayerArray.end())
	{
		if (pPlayer->PlayerID == PlayerID)
		{
			pPlayer->Param = Param;
			return;
		}
		pPlayer++;
	}
}
int32 cTicTacToeGame::iSetChess(const uint32 PlayerID, const POSITION2D &Position)
{
	/*Check Data Value*/
	if ((PlayerID == 0) || (PlayerID > this->m_PlayerArray.size()))
	{
		return eCHESSRESULT::SR_INPUT_INVAILD;
	}
	if (PlayerID != this->m_CurrentPlayer)
	{
		return eCHESSRESULT::SR_INPUT_INVAILD;
	}
	if (Position.x >= this->m_BoardSize)
	{
		return eCHESSRESULT::SR_INPUT_INVAILD;
	}
	if (Position.y >= this->m_BoardSize)
	{
		return eCHESSRESULT::SR_INPUT_INVAILD;
	}
	if (this->m_pBoardBuffer[Position.x][Position.y] != 0)
	{
		return eCHESSRESULT::SR_INPUT_INVAILD;
	}
	/*Set Chess*/
	this->m_pBoardBuffer[Position.x][Position.y] = PlayerID;
	this->m_LastMovePosition = Position;
	/*Move to next player*/
	this->m_CurrentPlayer = this->m_CurrentPlayer % this->m_PlayerArray.size() + 1;
	/*Winner Check*/
	uint32 Res = this->_Check(PlayerID, Position);
	if (Res != 0)
	{
		return Res;
	}
	return eCHESSRESULT::SR_INPUT_ACCEPT;
}
SHANDLE cTicTacToeGame::iGetPlayerParamValue(const uint32 PlayerID) const
{
	if (PlayerID == 0)
	{
		return nullptr;
	}
	vector<PLAYER>::const_iterator pPlayer = this->m_PlayerArray.begin();
	while (pPlayer != this->m_PlayerArray.end())
	{
		if (pPlayer->PlayerID == PlayerID)
		{
			return pPlayer->Param;
		}
		pPlayer++;
	}
}
uint32 cTicTacToeGame::iGetCurrentPlayerID() const
{
	return this->m_CurrentPlayer;
}
uint32 cTicTacToeGame::iGetBoardState(const POSITION2D &Pos) const
{
	if ((Pos.x >= this->m_BoardSize) || (Pos.y >= this->m_BoardSize))
	{
		return 0;
	}
	return this->m_pBoardBuffer[Pos.x][Pos.y];
}
uint32 cTicTacToeGame::iGetBoardSize() const
{
	return this->m_BoardSize;
}
bool cTicTacToeGame::iGetLastMovePosition(POSITION2D &Position) const
{
	if (this->m_LastMovePosition.x >= this->m_BoardSize)
	{
		return false;
	}
	Position = this->m_LastMovePosition;
	return true;
}
void cTicTacToeGame::iResetBoardSize(const uint32 BoardSize)
{
	if (this->m_BoardSize == BoardSize)
	{
		return;
	}
	this->m_BoardSize = BoardSize <= 1 ? 3 : BoardSize;
	if (this->m_pBoardBuffer != nullptr)
	{
		for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
		{
			if (this->m_pBoardBuffer[seek] != nullptr)
			{
				delete[] this->m_pBoardBuffer[seek];
				this->m_pBoardBuffer[seek] = nullptr;
			}
		}
		delete[] this->m_pBoardBuffer;
		this->m_pBoardBuffer = nullptr;
	}
	this->m_pBoardBuffer = new uint32*[this->m_BoardSize];
	for (uint32 seek = 0; seek < this->m_BoardSize; seek = seek + 1)
	{
		this->m_pBoardBuffer[seek] = new uint32[this->m_BoardSize];
	}
	this->iClearBoard();
	this->m_CurrentPlayer = 1;
	this->m_LastMovePosition = POSITION2D(this->m_BoardSize, this->m_BoardSize);
}
int32 cTicTacToeGame::iUpdateBoard(cSanTerminalDevice* pTerminal)
{
	int32 Res = eCHESSRESULT::SR_INPUT_INVAILD;
	if (this->m_PlayerArray[this->m_CurrentPlayer - 1].pMoveFunc != nullptr)
	{
		/*Update current user until the current user move function return a available position*/
		while (Res == eCHESSRESULT::SR_INPUT_INVAILD)
		{
			Res = this->iSetChess(this->m_CurrentPlayer, this->m_PlayerArray[this->m_CurrentPlayer - 1].pMoveFunc((SHANDLE)this, this->m_PlayerArray[this->m_CurrentPlayer - 1].Param, pTerminal));
		}
		return Res;
	}
	return eCHESSRESULT::SR_INPUT_INVAILD;
}
void cTicTacToeGame::iRenderBoard(cSanTerminalDevice* pTerminal) const
{
	schar ChessSymbol = _SSTR(' ');
	schar RowSymbolArray [] = _SSTR("012");
	pTerminal->iOutputString(_SSTR("    0   1   2\n"));
	pTerminal->iOutputString(_SSTR("  ¨X¨T¨j¨T¨j¨T¨[\n"));
	for (uint32 seek_x = 0; seek_x < this->m_BoardSize; seek_x = seek_x + 1)
	{
		pTerminal->iOutputString(RowSymbolArray[seek_x] + _SSTR(" ¨U"));
		for (uint32 seek_y = 0; seek_y < this->m_BoardSize; seek_y = seek_y + 1)
		{
			switch (this->m_pBoardBuffer[seek_x][seek_y])
			{
			case 0:
				ChessSymbol = _SSTR(' ');
				break;
			case 1:
				ChessSymbol = _SSTR('O');
				break;
			case 2:
				ChessSymbol = _SSTR('X');
				break;
			default:
				ChessSymbol = _SSTR(' ');
				break;
			}
			pTerminal->iOutputString(ChessSymbol + _SSTR(" ¨U"));
		}
		pTerminal->iOutputString(_SSTR("\n") + ((seek_x + 1) != this->m_BoardSize) ? _SSTR("  ¨d¨T¨p¨T¨p¨T¨g\n") : _SSTR("  ¨^¨T¨m¨T¨m¨T¨a\n"));
	}
}
void cTicTacToeGame::iClearBoard()
{
	for (uint32 seek_x = 0; seek_x < this->m_BoardSize; seek_x = seek_x + 1)
	{
		for (uint32 seek_y = 0; seek_y < this->m_BoardSize; seek_y = seek_y + 1)
		{
			this->m_pBoardBuffer[seek_x][seek_y] = 0;
		}
	}
	this->m_CurrentPlayer = 1;
	this->m_LastMovePosition = POSITION2D(this->m_BoardSize, this->m_BoardSize);
}