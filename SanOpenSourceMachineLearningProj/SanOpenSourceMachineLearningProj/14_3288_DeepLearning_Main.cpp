#include"iostream"
#include"process.h"
#include"ProblemSet\ProblemUIFunc.h"
using namespace std;
using namespace San;
using namespace San::Device;
using namespace San::FileIOStream;
using namespace San::MachineLearning;

int main(int argc, char** argv)
{
	::srand(time(0));

	cSanTerminalDevice Terminal;
	Terminal.iSetTerminalTittle(_SSTR("San Machine Learning Terminal"));

	SString strOutput;

	char Buffer[1024];

	Terminal.iOutputString(_SSTR("Please select search problem set: \r\n"));
	Terminal.iOutputString(_SSTR("\tTTT                   for TicTacToe Linear Learning Problem\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tDT                    for Decision Tree Algorithm\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tNN                    for Artificial Neural Network Algorithm\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tGA                    for Genetic Algorithm\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("\tDNN                   for Deep Neural Network Algorithm\r\n"), STC_GREY);
	Terminal.iOutputString(_SSTR("Commander> "));
	::cin.getline(Buffer, 1024);

	SString strCommand = Buffer;


	if (strCommand == _SSTR("TTT")){ TicTacToeLearningProblem(&Terminal, &strOutput); }
	//if (strCommand == _SSTR("DT")){ HOPProblem(&Terminal, &strOutput); }
	//if (strCommand == _SSTR("NN")){ MCProblem(&Terminal, &strOutput); }
	//if (strCommand == _SSTR("GA")){ CNFProblem(&Terminal, &strOutput); }
	//if (strCommand == _SSTR("ResolutionAlgorithm")){ ResolutionAlgorithmProblem(&Terminal, &strOutput); }

	::gloWriteFile(_SSTR("Output.txt"), strOutput);
	::system("notepad Output.txt");

	::system("pause");
	return 0;
}