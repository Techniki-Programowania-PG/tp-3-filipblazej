#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <map>

#pragma comment(lib, "Gdiplus.lib")

using namespace std;
using namespace Gdiplus;
ULONG_PTR gdiplusToken;
HWND hWeightLabel = NULL;

const int MAX_WEIGHT = 600;
const int AVG_PERSON_WEIGHT = 70;
#define MAX_FLOORS 5
#define TIMER_ID 1

class Person {
public:
	int start;
	int destination;
};

class Elevator {
public:
	int passengers = 0;
	int currentfloor = 0;
	vector<Person> inside;
	map<int ,vector<Person>> waiting;
    bool moving = false;
	bool Up = true;

    void move() {
        if (inside.empty() && allWaitingEmpty()) {
            idleCount++;
            if (idleCount > 25 && currentfloor != 0) {
                moving = true;
                currentfloor--;
            }
            return;
        }

        idleCount = 0;

        moving = true;

        auto it = std::remove_if(inside.begin(), inside.end(), [&](Person& p) {
            return p.destination == currentfloor;
            });
        inside.erase(it, inside.end());

        auto& queue = waiting[currentfloor];
        auto it2 = queue.begin();
        bool tookPassenger = false;

        while (it2 != queue.end()) {
            bool sameDir = Up ? it2->destination > currentfloor : it2->destination < currentfloor;
            if (canEnter() && sameDir) {
                inside.push_back(*it2);
                it2 = queue.erase(it2);
                tookPassenger = true;
            }
            else {
                ++it2;
            }
        }

        if (!tookPassenger && !queue.empty()) {
            Up = !Up;
            it2 = queue.begin();
            while (it2 != queue.end()) {
                bool sameDir = Up ? it2->destination > currentfloor : it2->destination < currentfloor;
                if (canEnter() && sameDir) {
                    inside.push_back(*it2);
                    it2 = queue.erase(it2);
                }
                else {
                    ++it2;
                }
            }
        }

        auto someoneIsWaitingAbove = [&]() {
            for (int i = currentfloor + 1; i <= MAX_FLOORS; ++i) {
                if (!waiting[i].empty()) return true;
            }
            return false;
            };

        auto someoneIsWaitingBelow = [&]() {
            for (int i = currentfloor - 1; i >= 0; --i) {
                if (!waiting[i].empty()) return true;
            }
            return false;
            };

        auto wantsThisWay = [&](bool up) {
            for (auto& p : inside)
                if ((up && p.destination > currentfloor) || (!up && p.destination < currentfloor))
                    return true;
            return false;
            };

        if (!wantsThisWay(Up)) {
            if (!inside.empty()) {
                if (wantsThisWay(!Up)) {
                    Up = !Up;
                }
                else if (currentfloor != 0) {
                    currentfloor--;
                }
                return;
            }
            else {
                if (someoneIsWaitingAbove()) {
                    Up = true;
                    currentfloor++;
                }
                else if (someoneIsWaitingBelow()) {
                    Up = false;
                    currentfloor--;
                }
                return;
            }
        }

        if (Up && currentfloor < MAX_FLOORS)
            currentfloor++;
        else if (!Up && currentfloor > 0)
            currentfloor--;
    }




	int totalWeight() {
		return inside.size() * 70;
	}

	bool canEnter() {
		return totalWeight() + 70 <= 600;
	}

    bool allWaitingEmpty() {
        for (auto& pair : waiting)
            if (!pair.second.empty()) 
                return false;
        return true;
    }
private:
    int idleCount = 0;
};

Elevator elevator;

void Draw_elevator(HDC hdc) {
    Graphics graphics(hdc);
    int y = 430 - elevator.currentfloor * 100;

    SolidBrush brush(Color(255, 105, 162, 255));
    SolidBrush brush_1(Color(165, 212, 217));
    SolidBrush brush_2(Color(107, 76, 55));
    Pen pen(Color(255, 0, 0, 0), 2);

    graphics.FillRectangle(&brush, 50, y, 50, 70);
    graphics.FillRectangle(&brush_1, 60, y + 10, 25, 50);
    graphics.FillRectangle(&brush_2, 95, y, 5, 70);
    graphics.DrawRectangle(&pen, 50, y, 50, 70);
}


void Draw_floor(HDC hdc) {
    Graphics graphics(hdc);
    Pen floorPen(Color(255, 0, 0, 0), 1);
    SolidBrush textBrush(Color(255, 0, 0, 0));
    Font font(L"Arial", 14);

    int floorCount = 5;
    int floorSpacing = 100;
    int floorStartY = 100;

    for (int i = floorCount - 1; i >= 0; --i) {
        int yLine = floorStartY + (floorCount - 1 - i) * floorSpacing;
        graphics.DrawLine(&floorPen, 50, yLine, 200, yLine);

        int passengersWaiting = elevator.waiting[i].size();
        std::wstring label = L"Pasażerów: " + std::to_wstring(passengersWaiting);
        graphics.DrawString(label.c_str(), -1, &font, PointF(210, yLine - 15), &textBrush);
    }

}

void Draw_weight(HDC hdc) {
    Graphics graphics(hdc);
    SolidBrush brushText(Color(255, 0, 0, 0));
    std::wstring txt = L"Waga: " + std::to_wstring(elevator.totalWeight()) + L"kg";
    graphics.DrawString(txt.c_str(), -1, new Font(L"Arial", 16), PointF(10, 10), &brushText);
}

void AddPassenger(int floor, int destination) {
	if (floor < 0 || floor > MAX_FLOORS || destination < 0 || destination > MAX_FLOORS || floor == destination)
		return;
	elevator.waiting[floor].push_back(Person{floor, destination });
}

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ElevatorSim";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"ElevatorSim", L"Symulator Windy",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1000, 600,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    SetTimer(hwnd, TIMER_ID, 200, NULL);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

void CreatePassengerButtons(HWND hwnd) {
    const int buttonWidth = 80;
    const int buttonHeight = 20;
    const int startX = 10;
    const int startY = 10;
    const int verticalSpacing = 5;
    const int horizontalSpacing = 10;

    for (int floor = 0; floor <= MAX_FLOORS-1; ++floor) {
        for (int dest = 0; dest <= MAX_FLOORS-1; ++dest) {
            if (floor == dest) continue;

            std::wstring text = L"Z " + std::to_wstring(floor) + L" do " + std::to_wstring(dest);
            int x = startX + dest * (buttonWidth + horizontalSpacing) + 350;
            int y = startY + (MAX_FLOORS - floor) * (buttonHeight + verticalSpacing);

            CreateWindow(L"BUTTON", text.c_str(), WS_VISIBLE | WS_CHILD,
                x, y, buttonWidth, buttonHeight,
                hwnd, (HMENU)(1000 + floor * 10 + dest), NULL, NULL);
        }
    }
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreatePassengerButtons(hwnd);
        break;
    case WM_COMMAND:
        if (LOWORD(wParam) >= 1000) {
            int id = LOWORD(wParam) - 1000;
            int floor = id / 10;
            int dest = id % 10;
            AddPassenger(floor, dest);
        }
        break;
    case WM_TIMER:
        elevator.move();
        if(elevator.moving)
        InvalidateRect(hwnd, NULL, TRUE);
        elevator.moving = false;
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        Draw_elevator(hdc);
        Draw_floor(hdc);
        Draw_weight(hdc);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}