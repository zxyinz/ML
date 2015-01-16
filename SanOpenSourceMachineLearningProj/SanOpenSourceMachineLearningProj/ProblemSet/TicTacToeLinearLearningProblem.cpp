#include"TicTacToeLinearLearningProblem.h"
using namespace std;
using namespace San;
using namespace San::FileIOStream;
using namespace San::MachineLearning;
void TicTacToePrintBoard(const cTicTacToeGame &GameObj, const uint32 LearnerID, const uint32 SuperviserID, cSanTerminalDevice* pTerminal)
{
	::system("cls");

	pTerminal->iOutputString(_SSTR("/*--------------- CSE5693 Machine Learning HW1 ---------------*/\n"));
	pTerminal->iOutputString(_SSTR("/*REQUIREMENT: TIC - TAC - TOE                                */\n"));
	pTerminal->iOutputString(_SSTR("/*------------------------------------------------------------*/\n"));

	GameObj.iRenderBoard(pTerminal);

	SString strString = SString(_SSTR("First Move : ")) + (LearnerID == 1 ? _SSTR("Learner\tChess: O\n") : _SSTR("Superviser\tChess: O\n"));

	pTerminal->iOutputString(strString);
}
POSITION2D TicTacToeCreatePlayer(cTicTacToeGame &Game, SHANDLE LearnerParam, SHANDLE SuperviserParam, const uint32 Mode)
{
	/*Mode: 0 User - User	1 User - AI		2 AI - AI*/
	uint32 PlayerLearner = 0;
	uint32 PlayerSuperviser = 0;
	::srand(time(0));
	bool bSuperviserFirst = (::rand() % 2) == 0;
	switch (Mode)
	{
	case 0:
		if (bSuperviserFirst)
		{
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
			PlayerLearner = Game.iCreatePlayer(GetUserMove, SuperviserParam);
		}
		else
		{
			PlayerLearner = Game.iCreatePlayer(GetUserMove, LearnerParam);
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
		}
		break;
	case 1:
		if (bSuperviserFirst)
		{
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
			PlayerLearner = Game.iCreatePlayer(GetAIMove, LearnerParam);
		}
		else
		{
			PlayerLearner = Game.iCreatePlayer(GetAIMove, LearnerParam);
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
		}
		break;
	case 2:
		if (bSuperviserFirst)
		{
			PlayerSuperviser = Game.iCreatePlayer(GetAIMove, SuperviserParam);
			PlayerLearner = Game.iCreatePlayer(GetAIMove, LearnerParam);
		}
		else
		{
			PlayerLearner = Game.iCreatePlayer(GetAIMove, LearnerParam);
			PlayerSuperviser = Game.iCreatePlayer(GetAIMove, SuperviserParam);
		}
		break;
	default:
		if (bSuperviserFirst)
		{
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
			PlayerLearner = Game.iCreatePlayer(GetUserMove, SuperviserParam);
		}
		else
		{
			PlayerLearner = Game.iCreatePlayer(GetUserMove, LearnerParam);
			PlayerSuperviser = Game.iCreatePlayer(GetUserMove, SuperviserParam);
		}
		break;
	}
	return POSITION2D(PlayerLearner, PlayerSuperviser);
}
/*Superviser Learning Function*/
void SupervisetLearning(const uint32 Times, const SString strFilePath, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal)
{
	if (Times == 0)
	{
		return;
	}
	SStringA Data;
	if (!::gloLoadFile(strFilePath, Data))
	{
		return;
	}
	vector<SStringA> Item = ::gloGetStringItemsA(Data, "\r\n\t");
	vector<vector<POSITION2D>> TrainSetList;
	for (uint32 seek = 0; seek < Item.size(); seek = seek + 4)
	{
		vector<POSITION2D> SubList;
		if (Item[seek + 1] == "#Learner")
		{
			SubList.push_back(POSITION2D(1, 2));
		}
		else
		{
			SubList.push_back(POSITION2D(2, 1));
		}
		vector<SStringA> RecordList = ::gloGetStringItemsA(Item[seek + 3], "|");
		vector<SStringA> Pos2D;
		for (uint32 seek_r = 0; seek_r < RecordList.size(); seek_r = seek_r + 1)
		{
			Pos2D = ::gloGetStringItemsA(RecordList[seek_r], ",");
			SubList.push_back(POSITION2D(::atoi(Pos2D[0].c_str()), ::atoi(Pos2D[1].c_str())));
		}
		TrainSetList.push_back(SubList);
	}

	uint32		TraningDataSize = Item.size() / 4;
	uint32		PlayerLearner = 0;
	uint32		PlayerSuperviser = 0;
	uint32		Res = 0;
	uint32		CurrentPlayer = 0;
	POSITION2D	Pos;

	/*Game Loop*/
	for (uint32 seek = 0; seek < Times; seek = seek + 1)
	{
		for (uint32 seek_tr = 0; seek_tr < TraningDataSize; seek_tr = seek_tr + 1)
		{
			uint32 CurrentDataIndex = 0;

			/*Reset Learner State*/
			Learner.iClearAvailableStateList();
			Learner.iClearStateStack();
			for (uint32 seek_x = 0; seek_x < 3; seek_x = seek_x + 1)
			{
				for (uint32 seek_y = 0; seek_y < 3; seek_y = seek_y + 1)
				{
					Learner.iAddAvailableState(POSITION2D(seek_x, seek_y));
				}
			}

			/*Start a New Game and Render Board*/
			Game.iClearBoard();
			Game.iClearPlayer();
			if (TrainSetList[seek_tr][0].x == 1)
			{
				PlayerLearner = Game.iCreatePlayer(GetAIMove, (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, true));
				PlayerSuperviser = Game.iCreatePlayer(GetUserMove, 0);
			}
			else
			{
				PlayerSuperviser = Game.iCreatePlayer(GetUserMove, 0);
				PlayerLearner = Game.iCreatePlayer(GetAIMove, (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, true));
			}
			CurrentDataIndex = CurrentDataIndex + 1;

			TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

			while (true)
			{
				/*One Move*/
				CurrentPlayer = Game.iGetCurrentPlayerID();
				if (CurrentPlayer == PlayerLearner)
				{
					Res = Game.iSetChess(CurrentPlayer, Learner.iSupervisedLearning(TrainSetList[seek_tr][CurrentDataIndex]));
				}
				else
				{
					Res = Game.iSetChess(CurrentPlayer, TrainSetList[seek_tr][CurrentDataIndex]);
				}
				CurrentDataIndex = CurrentDataIndex + 1;

				if (Game.iGetLastMovePosition(Pos))
				{
					Learner.iRemoveAvailableStateByValue(Pos);
				}

				TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

				/*Winner Estimate*/
				if ((Res & 0xff00) == eCHESSRESULT::SR_WIN)
				{
					/*Update Learner State*/
					((Res & 0xff) == PlayerLearner) ? Learner.iFeedback(WinFeedBack) : Learner.iFeedback(LoseFeedBack);
					pTerminal->iOutputString(_SSTR("Player ") + ::gloIToS((Res & 0xff)) + _SSTR(" Win\n"));//, press any key to start a new game...";
					//::system("pause");
					break;
				}
				if (Res == eCHESSRESULT::SR_DRAW)
				{
					/*Update Learner State*/
					Learner.iFeedback(DrawFeedBack);
					pTerminal->iOutputString(_SSTR("Game Result: Draw\n"));//, press any key to start a new game...";
					//::system("pause");
					break;
				}
			}
		}
	}
}
/*Self Lerning Function*/
void SelfLearning(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal)
{
	if (Times == 0)
	{
		return;
	}

	uint32		PlayerLearner = 0;
	uint32		PlayerSuperviser = 0;
	uint32		Res = 0;
	uint32		CurrentPlayer = 0;
	POSITION2D	Pos;

	SHANDLE	LParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, true);
	SHANDLE	SParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, false);

	/*Game Loop*/
	for (uint32 seek = 0; seek < Times; seek = seek + 1)
	{
		/*Reset Learner State*/
		Learner.iClearAvailableStateList();
		Learner.iClearStateStack();
		for (uint32 seek_x = 0; seek_x < 3; seek_x = seek_x + 1)
		{
			for (uint32 seek_y = 0; seek_y < 3; seek_y = seek_y + 1)
			{
				Learner.iAddAvailableState(POSITION2D(seek_x, seek_y));
			}
		}

		/*Start a New Game and Render Board*/
		Game.iClearBoard();
		Game.iClearPlayer();
		Pos = TicTacToeCreatePlayer(Game, LParam, SParam, 2);
		PlayerLearner = Pos.x;
		PlayerSuperviser = Pos.y;

		TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

		while (true)
		{
			/*One Move*/
			CurrentPlayer = Game.iGetCurrentPlayerID();
			Res = Game.iUpdateBoard(pTerminal);

			if (Game.iGetLastMovePosition(Pos))
			{
				Learner.iRemoveAvailableStateByValue(Pos);
			}

			TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

			/*Winner Estimate*/
			if ((Res & 0xff00) == eCHESSRESULT::SR_WIN)
			{
				/*Update Learner State*/
				if ((Res & 0xff) == PlayerLearner)
				{
					Learner.iFeedback(WinFeedBack);
				}
				else
				{
					Learner.iFeedback(LoseFeedBack);
				}
				pTerminal->iOutputString(_SSTR("Player ") + ::gloIToS(Res & 0xff) + _SSTR(" Win\n"));//, press any key to start a new game...";
				//::system("pause");
				break;
			}
			if (Res == eCHESSRESULT::SR_DRAW)
			{
				/*Update Learner State*/
				Learner.iFeedback(DrawFeedBack);
				pTerminal->iOutputString(_SSTR("Game Result: Draw\n"));//, press any key to start a new game...";
				//::system("pause");
				break;
			}
		}
	}
}
/*Context Function*/
void ContextFunc(cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal)
{
	uint32		PlayerLearner = 0;
	uint32		PlayerSuperviser = 0;
	uint32		Res = 0;
	uint32		CurrentPlayer = 0;
	POSITION2D	Pos;

	SHANDLE	LParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, false);
	SHANDLE	SParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, false);

	/*Game Loop*/
	while (true)
	{
		/*Reset Learner State*/
		Learner.iClearAvailableStateList();
		Learner.iClearStateStack();
		for (uint32 seek_x = 0; seek_x < 3; seek_x = seek_x + 1)
		{
			for (uint32 seek_y = 0; seek_y < 3; seek_y = seek_y + 1)
			{
				Learner.iAddAvailableState(POSITION2D(seek_x, seek_y));
			}
		}

		/*Start a New Game and Render Board*/
		Game.iClearBoard();
		Game.iClearPlayer();
		Pos = TicTacToeCreatePlayer(Game, LParam, SParam, 1);
		PlayerLearner = Pos.x;
		PlayerSuperviser = Pos.y;

		TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

		while (true)
		{
			/*One Move*/
			CurrentPlayer = Game.iGetCurrentPlayerID();
			Res = Game.iUpdateBoard(pTerminal);

			if (Game.iGetLastMovePosition(Pos))
			{
				Learner.iRemoveAvailableStateByValue(Pos);
			}

			TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

			/*Winner Estimate*/
			if ((Res & 0xff00) == eCHESSRESULT::SR_WIN)
			{
				/*Update Learner State*/
				pTerminal->iOutputString((Res & 0xff) == PlayerLearner ? _SSTR("Learner Win\n") : _SSTR("Superviser Win\n"));
				::system("pause");
				break;
			}
			if (Res == eCHESSRESULT::SR_DRAW)
			{
				/*Update Learner State*/
				//Learner.iFeedback(0.0);
				pTerminal->iOutputString(_SSTR("Game Result: Draw\n"));//, press any key to start a new game...";
				::system("pause");
				break;
			}
		}
	}
}
void ContextFuncAIAI(const uint32 Times, cTicTacToeGame &Game, cLinearLearningAlgorithm<POSITION2D> &Learner, cSanTerminalDevice* pTerminal)
{
	uint32		PlayerLearner = 0;
	uint32		PlayerSuperviser = 0;
	uint32		Res = 0;
	uint32		CurrentPlayer = 0;
	POSITION2D	Pos;

	cLinearLearningAlgorithm<POSITION2D> RandomLearner;
	RandomLearner.iCreateFeature(_SSTR("X0"), 1.0, &Game, OrigionalFeatureFunc);

	SHANDLE	LParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&Learner, false);
	SHANDLE	SParam = (SHANDLE) &pair<cLinearLearningAlgorithm<POSITION2D>*, bool>(&RandomLearner, false);


	/*Game Loop*/
	for (uint32 seek = 0; seek < Times; seek = seek + 1)
	{
		/*Reset Learner State*/
		Learner.iClearAvailableStateList();
		Learner.iClearStateStack();
		RandomLearner.iClearAvailableStateList();
		RandomLearner.iClearStateStack();
		for (uint32 seek_x = 0; seek_x < 3; seek_x = seek_x + 1)
		{
			for (uint32 seek_y = 0; seek_y < 3; seek_y = seek_y + 1)
			{
				Learner.iAddAvailableState(POSITION2D(seek_x, seek_y));
				RandomLearner.iAddAvailableState(POSITION2D(seek_x, seek_y));
			}
		}

		/*Start a New Game and Render Board*/
		Game.iClearBoard();
		Game.iClearPlayer();
		Pos = TicTacToeCreatePlayer(Game, LParam, SParam, 2);
		PlayerLearner = Pos.x;
		PlayerSuperviser = Pos.y;

		TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

		while (true)
		{
			/*One Move*/
			CurrentPlayer = Game.iGetCurrentPlayerID();
			Res = Game.iUpdateBoard(pTerminal);

			if (Game.iGetLastMovePosition(Pos))
			{
				Learner.iRemoveAvailableStateByValue(Pos);
				RandomLearner.iRemoveAvailableStateByValue(Pos);
			}

			TicTacToePrintBoard(Game, PlayerLearner, PlayerSuperviser, pTerminal);

			/*Winner Estimate*/
			if ((Res & 0xff00) == eCHESSRESULT::SR_WIN)
			{
				/*Update Learner State*/
				pTerminal->iOutputString((Res & 0xff) == PlayerLearner ? _SSTR("Learner Win\n") : _SSTR("Superviser Win\n"));
				::system("pause");
				break;
			}
			if (Res == eCHESSRESULT::SR_DRAW)
			{
				/*Update Learner State*/
				//Learner.iFeedback(0.0);
				pTerminal->iOutputString(_SSTR("Game Result: Draw\n"));//, press any key to start a new game...";
				::system("pause");
				break;
			}
		}
	}
}

