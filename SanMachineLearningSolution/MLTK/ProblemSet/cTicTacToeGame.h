#include"..\Core\SanContainer.h"
#include"..\Device\cSanTerminalDeviceWin.h"
using namespace std;
using namespace San;
using namespace San::Device;
#ifndef __CTICTACTOEGAME_H__
#define __CTICTACTOEGAME_H__

/*Position Data Define*/
struct stPOSITION2D
{
public:
	uint32 x;
	uint32 y;
public:
	stPOSITION2D()
		:x(0),y(0)
	{};
	stPOSITION2D(const uint32 x, const uint32 y)
		:x(x), y(y)
	{};
	stPOSITION2D(const stPOSITION2D& Pos)
		:x(Pos.x), y(Pos.y)
	{};
	~stPOSITION2D(){};
	bool operator==(const stPOSITION2D& Pos){ return (this->x == Pos.x) && (this->y == Pos.y); };
};
typedef stPOSITION2D	POSITION2D;
typedef stPOSITION2D*	lpPOSITION2D;

/*Move Function Pointer Define*/
typedef POSITION2D(*PLAYERMOVEFUNC)(SHANDLE, SHANDLE, cSanTerminalDevice*);

/*Game Result Type*/
enum eCHESSRESULT
{
	SR_INPUT_ACCEPT		= 0x0100, // Data Accept, no win, no lose, no draw
	SR_INPUT_INVAILD	= 0x0200, // Data Invaild, chess state didn't change
	SR_WIN				= 0x0300, // User Win, Format: 0 - 7 bit User ID, 8 - 15 bit 0x03 SR_WIN
	SR_LOSE				= 0x0400, // Not Use
	SR_DRAW				= 0x0500, // Game Draw, just return SR_DRAW
	//SR_DEFAULT			= 0x0500,
};

/*Player Define, include the move function pointer, param is the user define value when call the move function pass to it*/
/*SHANDLE : void* */
struct stPLAYER
{
	uint32			PlayerID;
	PLAYERMOVEFUNC	pMoveFunc;
	SHANDLE			Param;
};
typedef stPLAYER	PLAYER;
typedef stPLAYER*	lpPLAYER;

/*Game Class, it didn't contain any AI part, just simple game board state class*/
class cTicTacToeGame
{
private:
	uint32**		m_pBoardBuffer; // The real board 2 - D data pointer
	uint32			m_BoardSize;
	vector<PLAYER>	m_PlayerArray; // Player Data Set, player offset in this structure is ID - 1, start at 1, zero stand for NULL
	uint32			m_CurrentPlayer;
	POSITION2D		m_LastMovePosition;
protected:
	/*Check the current board state, and return the modifyed value of eCHESSRESULT (only modifyed in winner situation)*/
	uint32 _Check(const uint32 PlayerID,const POSITION2D &Pos);
public:
	cTicTacToeGame(const uint32 BoardSize);
	~cTicTacToeGame();
	uint32	iCreatePlayer(PLAYERMOVEFUNC pFunc,const SHANDLE Param);
	void	iReleasePlayer(const uint32 PlayerID);
	void	iClearPlayer();
	/*Set the user define value which pass to the move function*/
	void	iSetPlayerParamValue(const uint32 PlayerID, const SHANDLE Param);
	/*Set Chess,  and make the state move to next user, return the _Check value, if data is invaild, return SR_INPUT_INVAILD*/
	int32	iSetChess(const uint32 PlayerID, const POSITION2D &Position);
	SHANDLE	iGetPlayerParamValue(const uint32 PlayerID) const;
	uint32	iGetCurrentPlayerID() const;
	uint32	iGetBoardState(const POSITION2D &Pos) const;
	uint32	iGetBoardSize() const;
	bool	iGetLastMovePosition(POSITION2D &Position) const;
	void	iResetBoardSize(const uint32 BoardSize);
	/*Call each player to move, one update call one user*/
	/*Set chess by call iSetChess function*/
	int32	iUpdateBoard(cSanTerminalDevice* pTerminal);
	void	iRenderBoard(cSanTerminalDevice* pTerminal) const;
	void	iClearBoard();
};
#endif