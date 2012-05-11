typedef int GID;

#define CONSOLE_OPENED 1
#define CONSOLE_CLOSED 0

#define CONSOLE_NAME "txtConsole"

#define CONSOLE_MEMO_NAME "memoConsole" // [ender]

#define GUI_CONSOLE_GROUP 15123

class Console
{
public:
	int console_state;

	
	Console();
	
	void ShowConsole();
	void HideConsole();
	void CreateConsole();
	void Execute();

	// Для восстановления предыдущих контролов
	map<GID, bool> controls;
	//А надо ли их вообще прятать
};