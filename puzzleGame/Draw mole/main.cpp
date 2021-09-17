#include "GameLib/Framework.h"
//using namespace GameLib;
// 열거형
enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};
namespace GameLib
{
	void Framework::update()
	{
		GetInput();
		updateGame();
		draw();
		unsigned* vram = videoMemory();
		vram[200 * width() + 100] = 0xff0000;
	}
}
void draw(Object* state, int w, int h)
{
	// Object 열거형 순서
	const char font[] = { ' ', '#', '.','o','0','p','P' };
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			Object o = state[y * w + x];
			cout << font[o];
		}
		cout << endl;
	}
	//throw gcnew System::NotImplementedException();
}

void update(Object* state, char input, int w, int h)
{
	// 이동량으로 변환
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a':dx = -1; break;
	case 'd':dx = 1; break;
	case 'w': dy = -1; break;
	case 's': dy = 1; break;
	default:
		break;
	}

	// 플레이어 좌표 검색
	int i = -1; // 의미는 없지만 초기화 시키기
	for (i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % w; // x 는 폭으로 나눈 나머지
	int y = i / w; // y 는 폭으로 나눈 몫


	// 이동 후 좌표
	// difference(차이), temporary(일시적인)
	int tx = x + dx;
	int ty = y + dy;
	// 좌표에서 최대최소 검사. 벗어나면 허가하지 않는다.
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. 이동할 곳이 빈칸 또는 목적지 -> 플레이어 이동
	int p = y * w + x; // 플레이어 현재 위치
	int tp = ty * w + tx; // 목표 위치 ( TargetPosition)

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL) // 1번째 if - 이동할곳이 빈칸이면 이동(공백, 목적지)
	{
		// 이동할 곳이 목적지라면 목적지 위 사람, 아니면 그냥 사람
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// 현재 플레이어의 위치 가 목적지 위라면 목적지로 아니면 공백으로
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	// B. 이동할 곳이 상자 -> 그 방향 다음 칸이 공백이나 목적지면 이동
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL) // 2번째 if - 이동할 곳에 상자
	{
		// 2칸 앞이 범위 내인지 검사
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) // 2칸 앞이 필드끝일 경우
		{
			return; // 밀 수 없다.
		}

		int tp2 = (ty + dy) * w + (tx + dx); // 2칸 앞
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			// 순차적으로 교체
			state[tp2] = (state[tp2] == OBJ_GOAL) ?
				OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ?
				OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ?
				OBJ_GOAL : OBJ_SPACE;
		}
	}
	else if (state[tp] == OBJ_WALL) // 내가 임의로 추가한 코드 p 65 참고
		return;
	//throw gcnew System::NotImplementedException();
}

