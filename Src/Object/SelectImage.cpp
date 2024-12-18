#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/DataBank.h"
#include "../Scene/SelectScene.h"
#include "../Object/Character/SelectPlayer.h"
#include "SelectImage.h"

SelectImage::SelectImage(SelectScene& select, std::shared_ptr<SelectPlayer> player) : selectScene_(select), player_(player)
{
	imgPlayerNum_ = nullptr;
	imgLeftPoint_ = -1;
	imgRightPoint_ = -1;

	//座標(四角形を作るために2つの三角形を使う)
	vertices_[0].pos = VGet(VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// 左下
	vertices_[1].pos = VGet(VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z + 12.0f);	// 右下
	vertices_[2].pos = VGet(VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z);				// 左上
	vertices_[3].pos = VGet(VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z);			// 右上

	//UV座標（テクスチャ座標）
	vertices_[0].u = 0.0f / 4.0f;	vertices_[0].v = 1.0f;	// 左下
	vertices_[1].u = 1.0f / 4.0f;	vertices_[1].v = 1.0f;	// 右下
	vertices_[2].u = 0.0f / 4.0f;	vertices_[2].v = 0.0f;	// 左上
	vertices_[3].u = 1.0f / 4.0f;	vertices_[3].v = 0.0f;	// 右上


	// 法線の設定（今回は省略、適当な値を設定）
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].norm = VGet(0.0f, 0.0f, 1.0f);  // 法線は-Z軸方向
	}

	// 色の設定（ディフューズカラー）
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].dif = GetColorU8(255, 255, 255, 255);  // 白色
	}

	// 色の設定（ディフューズカラー）
	for (int i = 0; i < VERTEX_NUM; i++) {
		vertices_[i].spc = GetColorU8(0, 0, 0, 0);  // 白色
	}

	leftTop_ = AsoUtility::VECTOR_ZERO;
	leftBottom_ = AsoUtility::VECTOR_ZERO;
	rightTop_ = AsoUtility::VECTOR_ZERO;
	rightBottom_ = AsoUtility::VECTOR_ZERO;

	playerNum_ = 1;
	isPad_ = false;
	role_ = 0;

	keyPressTime_ = 0.0f;
	interval_ = 0.0f;
	press_ = false;
	angle_ = 0.0f;
}

void SelectImage::Destroy(void)
{
}

void SelectImage::Init(void)
{
	Load();

	InitVertex();

	pointL_ = { LEFT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgLeftPoint_ };	
	
	pointR_ = { RIGHT_POS_X,POINT_POS_Y,POINT_SCALE,POINT_SCALE,false,imgRightPoint_ };
}

void SelectImage::Update(void)
{
	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberUpdate();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationUpdate();
		break;

	case SelectScene::SELECT::ROLE:
		RoleUpdate();
		break;

	default:
		break;
	}
}

void SelectImage::Draw(void)
{
	//将来的にはDrawPolygon3Dで描画

	switch (selectScene_.GetSelect())
	{
	case SelectScene::SELECT::NUMBER:
		NumberDraw();
		break;

	case SelectScene::SELECT::OPERATION:
		OperationDraw();
		break;

	case SelectScene::SELECT::ROLE:
		RoleDraw();
		break;

	default:
		break;
	}
	
	DrawFormatString(Application::SCREEN_SIZE_X - 100,0, 0x000000, "L : %d", pointL_.isToggle_);
	DrawFormatString(Application::SCREEN_SIZE_X - 100,20, 0x000000, "R : %d", pointR_.isToggle_);
	//球
	//左下
	DrawSphere3D(vertices_[0].pos, 10.0f, 10, 0xFF0000, 0xFF0000, false);
	//右下
	DrawSphere3D(vertices_[1].pos, 10.0f, 10, 0x00FF00, 0x00FF00, false);
	//左上
	DrawSphere3D(vertices_[2].pos, 10.0f, 10, 0x0000FF, 0x0000FF, false);
	//右上
	DrawSphere3D(vertices_[3].pos, 10.0f, 10, 0x000000, 0x000000, false);
	
	for (int i = 0; i < 4; i++)
	{
		DrawFormatString(0, 40 + (20 * i), 0xFF0000, "pos %0.2f,%0.2f,%0.2f", vertices_[i].pos.x, vertices_[i].pos.y, vertices_[i].pos.z);
	}
}

void SelectImage::Load(void)
{
	//画像
	imgPlayerNum_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PLAYER_NUM).handleIds_;

	imgRightPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RIGHT_POINT).handleId_;
	if (imgRightPoint_ == -1)
	{
		return;
	}

	imgLeftPoint_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::LEFT_POINT).handleId_;
	if (imgLeftPoint_ == -1)
	{
		return;
	}
}

