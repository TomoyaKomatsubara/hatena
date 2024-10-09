#include <Novice.h>
#include <stdlib.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

const char kWindowTitle[] = "[]";

void PrintScore(int targetNum)//小松原追加
{
	int numberGraphs[10] = { 0 };

	numberGraphs[0] = Novice::LoadTexture("./Resources/Images/0.png");
	numberGraphs[1] = Novice::LoadTexture("./Resources/Images/1.png");
	numberGraphs[2] = Novice::LoadTexture("./Resources/Images/2.png");
	numberGraphs[3] = Novice::LoadTexture("./Resources/Images/3.png");
	numberGraphs[4] = Novice::LoadTexture("./Resources/Images/4.png");
	numberGraphs[5] = Novice::LoadTexture("./Resources/Images/5.png");
	numberGraphs[6] = Novice::LoadTexture("./Resources/Images/6.png");
	numberGraphs[7] = Novice::LoadTexture("./Resources/Images/7.png");
	numberGraphs[8] = Novice::LoadTexture("./Resources/Images/8.png");
	numberGraphs[9] = Novice::LoadTexture("./Resources/Images/9.png");

	int number = targetNum;
	int numberAlley[6] = { 0 };


	numberAlley[0] = number / 100000;
	number %= 100000;

	numberAlley[1] = number / 10000;
	number %= 10000;

	numberAlley[2] = number / 1000;
	number %= 1000;

	numberAlley[3] = number / 100;
	number %= 100;

	numberAlley[4] = number / 10;
	number %= 10;
	numberAlley[5] = number;
	number = targetNum;

	for (int i = 0; i < 6; i++) {
		Novice::DrawSprite(100 + (i * 60), 20, numberGraphs[numberAlley[i]], 1.0f, 1.0f, 0.0f, WHITE);
	}
}





// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);


	srand(unsigned int(time(nullptr)));
	struct Vector2 {
		float x;
		float y;
	};

	struct Player {
		Vector2 position;
		Vector2 velocity;
	};

	Player player{
		{100.0f,600.0f},
		{0.0f,0.0f}
	};

	struct Character {
		Vector2 position;
		Vector2 velocity;
		bool isBoot;
	};
	Character hurdle[10];
	for (int i = 0; i < 10; i++) {
		hurdle[i].position.x = 1300.0f;
		hurdle[i].position.y = 600.0f;
		hurdle[i].velocity.x = -10.0f;
		hurdle[i].velocity.y = 0.0f;
		hurdle[i].isBoot = true;
	}

	int score = 0;
	float junpPower = 1.5f;
	int baseCoolTime = 200;
	int coolTime = baseCoolTime;
	int level = 0;

	enum Shene {
		TITLE,
		GAME,
		END
	};
	int gameShene = TITLE;

#pragma region addBy1c05_1
	//小松原追加
	float midairJumpPower = 1.5f;
	int isMJump = true;

	//小松原ここまで
#pragma endregion addBy1c05_1 end

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		switch (gameShene) {
		case TITLE:
			if (Novice::CheckHitKey(DIK_SPACE) && !preKeys[DIK_SPACE]) {
				gameShene = GAME;
			}
			//初期化
			player.position.y = 600.0f;
			player.velocity.y = 0.0f;
			for (int i = 0; i < 10; i++) {
				hurdle[i].position.x = 1300.0f;
				hurdle[i].position.y = 600.0f;
				hurdle[i].velocity.x = -10.0f;
				hurdle[i].velocity.y = 0.0f;
			}
			baseCoolTime = 200;
			coolTime = baseCoolTime;
			junpPower = 1.5f;
			score = 0;
			break;
		case GAME:

			//ジャンプ　長押しでチャージ、離したらジャンプ
			if (Novice::CheckHitKey(DIK_SPACE) && player.position.y >= 600.0f) {
				junpPower += 0.1f;
			}
			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE] && player.position.y >= 600.0f) {
				player.velocity.y = junpPower * -10.0f;
				junpPower = 1.5f;
			}
			//チャージ限界
			if (junpPower > 5.0f) {
				junpPower = 5.0f;
			}

