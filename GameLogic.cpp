#include "GameLogic.h"
#include <string>
#include <vector>
#include <random>

using namespace Gdiplus;

namespace solitaire
{
	void solitaire::GameLogic::CreateCards()
	{
	
		std::vector<Type> types;
		while (types.size() < static_cast<size_t>(BOARD_COL * BOARD_ROW))
		{
			int mod = types.size() % 6;

			switch (mod)
			{
			case 0:
				types.push_back(Type::Bear);
				types.push_back(Type::Bear);
				break;

			case 2:
				types.push_back(Type::Wolf);
				types.push_back(Type::Wolf);
				break;

			case 4:
				types.push_back(Type::Dragon);
				types.push_back(Type::Dragon);
				break;
			}
		}

		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(types.begin(), types.end(), g);

		int index{};
		int posX{ 15 }, posY{ 15 };

		for (int x = 0; x < BOARD_COL; ++x)
		{
			posY = 15;
			for (int y = 0; y < BOARD_ROW; ++y)
			{
				mDeck.push_back(Card(mHwnd, index, types[index++], posX, posY));
				posY += 150;
			}

			posX += 110;
		}
	}

	void solitaire::GameLogic::Init(HWND hwnd)
	{
		mHwnd = hwnd;
		mBackground = std::make_unique<Image>(L"Images/bg_blank.png");
		CreateCards();
	}

	void solitaire::GameLogic::Release()
	{
		mDeck.clear();
		mBackground.reset(); // ��۸� ����Ʈ �������� ��.
	}

	void solitaire::GameLogic::Draw(Gdiplus::Graphics& graphics)
	{
		graphics.DrawImage(mBackground.get(), 0, 0, mBackground->GetWidth(), mBackground->GetHeight());

		for (auto& card : mDeck)
		{
			card.Draw(graphics);
		}

		Gdiplus::PointF pos(895.0f, 20.0f);
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, 79, 64)); // �귯��, ��Ʈ: ��� �ε� �� �ʿ� ������ ����� ����� ������
		Gdiplus::Font font(L"���� ���", 20);
		graphics.DrawString(L"Ŭ����:", -1, &font, pos, &brush);

		Gdiplus::StringFormat format;
		format.SetAlignment(StringAlignmentCenter); // ���η� �߾� ����
		format.SetLineAlignment(StringAlignmentCenter); // ���η� �߾� ����
		graphics.DrawString(std::to_wstring(mClickCount).c_str(), -1, &font, BOARD_COUNT_RECT, &format, &brush);
	}
	// mClickCount:�ƽ�Ű �ڵ�, wstring �ؼ� �����ڵ�� �ٲ�� ��
	void solitaire::GameLogic::OnClick(int x, int y)
	{
		Card* pCard{};

		for (auto& card : mDeck)
		{
			if (card.CheckClicked(x, y))
			{
				pCard = &card;
				break;
			}

		}

		if (pCard)
		{
			mClickCount++;
			RECT rct{ static_cast<LONG>(BOARD_COUNT_RECT.GetLeft()), static_cast<LONG>(BOARD_COUNT_RECT.GetTop()),
					static_cast<LONG>(BOARD_COUNT_RECT.GetRight()),static_cast<LONG>( BOARD_COUNT_RECT.GetBottom()) };
			InvalidateRect(mHwnd, &rct, true); // false -> true

			if (mpPrevious == nullptr)
			{
				mpPrevious = pCard;
			}
			else
			{
				if (mpPrevious != pCard)
				{
					if (pCard->GetType() == mpPrevious->GetType())
					{
						mpPrevious->Invalidate();
						pCard->Invalidate();

						mDeck.remove_if([&](Card& card) {return card.GetIndex() == pCard->GetIndex(); });
						mDeck.remove_if([&](Card& card) {return card.GetIndex() == mpPrevious->GetIndex(); });

					}
					else
					{
						UpdateWindow(mHwnd); // ȭ���� �ѹ� ����
						Sleep(500); // 500ms(0.5s)
						pCard->Flip(false);
						mpPrevious->Flip(false);
					}
				}

				mpPrevious = nullptr;
			}
		}
	}

}