void SelectImage::NumberUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();


	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１追加(中身は1～4に収める)
			playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ % SceneManager::PLAYER_NUM) + 1 : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//人数を１削除(中身は1～4に収める)
			playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM;
			if (playerNum_ == 0)playerNum_ = 4;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, playerNum_ = (playerNum_ + 3) % SceneManager::PLAYER_NUM : interval_;
			if (playerNum_ == 0)playerNum_ = 4;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&入力デバイス選択へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//プレイヤー人数の設定
		data.Input(SceneManager::PLAY_MODE::USER, playerNum_);
		//ディスプレイの設定
		data.Input(DataBank::INFO::DHISPLAY_NUM, playerNum_);
		data.Input(DataBank::INFO::USER_NUM, playerNum_);

		//CPU人数の設定(CPUは１人から３人)
		data.Input(SceneManager::PLAY_MODE::CPU, (SceneManager::PLAYER_NUM - playerNum_));

		//ウィンドウ複製の準備
		SceneManager::GetInstance().RedySubWindow();

		//カメラの設定
		auto cameras = SceneManager::GetInstance().GetCameras();
		for (int i = 0; i < cameras.size(); i++)
		{
			cameras[i]->SetPos(SelectScene::DEFAULT_CAMERA_POS, SelectScene::DEFAULT_TARGET_POS);
			cameras[i]->ChangeMode(Camera::MODE::FIXED_POINT);
		}

		selectScene_.ChangeSelect(SelectScene::SELECT::OPERATION);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	vertices_[0].u = ((float)(playerNum_)-1.0f) / 4.0f;		vertices_[0].v = 1.0f;	// 左下
	vertices_[1].u = (float)(playerNum_) / 4.0f;			vertices_[1].v = 1.0f;	// 右下
	vertices_[2].u = ((float)(playerNum_)-1.0f) / 4.0f;		vertices_[2].v = 0.0f;	// 左上
	vertices_[3].u = (float)(playerNum_) / 4.0f;			vertices_[3].v = 0.0f;	// 右上
}

void SelectImage::OperationUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	data.Input(SceneManager::CNTL::PAD, 2);
	data.Input(SceneManager::CNTL::PAD, 3);
	data.Input(SceneManager::CNTL::PAD, 4);

	//右の矢印がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//左
	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//キーを押した際に選択しているものを反転(2種類しかないので)
			isPad_ = !isPad_;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, isPad_ = !isPad_ : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下で決定&役職選択へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//1Pの操作の設定
		(isPad_) ? data.Input(SceneManager::CNTL::PAD, 1) : data.Input(SceneManager::CNTL::KEYBOARD, 1);
		//selectedCntl_ = (isPad_) ? SceneManager::CNTL::PAD : SceneManager::CNTL::KEYBOARD;

		selectScene_.ChangeSelect(SelectScene::SELECT::ROLE);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}
}

void SelectImage::RoleUpdate(void)
{
	DataBank& data = DataBank::GetInstance();
	float delta = 2.0f * SceneManager::GetInstance().GetDeltaTime();

	//右の三角形がONの時にキーの右に値する入力をし続けると
	if (pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ + 1) % SceneManager::PLAYER_NUM;
		}

		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ増やしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ + 1) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointR_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	if (pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		if (!press_)
		{
			press_ = true;

			//役職を選択
			role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM;
		}
		//キーが押されている間経過時間を加算していく
		keyPressTime_ += delta;

		//経過時間がある一定時間経った場合
		if (keyPressTime_ > SELECT_TIME)
		{
			//インターバルを加算していく
			interval_ += delta;

			//インターバル1秒ごとにプレイ人数を１ずつ減らしていく
			(interval_ > INTERVAL_TIME) ?
				interval_ = 0.0f, role_ = (role_ - 1 + SceneManager::PLAYER_NUM) % SceneManager::PLAYER_NUM : interval_;
		}
	}
	else if (pointL_.isToggle_)
	{
		keyPressTime_ = 0.0f;
		interval_ = INTERVAL_TIME;
		press_ = false;
	}

	//スペースキー押下でゲーム画面へ
	if (selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::DECIDE)
	{
		//役職の設定
		data.Input(static_cast<SceneManager::ROLE>(role_), playerNum_);

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//選択する矢印
	if (!pointR_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::RIGHT)
	{
		pointR_.isToggle_ = true;
		pointL_.isToggle_ = false;
	}

	if (!pointL_.isToggle_ &&
		selectScene_.GetKeyConfig() == SelectScene::KEY_CONFIG::LEFT)
	{
		pointR_.isToggle_ = false;
		pointL_.isToggle_ = true;
	}

	//UV座標（テクスチャ座標）
	vertices_[0].u = (float)(role_) / 4.0f;				vertices_[0].v = 1.0f;	// 左下
	vertices_[1].u = ((float)(role_) + 1.0f) / 4.0f;	vertices_[1].v = 1.0f;	// 右下
	vertices_[2].u = (float)(role_) / 4.0f;				vertices_[2].v = 0.0f;	// 左上
	vertices_[3].u = ((float)(role_) +1.0f) / 4.0f;		vertices_[3].v = 0.0f;	// 右上

	player_->SetRole(role_);
}

