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

// Function Prototype 함수 프로토타입
void Initialize(Object* state, int w, int h, const char* stageData); // 문자열을 오브젝트로 바꿈
void Draw(Object* state, int w, int h); // 화면 그리기
void Update(Object* state, char input, int w, int h); // 업데이트
bool CheckClear(const Object* state, int w, int h); // 클리어했는지 검사


int main()
{
	// 상태 배열 확보
	Object* state = new Object[gStageWidth * gStageHeight];
	// 스테이지 초기화
	Initialize(state, gStageWidth, gStageHeight, gStageData);
	

	while (true)
	{
		// 화면 그리기
		Draw(state, gStageWidth, gStageHeight);
		// 클리어 체크
		if (CheckClear(state, gStageWidth, gStageHeight))
		{
			break; // 클리어 체크
		}
		// 입력값 획득
		cout << " a: left, d : right, w : up, s : down, command? " << endl; // 조작설명
		char input;
		cin >> input;
		// 갱신
		Update(state, input, gStageWidth, gStageHeight);
	}

	// 축하 메세지
	cout << "Congratulation" << endl;
	// 뒷정리
	delete[] state; // 배열을 new 로 생성했으므로 delete 가 아니라 delete[]
	state = 0; // 내 규칙  delete 한 포인터에는 0을 넣는다

	return 0;
}

void Initialize(Object* state, int w, int h, const char* stageData)
{
	const char* d = stageData; // 읽기용 포인터
	int x = 0;
	int y = 0;
	while (*d != '\0')// null 문자가 아닌 동안
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
			x = 0; // x 를 왼쪽 끝으로 되돌리기
			++y; // y는 한단계 아래로
			t = OBJ_UNKNOWN; // 데이터는 없음
			break;
		default:
			t = OBJ_UNKNOWN; break; // 부정 데이터
		}
		++d; // ?
		// 이 if 문은 모르는 문자면 무시한다
		if (t != OBJ_UNKNOWN) 
		{
			state[y * w + x] = t; // 쓰기 ( 왼쪽 위 칸에서 부터 오른쪽으로 x, 아래로 y 만큼 진행한 장소) p60참고
			++x;
		}
	}
	//throw gcnew System::NotImplementedException();
}

void Draw(Object* state, int w, int h)
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

void Update(Object* state, char input, int w, int h)
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
	if (tx < 0 || ty <  0 || tx >= w || ty >= h)
	{
		return;
	}

	// A. 이동할 곳이 빈칸 또는 목적지 -> 플레이어 이동
	int p = y * w + x; // 플레이어 현재 위치
	int tp = ty * w + tx; // 목표 위치 ( TargetPosition)
	if (state[tp] == OBJ_SPACE || state[tp] == OBJ_GOAL)
	{
		// 이동할 곳이 목적지라면 목적지 위 사람, 아니면 그냥 사람
		state[tp] = (state[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;

		// 현재 플레이어의 위치 가 목적지 위라면 목적지로 아니면 공백으로
		state[p] = (state[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
	}
	// B. 이동할 곳이 상자 -> 그 방향 다음 칸이 공백이나 목적지면 이동
	else if (state[tp] == OBJ_BLOCK || state[tp] == OBJ_BLOCK_ON_GOAL)
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


	//throw gcnew System::NotImplementedException();
}




