#include<iostream>
using namespace std;

// wall = #, goal = . , box = o, player = p
const char gStageData[] = "\
########\n\
#      #\n\
# ..p  #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

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

// Function Prototype �Լ� ������Ÿ��
void Initialize(Object* state, int w, int h, const char* stageData); // ���ڿ��� ������Ʈ�� �ٲ�
void Draw(Object* state, int w, int h); // ȭ�� �׸���
void Update(Object* state, char input, int w, int h); // ������Ʈ
bool CheckClear(const Object* state, int w, int h); // Ŭ�����ߴ��� �˻�


int main()
{
	// ���� �迭 Ȯ��
	Object* state = new Object[gStageWidth * gStageHeight];
	// �������� �ʱ�ȭ
	Initialize(state, gStageWidth, gStageHeight, gStageData);
	

	while (true)
	{
		// ȭ�� �׸���
		Draw(state, gStageWidth, gStageHeight);
		// Ŭ���� üũ
		if (CheckClear(state, gStageWidth, gStageHeight))
		{
			break; // Ŭ���� üũ
		}
		// �Է°� ȹ��
		cout << " a: left, d : right, w : up, s : down, command? " << endl; // ���ۼ���
		char input;
		cin >> input;
		// ����
		Update(state, input, gStageWidth, gStageHeight);
	}

	// ���� �޼���
	cout << "Congratulation" << endl;
	// ������
	delete[] state; // �迭�� new �� ���������Ƿ� delete �� �ƴ϶� delete[]
	state = 0; // �� ��Ģ  delete �� �����Ϳ��� 0�� �ִ´�

	return 0;
}

void Initialize(Object* state, int w, int h, const char* stageData)
{
	const char* d = stageData; // �б�� ������
	int x = 0;
	int y = 0;
	while (*d != '\0')// null ���ڰ� �ƴ� ����
	{
		Object t;
		switch ( *d)
		{
		case '#': t = OBJ_WALL; break;
		case ' ': t = OBJ_SPACE; break;
		case 'o': t = OBJ_BLOCK; break;
		case '0': t = OBJ_BLOCK_ON_GOAL; break;
		case '.': t = OBJ_GOAL; break;
		case 'p': t = OBJ_MAN; break;
		case 'P': t = OBJ_MAN_ON_GOAL; break;
		case '\n':
			x = 0; // x �� ���� ������ �ǵ�����
			++y; // y�� �Ѵܰ� �Ʒ���
			t = OBJ_UNKNOWN; // �����ʹ� ����
			break;
		default:
			t = OBJ_UNKNOWN; break; // ���� ������
		}
		++d; // ?
		// �� if ���� �𸣴� ���ڸ� �����Ѵ�
		if (t != OBJ_UNKNOWN) 
		{
			state[y * w + x] = t; // ���� ( ���� �� ĭ���� ���� ���������� x, �Ʒ��� y ��ŭ ������ ���) p60����
			++x;
		}
	}
	//throw gcnew System::NotImplementedException();
}

void Draw(Object* state, int w, int h)
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

void Update(Object* state, char input, int w, int h)
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
	if (tx < 0 || ty <  0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. �̵��� ���� ��ĭ �Ǵ� ������ -> �÷��̾� �̵�
	int p = y * w + x; // �÷��̾� ���� ��ġ
	int tp = ty * w + tx; // ��ǥ ��ġ ( TargetPosition)
	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// �̵��� ���� ��������� ������ �� ���, �ƴϸ� �׳� ���
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// ���� �÷��̾��� ��ġ �� ������ ����� �������� �ƴϸ� ��������
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	// B. �̵��� ���� ���� -> �� ���� ���� ĭ�� �����̳� �������� �̵�
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
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


	//throw gcnew System::NotImplementedException();
}