void SelectImage::NumberDraw(void)
{
	// 2つの三角形を描画（テクスチャ付き）
	// 1つ目の三角形
	VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	// 2つ目の三角形
	VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	// 三角形を描画
	DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1つ目の三角形
	DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2つ目の三角形

	////人数選択画像の描画
	//DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
	//	Application::SCREEN_SIZE_Y / 2,
	//	1.0f, 0.0f,
	//	imgPlayerNum_[playerNum_ - 1],	//配列は0～3なので
	//	true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "num : %d", playerNum_);

}

void SelectImage::OperationDraw(void)
{
	//仮置き
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		1.0f, 0.0f,
		imgPlayerNum_[(int)(isPad_)],
		true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "ope : %d",isPad_);
}

void SelectImage::RoleDraw(void)
{
	// 2つの三角形を描画（テクスチャ付き）
	// 1つ目の三角形
	VERTEX3D triangle1[3] = { vertices_[2], vertices_[1], vertices_[0] };
	// 2つ目の三角形
	VERTEX3D triangle2[3] = { vertices_[1], vertices_[2], vertices_[3] };

	// 三角形を描画
	DrawPolygon3D(triangle1, 2, *imgPlayerNum_, true);  // 1つ目の三角形
	DrawPolygon3D(triangle2, 2, *imgPlayerNum_, true);  // 2つ目の三角形

	////仮置き
	//DrawRotaGraph(Application::SCREEN_SIZE_X / 2,
	//	Application::SCREEN_SIZE_Y / 2,
	//	1.0f, 0.0f,
	//	imgPlayerNum_[(int)(role_)],
	//	true, false);

	PointsDraw();

	DrawFormatString(Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2, 0x000000, "role : %d",role_);
}

void SelectImage::PointsDraw(void)
{
	//左の矢印の描画(右の矢印は暗めにする)
	if (pointL_.isToggle_)
	{
		//右の矢印画像を描画し、同じ画像を減算ブレンドする
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		//普通に描画
		pointL_.PointDraw();
	}
	//右の矢印の描画(左の矢印は暗めにする)
	if (pointR_.isToggle_)
	{
		//左の矢印画像を描画し、同じ画像を減算ブレンドする
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//ブレンドモードを戻す

		//普通に描画
		pointR_.PointDraw();
	}

	//どちらの矢印も選んでいないときの描画
	if (!pointL_.isToggle_ && !pointR_.isToggle_)
	{
		//普通に描画
		pointL_.PointDraw();
		pointR_.PointDraw();
		//↑の上の画像に減算ブレンドする
		SetDrawBlendMode(DX_BLENDMODE_SUB, 128);
		pointL_.PointDraw();
		pointR_.PointDraw();
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	}
}

void SelectImage::InitVertex(void)
{
	leftTop_ = { VERTEX_LEFT_X, VERTEX_TOP_Y, VERTEX_Z };
	leftBottom_ = { VERTEX_LEFT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f*/ };
	rightTop_ = { VERTEX_RIGHT_X, VERTEX_TOP_Y, VERTEX_Z };
	rightBottom_ = { VERTEX_RIGHT_X, VERTEX_UNDER_Y, VERTEX_Z /*+ 12.0f */};

	//座標(四角形を作るために2つの三角形を使う)
	vertices_[0].pos = leftBottom_;		// 左下
	vertices_[1].pos = rightBottom_;	// 右下
	vertices_[2].pos = leftTop_;		// 左上
	vertices_[3].pos = rightTop_;		// 右上
}

VECTOR SelectImage::RotateVertex(VECTOR pos, VECTOR center, float angle)
{
	//メッシュの中心点を計算(今はとりあえずメッシュの真ん中の座標)
	VECTOR Center;
	Center.x = (VERTEX_RIGHT_X + VERTEX_LEFT_X) * 0.5f;
	Center.y = (VERTEX_TOP_Y + VERTEX_UNDER_Y) * 0.5f;
	Center.z = VERTEX_Z;

	//半径
	const float DIS = 50.0f;

	// 時間進行
	angle_ += SceneManager::GetInstance().GetDeltaTime();

	// 回転行列を適用して頂点を回転

	for (int i = 1; i < 4; i += 2)
	{
		//XZ平面上で回転させる
		vertices_[i].pos.x = Center.x + (DIS * (sinf(angle_)));
		vertices_[i].pos.z = Center.z + (DIS * (cosf(angle_)));
	}
	for (int i = 0; i < 4; i += 2)
	{
		vertices_[i].pos.x = Center.x + (-DIS * (sinf(angle_)));
		vertices_[i].pos.z = Center.z + (-DIS * (cosf(angle_)));
	}
	return AsoUtility::VECTOR_ZERO;
}

void SelectImage::Point::PointDraw(void)
{
	DrawRotaGraph(pos.x, pos.y,
		POINT_SCALE_RATE, 0.0f,
		imgHandle_,
		true, false);
}