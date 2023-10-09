#pragma once

#include <list>
#include "Card.h"

namespace solitaire
{
	class GameLogic
	{
		const int BOARD_ROW = 5;
		const int BOARD_COL = 8;
		const Gdiplus::RectF BOARD_COUNT_RECT{ 885.0f, 60.0f, 120.0f, 30.0f };

	private:
		HWND mHwnd;
		std::unique_ptr<Gdiplus::Image> mBackground;
		std::list<Card> mDeck;
		int mClickCount;
		Card* mpPrevious;

	private:
		void CreateCards();

	public:
		void Init(HWND hwnd);
		void Release();
		void Draw(Gdiplus::Graphics& graphics);
		void OnClick(int x, int y);

	};


}