/*Uesr Input Function*/
POSITION2D GetUserMove(SHANDLE lParam, SHANDLE wParam, cSanTerminalDevice* pTerminal)
{
	POSITION2D Pos;
	char Buffer[512];
	while (true)
	{
		pTerminal->iOutputString(_SSTR("Player ") + ::gloIToS(((cTicTacToeGame*) lParam)->iGetCurrentPlayerID()) + _SSTR(" Move (x,y):"));
		::cin >> Buffer;
		vector<SStringA> Item = ::gloGetStringItemsA(Buffer, ",");
		if (Item.size() != 2)
		{
			pTerminal->iOutputString(_SSTR("Invalid Input\n"), STC_WHITE, STC_RED);
			continue;
		}
		Pos.x = ::atoi(Item[0].c_str());
		Pos.y = ::atoi(Item[1].c_str());
		break;
	}
	return Pos;
}

/*AI Input Function*/
POSITION2D GetAIMove(SHANDLE lParam, SHANDLE wParam, cSanTerminalDevice* pTerminal)
{
	pair<cLinearLearningAlgorithm<POSITION2D>*, bool>* pLearnerParam = (pair<cLinearLearningAlgorithm<POSITION2D>*, bool>*)wParam;
	if (pLearnerParam == nullptr)
	{
		pTerminal->iOutputString(_SSTR("Error: The Learner Object Pointer is Empty\n"), STC_WHITE, STC_RED);
		::system("pause");
		::exit(0);
	}
	return pLearnerParam->first->iPredict(pLearnerParam->second);
}