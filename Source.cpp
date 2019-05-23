#include <iostream>
#include <windows.h>


bool clicking;
POINT point;
DWORD Milliseconds;
WORD Virtualkey;
HANDLE hStdin;
DWORD fdwSaveOldMode;

void KeyEventProc(KEY_EVENT_RECORD);

void setTimer() {
	std::cout << "type how many milliseconds you want the auto clicker to rest between clicks?" << std::endl;
	std::cin >> Milliseconds;
}


void mouse_clicker() {
	clicking = true;

	SetCursorPos(point.x, point.y);

	setTimer();

	while (true)
	{
		INPUT ip;
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &ip, sizeof(INPUT));

		ZeroMemory(&ip, sizeof(INPUT));


		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &ip, sizeof(INPUT));


		if (GetAsyncKeyState(VK_NUMPAD1)) {
			clicking = false;
			break;
		}


		Sleep(Milliseconds);
	}
}

int GetpressedKey() {
	// ref : https://docs.microsoft.com/en-us/windows/console/reading-input-buffer-events

	DWORD cNumRead, fdwMode, i;
	INPUT_RECORD irInBuf[128]; // i think i need to lower this value cuz its too big what do you think?
	int counter = 0;

	// Get the standard input handle. 

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		std::cout << "getstdhandle" << std::endl;

	// Save the current input mode, to be restored on exit. 

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		return 0;

	// Enable the window and mouse input events. 

	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	if (!SetConsoleMode(hStdin, fdwMode))
		return 0;

	// Loop to read and handle the next 1 and only input event. 

	std::cout << "Press a key you want to get auto pressed" << std::endl;

	while (counter++ <= 1)
	{
		// Wait for the events. 

		if (!ReadConsoleInput(
			hStdin,      // input buffer handle 
			irInBuf,     // buffer to read into 
			128,         // size of read buffer 
			&cNumRead)) // number of records read 
			return 0;

		// Dispatch the events to the appropriate handler. 

		for (i = 0; i < cNumRead; i++)
		{
			switch (irInBuf[i].EventType)
			{
			case KEY_EVENT: // keyboard input 
				KeyEventProc(irInBuf[i].Event.KeyEvent);
				break;

			default:
				break;
			}

		}
	}
}

void KeyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown)
		Virtualkey = ker.wVirtualKeyCode;
}

void keyboard_clicker() {
	clicking = true;

	setTimer();

	GetpressedKey();

	while (true) {
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wScan = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		ip.ki.wVk = Virtualkey;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release  key
		ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
		SendInput(1, &ip, sizeof(INPUT));

		if (GetAsyncKeyState(VK_NUMPAD3)) {
			clicking = false;
			break;
		}
		Sleep(Milliseconds);
	}
}


void StartClicker() {

	while (true)
	{
		if (GetAsyncKeyState(VK_HOME)) {
			// capture mosue postion
			GetCursorPos(&point);
		}

		if (GetAsyncKeyState(VK_DELETE)) {
			// exit program
			break;
		}

		if (GetAsyncKeyState(VK_NUMPAD0)) {
			// start mouse clicking here in a while loop
			mouse_clicker();
		}

		if (GetAsyncKeyState(VK_NUMPAD2)) {
			// keyboard clicking.
			keyboard_clicker();
		}

		Sleep(300);
	}
}


void printConsoleMenu() {
	clicking = false;

	// print stuff to the user
	std::cout << "[+] press [Home key] to capture mouse postion" << std::endl;
	std::cout << "[+] press [Delete key] to exit the program!" << std::endl;
	std::cout << "[+] press [Numpad 0] to start [mouse]autoclicker" << std::endl;
	std::cout << "[+] press [Numpad 1] to stop  [mouse]autoclicker" << std::endl;
	std::cout << "[+] press [Numpad 2] to start [keyboard]autoclicker" << std::endl;
	std::cout << "[+] press [Numpad 3] to stop  [keyboard]autoclicker" << std::endl;

	StartClicker();
}



int main() {
	printConsoleMenu();
}