#include"..\Core\SanContainer.h"
#include"cTicTacToeGame.h"
/*Feature Detect Functions*/
sfloat OrigionalFeatureFunc(const POSITION2D &Pos, const SHANDLE Param)
{
	return 1.0;
}
sfloat ConnectionDetectorCol(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat MaxConnection = 0.0f;
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	for (uint32 seek_x = 0; seek_x < pGame->iGetBoardSize(); seek_x = seek_x + 1)
	{
		ConnectionCount = 0.0;
		if (seek_x == Pos.x)
		{
			ConnectionCount = -2.0;
		}
		for (uint32 seek_y = 0; seek_y < pGame->iGetBoardSize(); seek_y = seek_y + 1)
		{
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == CurrentPlayerID)
			{
				ConnectionCount = ConnectionCount + 1.0;
			}
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == OpponentPlayerID)
			{
				ConnectionCount = ConnectionCount - 2.0;
			}
		}
		if (MaxConnection < ConnectionCount)
		{
			MaxConnection = ConnectionCount;
		}
	}
	MaxConnection = MaxConnection <= 0.0 ? 0.0 : MaxConnection;
	return (MaxConnection*MaxConnection) / 9.0;
}
sfloat ConnectionDetectorRow(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat MaxConnection = 0.0f;
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	for (uint32 seek_y = 0; seek_y < pGame->iGetBoardSize(); seek_y = seek_y + 1)
	{
		ConnectionCount = 0.0;
		if (seek_y == Pos.y)
		{
			ConnectionCount = -2.0;
		}
		for (uint32 seek_x = 0; seek_x < pGame->iGetBoardSize(); seek_x = seek_x + 1)
		{
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == CurrentPlayerID)
			{
				ConnectionCount = ConnectionCount + 1.0;
			}
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == OpponentPlayerID)
			{
				ConnectionCount = ConnectionCount - 2.0;
			}
		}
		if (MaxConnection < ConnectionCount)
		{
			MaxConnection = ConnectionCount;
		}
	}
	MaxConnection = MaxConnection <= 0.0 ? 0.0 : MaxConnection;
	return (MaxConnection*MaxConnection) / 9.0;
}
sfloat ConnectionDetectorDiag1(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	if (Pos.x != Pos.y)
	{
		ConnectionCount = 0.0;
	}
	for (uint32 seek = 0; seek < 3; seek = seek + 1)
	{
		if (pGame->iGetBoardState(POSITION2D(seek, seek)) == CurrentPlayerID)
		{
			ConnectionCount = ConnectionCount + 1.0;
		}
		if (pGame->iGetBoardState(POSITION2D(seek, seek)) == OpponentPlayerID)
		{
			ConnectionCount = ConnectionCount - 2.0;
		}
	}
	ConnectionCount = ConnectionCount > 0.0 ? ConnectionCount : 0.0;
	return (ConnectionCount*ConnectionCount) / 9.0;
}
sfloat ConnectionDetectorDiag2(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	if ((pGame->iGetBoardSize() - 1 - Pos.x) != Pos.y)
	{
		ConnectionCount = 0.0;
	}
	for (uint32 seek = 0; seek < 3; seek = seek + 1)
	{
		if (pGame->iGetBoardState(POSITION2D(seek, pGame->iGetBoardSize() - 1 - seek)) == CurrentPlayerID)
		{
			ConnectionCount = ConnectionCount + 1.0;
		}
		if (pGame->iGetBoardState(POSITION2D(seek, pGame->iGetBoardSize() - 1 - seek)) == OpponentPlayerID)
		{
			ConnectionCount = ConnectionCount - 2.0;
		}
	}
	ConnectionCount = ConnectionCount > 0.0 ? ConnectionCount : 0.0;
	return ConnectionDetectorDiag1(Pos, Param) + (ConnectionCount*ConnectionCount) / 9.0;
}
sfloat OpponentConnectionDetectorCol(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat MaxConnection = 0.0f;
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	for (uint32 seek_x = 0; seek_x < pGame->iGetBoardSize(); seek_x = seek_x + 1)
	{
		ConnectionCount = 0.0;
		if (seek_x == Pos.x)
		{
			ConnectionCount = -2.0;
		}
		for (uint32 seek_y = 0; seek_y < pGame->iGetBoardSize(); seek_y = seek_y + 1)
		{
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == CurrentPlayerID)
			{
				ConnectionCount = ConnectionCount - 2.0;
			}
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == OpponentPlayerID)
			{
				ConnectionCount = ConnectionCount + 1.0;
			}
		}
		if (MaxConnection < ConnectionCount)
		{
			MaxConnection = ConnectionCount;
		}
	}
	MaxConnection = MaxConnection <= 0.0 ? 0.0 : MaxConnection;
	return (MaxConnection*MaxConnection) / 9.0;
}
sfloat OpponentConnectionDetectorRow(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat MaxConnection = 0.0f;
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	for (uint32 seek_y = 0; seek_y < pGame->iGetBoardSize(); seek_y = seek_y + 1)
	{
		ConnectionCount = 0.0;
		if (seek_y == Pos.y)
		{
			ConnectionCount = -2.0;
		}
		for (uint32 seek_x = 0; seek_x < pGame->iGetBoardSize(); seek_x = seek_x + 1)
		{
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == CurrentPlayerID)
			{
				ConnectionCount = ConnectionCount - 2.0;
			}
			if (pGame->iGetBoardState(POSITION2D(seek_x, seek_y)) == OpponentPlayerID)
			{
				ConnectionCount = ConnectionCount + 1.0;
			}
		}
		if (MaxConnection < ConnectionCount)
		{
			MaxConnection = ConnectionCount;
		}
	}
	MaxConnection = MaxConnection <= 0.0 ? 0.0 : MaxConnection;
	return (MaxConnection*MaxConnection) / 9.0;
}
sfloat OpponentConnectionDetectorDiag1(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	if (Pos.x != Pos.y)
	{
		ConnectionCount = 0.0;
	}
	for (uint32 seek = 0; seek < 3; seek = seek + 1)
	{
		if (pGame->iGetBoardState(POSITION2D(seek, seek)) == CurrentPlayerID)
		{
			ConnectionCount = ConnectionCount - 2.0;
		}
		if (pGame->iGetBoardState(POSITION2D(seek, seek)) == OpponentPlayerID)
		{
			ConnectionCount = ConnectionCount + 1.0;
		}
	}
	ConnectionCount = ConnectionCount > 0.0 ? ConnectionCount : 0.0;
	return (ConnectionCount*ConnectionCount) / 9.0;
}
sfloat OpponentConnectionDetectorDiag2(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if ((Pos.x >= pGame->iGetBoardSize()) || (Pos.y >= pGame->iGetBoardSize()))
	{
		return 0.0;
	}
	sfloat ConnectionCount = -2.0;
	uint32 CurrentPlayerID = pGame->iGetCurrentPlayerID();
	uint32 OpponentPlayerID = CurrentPlayerID % 2 + 1;
	if ((pGame->iGetBoardSize() - 1 - Pos.x) != Pos.y)
	{
		ConnectionCount = 0.0;
	}
	for (uint32 seek = 0; seek < 3; seek = seek + 1)
	{
		if (pGame->iGetBoardState(POSITION2D(seek, pGame->iGetBoardSize() - 1 - seek)) == CurrentPlayerID)
		{
			ConnectionCount = ConnectionCount - 2.0;
		}
		if (pGame->iGetBoardState(POSITION2D(seek, pGame->iGetBoardSize() - 1 - seek)) == OpponentPlayerID)
		{
			ConnectionCount = ConnectionCount + 1.0;
		}
	}
	ConnectionCount = ConnectionCount > 0.0 ? ConnectionCount : 0.0;
	return OpponentConnectionDetectorDiag1(Pos,Param) + (ConnectionCount*ConnectionCount) / 9.0;
}
sfloat CornerPosDetector(const POSITION2D &Pos, const SHANDLE Param)
{
	cTicTacToeGame* pGame = (cTicTacToeGame*) Param;
	if (pGame == nullptr)
	{
		::cout << "Error: The Game Object Pointer is Empty\n";
		::system("pause");
		::exit(0);
	}
	if (pGame->iGetBoardSize() % 2 == 0)
	{
		return 0.0;
	}
	uint32 count = 0;
	if ((Pos.x == 0) && (Pos.y == 0))
	{
		count = 1;
	}
	if ((Pos.x == 0) && (Pos.y == 2))
	{
		count = 1;
	}
	if ((Pos.x == 2) && (Pos.y == 0))
	{
		count = 1;
	}
	if ((Pos.x == 2) && (Pos.y == 2))
	{
		count = 1;
	}
	if (count == 0)
	{
		return 0;
	}
	if (pGame->iGetBoardState(POSITION2D(0, 0)) == pGame->iGetCurrentPlayerID())
	{
		count = count + 1;
	}
	if (pGame->iGetBoardState(POSITION2D(0, 2)) == pGame->iGetCurrentPlayerID())
	{
		count = count + 1;
	}
	if (pGame->iGetBoardState(POSITION2D(2, 0)) == pGame->iGetCurrentPlayerID())
	{
		count = count + 1;
	}
	if (pGame->iGetBoardState(POSITION2D(2, 2)) == pGame->iGetCurrentPlayerID())
	{
		count = count + 1;
	}
	return count / 9.0;
}