#pragma region addBy1c05_02
			//小松原追加
			//空中ジャンプ
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE] && player.position.y < 600.0f && !isMJump)
			{
				player.velocity.y = midairJumpPower * -10.0f;
				isMJump = true;
			}
			//小松原ここまで
#pragma endregion addBy1c05_02 end


			if (player.position.y >= 600.0f)
			{
				isMJump = false;
			}



			//プレイヤーの動き　
			player.position.y += player.velocity.y;
			//重力
			

			if (isMJump && keys[DIK_SPACE] && preKeys[DIK_SPACE] && player.position.y < 600.0f && player.velocity.y >= 0.0f) {

				player.velocity.y += 0.3f;
			}
			else{ player.velocity.y += 2.0f; }
			//着地状態
			if (player.position.y >= 600.0f) {
				player.velocity.y = 0.0f; player.position.y = 600.0f;
			}

			//障害物
			for (int i = 0; i < 10; i++) {
				if (hurdle[i].isBoot == true) {
					//動く
					hurdle[i].position.x += hurdle[i].velocity.x;
					//画面外で消える
					if (hurdle[i].position.x <= -300) {
						hurdle[i].isBoot = false;
						hurdle[i].position.x = 1600.0f; hurdle[i].position.y = float(rand() % 450 + 150);
					}
					//下側の障害物　当たったらゲームオーバー
					if (player.position.x + 30 >= hurdle[i].position.x && player.position.x <= hurdle[i].position.x + 30 && player.position.y + 30 >= hurdle[i].position.y) {
						//gameShene = END;
					}
					//上側の障害物　当たったらゲームオーバー
					if (player.position.x + 30 >= hurdle[i].position.x && player.position.x <= hurdle[i].position.x + 30 && player.position.y <= hurdle[i].position.y - 400) {
						//gameShene = END;
					}
				} else if (coolTime <= 0) {
					//障害物出現・クールタイムリセット
					coolTime = baseCoolTime;
					hurdle[i].isBoot = true;
				}
				//経過時間に応じて障害物のスピード上昇
				if (hurdle[i].velocity.x > -20.0f) {
					hurdle[i].velocity.x = -10.0f - level / 10;
				}
			}
			//難易度上昇用のレベル変数
			if (level < 99) {
				level = score / 500;
			}
			//経過時間に応じて障害物の間隔を短くする
			if (baseCoolTime >= 90) {
				baseCoolTime = 200 - level * 5;
			}
			coolTime--;
			//スコア
			score++;

			break;
		case END:
			if (Novice::CheckHitKey(DIK_SPACE) && !preKeys[DIK_SPACE]) {
				gameShene = TITLE;
			}
			break;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		switch (gameShene) {
		case TITLE:
			Novice::ScreenPrintf(100, 100, "TITLE");
			break;
		case GAME:
			//プレイヤー
			Novice::DrawBox(int(player.position.x), int(player.position.y), 30, 50, 0.0f, WHITE, kFillModeSolid);

			//障害物
			for (int i = 0; i < 10; i++) {
				Novice::DrawBox(int(hurdle[i].position.x), int(hurdle[i].position.y), 30, 500, 0.0f, WHITE, kFillModeSolid);
				//上側障害物　時間経過に応じて下との隙間が狭くなっていく
				Novice::DrawBox(int(hurdle[i].position.x), int(hurdle[i].position.y - 900 + level * 2), 30, 500, 0.0f, WHITE, kFillModeSolid);
			}

			//ゲージ
			Novice::DrawBox(50, 650, 50, int((junpPower - 1.5f) * 100), float(M_PI), RED, kFillModeSolid);

			Novice::ScreenPrintf(100, 300, "score = %d", score);
			PrintScore(score);
			break;
		case END:
			Novice::ScreenPrintf(100, 100, "END");
			Novice::ScreenPrintf(100, 300, "score = %d", score);

			break;

		}

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
