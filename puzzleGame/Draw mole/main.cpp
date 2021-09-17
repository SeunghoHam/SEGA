#include "GameLib/Framework.h"
//using namespace GameLib;
// ������
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
	// Object ������ ����
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
	// �̵������� ��ȯ
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

	// �÷��̾� ��ǥ �˻�
	int i = -1; // �ǹ̴� ������ �ʱ�ȭ ��Ű��
	for (i = 0; i < w * h; i++)
	{
		if (state[i] == OBJ_MAN || state[i] == OBJ_MAN_ON_GOAL)
		{
			break;
		}
	}

	int x = i % w; // x �� ������ ���� ������
	int y = i / w; // y �� ������ ���� ��


	// �̵� �� ��ǥ
	// difference(����), temporary(�Ͻ�����)
	int tx = x + dx;
	int ty = y + dy;
	// ��ǥ���� �ִ��ּ� �˻�. ����� �㰡���� �ʴ´�.
	if (tx < 0 || ty < 0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. �̵��� ���� ��ĭ �Ǵ� ������ -> �÷��̾� �̵�
	int p = y * w + x; // �÷��̾� ���� ��ġ
	int tp = ty * w + tx; // ��ǥ ��ġ ( TargetPosition)

	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL) // 1��° if - �̵��Ұ��� ��ĭ�̸� �̵�(����, ������)
	{
		// �̵��� ���� ��������� ������ �� ���, �ƴϸ� �׳� ���
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// ���� �÷��̾��� ��ġ �� ������ ����� �������� �ƴϸ� ��������
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	// B. �̵��� ���� ���� -> �� ���� ���� ĭ�� �����̳� �������� �̵�
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL) // 2��° if - �̵��� ���� ����
	{
		// 2ĭ ���� ���� ������ �˻�
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) // 2ĭ ���� �ʵ峡�� ���
		{
			return; // �� �� ����.
		}

		int tp2 = (ty + dy) * w + (tx + dx); // 2ĭ ��
		if (state[tp2] == OBJ_SPACE || state[tp2] == OBJ_GOAL)
		{
			// ���������� ��ü
			state[tp2] = (state[tp2] == OBJ_GOAL) ?
				OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			state[tp] = (state[tp] == OBJ_BLOCK_ON_GOAL) ?
				OBJ_MAN_ON_GOAL : OBJ_MAN;
			state[p] = (state[p] == OBJ_MAN_ON_GOAL) ?
				OBJ_GOAL : OBJ_SPACE;
		}
	}
	else if (state[tp] == OBJ_WALL) // ���� ���Ƿ� �߰��� �ڵ� p 65 ����
		return;
	//throw gcnew System::NotImplementedException();
}

